#include "stdafx.h"
#include "PegasusDaily.h"
#include "MemScript.h"
#include "Path.h"
#include "Util.h"
#include "Protocol.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "Notice.h"
#include <time.h>

CPegasusDaily gPegasusDaily;

// Today as YYYYMMDD. A plain integer compare then tells us whether the calendar
// has rolled over, with no timezone or DST arithmetic to get wrong.
static int DailyToday()
{
	time_t now = time(0);
	struct tm t;
	localtime_s(&t, &now);
	return (t.tm_year + 1900) * 10000 + (t.tm_mon + 1) * 100 + t.tm_mday;
}

CPegasusDaily::CPegasusDaily()
{
	this->Init();
}

CPegasusDaily::~CPegasusDaily()
{
}

void CPegasusDaily::Init()
{
	this->m_bEnabled = false;

	for (int n = 0; n < DAILY_DAY_COUNT; n++)
	{
		this->m_Reward[n].Day = n;
		this->m_Reward[n].ItemType = -1;
		this->m_Reward[n].ItemIndex = 0;
		this->m_Reward[n].ItemLevel = 0;
		this->m_Reward[n].Durability = 0;
		this->m_Reward[n].Premium = 0;
	}
}

void CPegasusDaily::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->Init();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if (section == 0)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					// The file is 1-based to match the calendar the player
					// sees; everything past this line is 0-based.
					int day = lpMemScript->GetNumber() - 1;

					int type  = lpMemScript->GetAsNumber();
					int index = lpMemScript->GetAsNumber();
					int level = lpMemScript->GetAsNumber();
					int dur   = lpMemScript->GetAsNumber();
					int prem  = lpMemScript->GetAsNumber();

					// A day outside the calendar is dropped rather than
					// written past the end of the array.
					if (day >= 0 && day < DAILY_DAY_COUNT)
					{
						this->m_Reward[day].Day = day;
						this->m_Reward[day].ItemType = type;
						this->m_Reward[day].ItemIndex = index;
						this->m_Reward[day].ItemLevel = level;
						this->m_Reward[day].Durability = dur;
						this->m_Reward[day].Premium = prem;
					}
				}
				else
				{
					// Any other section number would spin forever here - the
					// dispatch below has no default branch and no break, which
					// is exactly how a stray section turns into the misleading
					// "file were not configured correctly" watchdog message.
					break;
				}
			}
		}

		this->m_bEnabled = true;
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

const DAILY_REWARD_INFO* CPegasusDaily::GetReward(int Day) const
{
	if (Day < 0 || Day >= DAILY_DAY_COUNT)
	{
		return 0;
	}

	if (this->m_Reward[Day].ItemType == -1)
	{
		return 0;
	}

	return &this->m_Reward[Day];
}

// Moves the streak on when the wall date changes.
//
// Missing a day does NOT reset the streak here - the player simply cannot go
// back and claim what they slept through, which is what the client already
// draws as DAY_MISSED. Resetting on a miss is a policy choice; if you want it,
// this is the one place to add it.
void CPegasusDaily::CheckDayRollover(LPOBJ lpObj)
{
	if (lpObj == 0 || this->m_bEnabled == false)
	{
		return;
	}

	int today = DailyToday();

	if (lpObj->DailyLastDate == today)
	{
		return;
	}

	// First login ever: start on day one rather than jumping the calendar.
	if (lpObj->DailyLastDate == 0)
	{
		lpObj->DailyDay = 0;
		lpObj->DailyClaimed = 0;
		lpObj->DailyLastDate = today;
		return;
	}

	lpObj->DailyLastDate = today;

	if (lpObj->DailyDay < (DAILY_DAY_COUNT - 1))
	{
		lpObj->DailyDay++;
	}
	else
	{
		// Past the end of the calendar it starts over, so a long-running
		// account keeps getting a reason to log in.
		lpObj->DailyDay = 0;
		lpObj->DailyClaimed = 0;
	}
}

