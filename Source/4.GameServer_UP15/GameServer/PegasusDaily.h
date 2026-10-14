#pragma once

#include "User.h"
#include "Protocol.h"		// PSBMSG_HEAD

// Daily attendance, server side.
//
// The client already models this in PegasusDaily.h: 28 days, each carrying a
// reward and one of four states, plus a premium flag. This mirrors it, and owns
// the two things the client must never decide for itself - which day it is, and
// whether a reward has already been taken.
//
// The rule that shapes the whole design: the client asks "may I claim day N",
// it never says "give me day N". Every check lives here. A client that lies
// gets refused, not obeyed.

#define DAILY_DAY_COUNT 28

// Server -> client. The whole calendar in one message; there is no incremental
// update because the state is 8 bytes and sending it whole removes a class of
// desync bug.
struct PMSG_DAILY_SYNC
{
	PSBMSG_HEAD header;		// C1:F3:60
	BYTE  Today;			// 0-based index of the claimable day
	BYTE  Premium;			// 1 if the account is on the paid track
	BYTE  pad[2];
	DWORD ClaimedMask;		// bit n set = day n already taken
};

// Client -> server. A request, not an instruction.
struct PMSG_DAILY_CLAIM_REQ
{
	PSBMSG_HEAD header;		// C1:F3:61
	BYTE Day;
};

// Server -> client. Result carries the new mask so the window can redraw from
// authority rather than guessing what its own click did.
struct PMSG_DAILY_CLAIM_RESULT
{
	PSBMSG_HEAD header;		// C1:F3:62
	BYTE  Result;			// see eDailyResult
	BYTE  Day;
	BYTE  pad[2];
	DWORD ClaimedMask;
};

enum eDailyResult
{
	DAILY_OK = 0,
	DAILY_ERR_NOT_TODAY,	// asked for a day that is not the current one
	DAILY_ERR_ALREADY,		// already in the claimed mask
	DAILY_ERR_PREMIUM,		// paid-track day, account is not premium
	DAILY_ERR_NO_ROOM,		// inventory full
	DAILY_ERR_DISABLED,
};

// One row of Data\Custom\DailyReward.txt.
struct DAILY_REWARD_INFO
{
	int  Day;				// 1-based in the file, stored 0-based
	int  ItemType;
	int  ItemIndex;
	int  ItemLevel;
	int  Durability;
	int  Premium;			// 1 = paid track only
};

class CPegasusDaily
{
public:
	CPegasusDaily();
	virtual ~CPegasusDaily();

	void Init();
	void Load(char* path);

	// Called on login and whenever the calendar changes.
	void SendSync(int aIndex);

	// Entry point for PMSG_DAILY_CLAIM_REQ.
	void ClaimRequest(int aIndex, BYTE Day);

	// Rolls the streak forward when the date changes. Safe to call often.
	void CheckDayRollover(LPOBJ lpObj);

private:
	const DAILY_REWARD_INFO* GetReward(int Day) const;
	void SendResult(int aIndex, BYTE Result, BYTE Day);

	bool m_bEnabled;
	DAILY_REWARD_INFO m_Reward[DAILY_DAY_COUNT];
};

extern CPegasusDaily gPegasusDaily;
