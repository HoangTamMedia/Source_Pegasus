// SauEventTime.cpp: implementation of the CSauEventTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MemScript.h"
#include "Util.h"
#include "Notice.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "Map.h"
#include "MemScript.h"
#include "Notice.h"
#include "Util.h"
#include "ItemOptionRate.h"
#include "ObjectManager.h"
#include "Guild.h"
#include "Move.h"
#include "Monster.h"
#include "ItemBagManager.h"
#include "Party.h"
#include "CashShop.h"
#include "MapServerManager.h"
#include "ServerInfo.h"
#include "CustomMonster.h"
#include "CustomEventTime.h"
#include "pugixml.hpp"
CSauEventTime gSauEventTime;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSauEventTime::CSauEventTime()
{
	this->Init();
}


CSauEventTime::~CSauEventTime()
{
}

void CSauEventTime::Init()
{

}

void CSauEventTime::LoadFileXML(char* FilePath)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(FilePath);
	if (res.status != pugi::status_ok) {
		ErrorMessageBox("File %s load fail. Error: %s", FilePath, res.description());
		return;
	}

	pugi::xml_node oSauEventTime = file.child("SauEventTime");
	if (!oSauEventTime) {
		ErrorMessageBox("File %s is missing SauEventTime node.", FilePath);
		return;
	}

	this->Enable = oSauEventTime.attribute("Enable").as_int();

	this->m_DataSauEvent.clear();
	pugi::xml_node ConfigEvent = oSauEventTime.child("TimeEventConfig");
	if (!ConfigEvent) {
		ErrorMessageBox("File %s is missing TimeEventConfig node.", FilePath);
		return;
	}

	int IndexMocNap = 1;
	for (pugi::xml_node Event = ConfigEvent.child("Event"); Event; Event = Event.next_sibling("Event"))
	{
		SAU_EVENT_TIME infoData = { 0 };
		infoData.IndexEvent = IndexMocNap++;

		infoData.TypeEvent = Event.attribute("TypeEvent").as_int();
		infoData.TimeSlot = Event.attribute("TimeSlot").as_int();
		infoData.IndexBoss = Event.attribute("IndexBoss").as_int();
		infoData.IndexItemBag = Event.attribute("IndexItemBag").as_int();
		infoData.GateMove[0] = Event.attribute("GateMove1").as_int();
		infoData.GateMove[1] = Event.attribute("GateMove2").as_int();
		infoData.GateMove[2] = Event.attribute("GateMove3").as_int();
		infoData.SizeBoss = Event.attribute("SizeBoss").as_float();
		strncpy_s(infoData.NameEvent, Event.attribute("Name").as_string(), sizeof(infoData.NameEvent) - 1);
		infoData.NameEvent[sizeof(infoData.NameEvent) - 1] = '\0'; // Ensure null-termination

		this->m_DataSauEvent.insert(std::make_pair(infoData.IndexEvent, infoData));
		//LogAdd(LOG_RED, "[EventTime] [%s] Size %d ", infoData.NameEvent, this->m_DataSauEvent.size());
	}

	LogAdd(LOG_BLUE, "[EventTime] [%d] Size %d ", this->Enable, this->m_DataSauEvent.size());
}