void CPegasusDaily::SendSync(int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	this->CheckDayRollover(lpObj);

	PMSG_DAILY_SYNC pMsg;

	pMsg.header.set(0xF3, 0x60, sizeof(pMsg));

	pMsg.Today = (BYTE)lpObj->DailyDay;
	pMsg.Premium = (lpObj->AccountLevel > 0) ? 1 : 0;
	pMsg.pad[0] = 0;
	pMsg.pad[1] = 0;
	pMsg.ClaimedMask = lpObj->DailyClaimed;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CPegasusDaily::SendResult(int aIndex, BYTE Result, BYTE Day)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_DAILY_CLAIM_RESULT pMsg;

	pMsg.header.set(0xF3, 0x62, sizeof(pMsg));

	pMsg.Result = Result;
	pMsg.Day = Day;
	pMsg.pad[0] = 0;
	pMsg.pad[1] = 0;

	// The authoritative mask rides back with every result, so the window
	// redraws from what the server holds instead of guessing what its own
	// click did.
	pMsg.ClaimedMask = lpObj->DailyClaimed;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

// The whole point of the system lives here: the client asks, this decides.
void CPegasusDaily::ClaimRequest(int aIndex, BYTE Day)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return;
	}

	// Nothing is claimable from the character select screen - there is no
	// inventory to put a reward into there. A crafted packet is the only way to
	// arrive here in that state, so it is dropped without an answer.
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (this->m_bEnabled == false)
	{
		this->SendResult(aIndex, DAILY_ERR_DISABLED, Day);
		return;
	}

	this->CheckDayRollover(lpObj);

	// Only the current day, whatever the client claims to be looking at. This
	// single check is what stops a crafted packet emptying the whole calendar
	// in one session.
	if (Day != (BYTE)lpObj->DailyDay || Day >= DAILY_DAY_COUNT)
	{
		this->SendResult(aIndex, DAILY_ERR_NOT_TODAY, Day);
		return;
	}

	if ((lpObj->DailyClaimed & (1 << Day)) != 0)
	{
		this->SendResult(aIndex, DAILY_ERR_ALREADY, Day);
		return;
	}

	const DAILY_REWARD_INFO* lpInfo = this->GetReward(Day);

	if (lpInfo == 0)
	{
		this->SendResult(aIndex, DAILY_ERR_DISABLED, Day);
		return;
	}

	if (lpInfo->Premium != 0 && lpObj->AccountLevel <= 0)
	{
		this->SendResult(aIndex, DAILY_ERR_PREMIUM, Day);
		return;
	}

	// The reward goes straight into the inventory rather than on the ground:
	// a drop can be stolen or lost to a full map, and the insert answers
	// success or failure in the same call, which is what DAILY_ERR_NO_ROOM
	// needs to mean anything.
	int ItemIndex = GET_ITEM(lpInfo->ItemType, lpInfo->ItemIndex);

	CItem item;

	item.m_Level = (BYTE)lpInfo->ItemLevel;

	item.Convert(ItemIndex, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF);

	// A row that names an item the server has no table entry for leaves
	// m_Index untouched, and inserting that would put a garbage item in the
	// bag. Treat it the same as a missing reward.
	if (item.m_Index != ItemIndex)
	{
		this->SendResult(aIndex, DAILY_ERR_DISABLED, Day);
		return;
	}

	// 0 in the file means "whatever this item ships with".
	item.m_Durability = (BYTE)((lpInfo->Durability > 0) ? lpInfo->Durability
		: gItemManager.GetItemDurability(item.m_Index, item.m_Level, item.m_NewOption, item.m_SetOption));

	BYTE slot = gItemManager.InventoryInsertItem(lpObj->Index, item);

	if (slot == 0xFF)
	{
		// Nothing was taken, so the day stays claimable - the player clears a
		// slot and asks again.
		this->SendResult(aIndex, DAILY_ERR_NO_ROOM, Day);
		return;
	}

	// Marked only once the item is actually in the bag. The insert above is
	// synchronous and cannot half-succeed, so there is no window in which the
	// player is charged for a reward they did not get.
	lpObj->DailyClaimed |= (1 << Day);

	gItemManager.GCItemModifySend(lpObj->Index, slot);

	this->SendResult(aIndex, DAILY_OK, Day);

	// GCNoticeSend takes a non-const char*, so the text needs to be a buffer
	// rather than a literal.
	char szNotice[64];
	strcpy_s(szNotice, "Daily reward claimed.");

	gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, szNotice);
}