void CSauEventTime::UserSendClientInfo(int aIndex)
{

	if (gObj[aIndex].Type != OBJECT_USER)
	{
		return;
	}

	if (gObjIsConnected(aIndex) == false)
	{
		return;
	}
	if (gObj[aIndex].IsBot >= 1 || gObj[aIndex].m_OfflineMode != 0 || gObj[aIndex].IsFakeOnline != 0)
	{
		return;
	}
	BYTE send[4096];
	PMSG_SAUEVENT_SEND pMsg = { 0 };
	// ---
	pMsg.header.set(0xD7, 0x41, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;


	for (std::map<int, SAU_EVENT_TIME>::iterator it = this->m_DataSauEvent.begin(); it != this->m_DataSauEvent.end(); it++)
	{
		if (it == this->m_DataSauEvent.end())
		{
			break;
		}
		SendEventTime info;
		info.IndexEvent = it->second.IndexEvent;;
		memset(info.EventName, 0, sizeof(info.EventName));
		memcpy(info.EventName, it->second.NameEvent, sizeof(info.EventName));
		if (it->second.TypeEvent == 0)
		{
			switch (it->second.TimeSlot)
			{
			case 0:
				info.TimeBoss = gServerDisplayer.EventBc;
				break;

			case 1:
				info.TimeBoss = gServerDisplayer.EventDs;
				break;
			case 2:
				info.TimeBoss = gServerDisplayer.EventCc;
				break;
				//case 3:
				//	info.TimeBoss = gServerDisplayer.EventCTCMini;
				//	break;
				//case 4:
				//	info.TimeBoss = gServerDisplayer.BEventThanMa;
				//	break;
			default:
				info.TimeBoss = -1;
				break;
			}
		}
		else if (it->second.TypeEvent == 1)
		{
			info.TimeBoss = gServerDisplayer.EventInvasion[it->second.TimeSlot];
		}
		info.MoveGate[0] = it->second.GateMove[0];
		info.MoveGate[1] = it->second.GateMove[1];
		info.MoveGate[2] = it->second.GateMove[2];
		info.IndexBag = it->second.IndexItemBag;
		info.IndexBoss = it->second.IndexBoss;
		info.Coin[0] = gCustomMonster.GetCoin(0, it->second.IndexBoss);
		info.Coin[1] = gCustomMonster.GetCoin(1, it->second.IndexBoss);
		info.Coin[2] = gCustomMonster.GetCoin(2, it->second.IndexBoss);
		info.SizeBoss = it->second.SizeBoss;
		if ((size + sizeof(info) >= 4096))
		{
			break;
		}
		pMsg.count++;
		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);
	}
	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);
	// ---
	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}


void CSauEventTime::SendInfoItemBag(int aIndex, int BagId)
{
	if (gObj[aIndex].Type != OBJECT_USER)
	{
		return;
	}

	if (gObjIsConnected(aIndex) == false)
	{
		return;
	}
	if (gObj[aIndex].IsBot >= 1 || gObj[aIndex].m_OfflineMode != 0 || gObj[aIndex].IsFakeOnline != 0)
	{
		return;
	}

	BYTE send[4096];
	PMSG_SAUEVENT_SEND pMsg = { 0 };
	pMsg.header.set(0xD7, 0x42, 0);

	int size = sizeof(pMsg);
	pMsg.count = 0;

	for (auto it = gItemBagManager.m_ItemBagManagerInfo.begin(); it != gItemBagManager.m_ItemBagManagerInfo.end(); ++it)
	{
		if (it->second.Index != -1 && it->second.Index == BagId)
		{
			for (int i = 0; i < it->second.ItemBag.m_count; i++)
			{
				ITEM_BAG_INFO* lpInfo = it->second.ItemBag.GetInfo(i);

				ITEM_BAG_INFO_SEND info;
				info.Index = lpInfo->Index;
				info.MinLevel = lpInfo->MinLevel;
				info.MaxLevel = lpInfo->MaxLevel;
				info.Option1 = lpInfo->Option1;
				info.Option2 = lpInfo->Option2;
				info.Option3 = lpInfo->Option3;
				info.NewOption = lpInfo->NewOption;
				info.SetOption = lpInfo->SetOption;
				info.SocketOption = lpInfo->SocketOption;
				info.RateDrop = lpInfo->DropRate;

				if ((size + sizeof(info) >= sizeof(send)))
				{
					break;
				}

				//LogAdd(LOG_RED, "%d ~ %d", info.Index, it->second.ItemBag.m_count);
				pMsg.count++;
				memcpy(&send[size], &info, sizeof(info));
				size += sizeof(info);
			}
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}