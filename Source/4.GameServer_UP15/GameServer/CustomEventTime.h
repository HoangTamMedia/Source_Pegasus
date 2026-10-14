// CustomHealthBar.h: interface for the CCustomHealthBar class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "Protocol.h"
// stuct load info
struct SAU_EVENT_TIME
{
	int IndexEvent;
	char NameEvent[30];
	int TypeEvent;
	int TimeSlot;
	int IndexBoss;
	int IndexItemBag;
	int GateMove[3];
	float SizeBoss;
};
// stuct send
struct SendEventTime
{
	int IndexEvent;
	char EventName[30];
	int TimeBoss;
	int MoveGate[3];
	int IndexBag;
	int IndexBoss;
	int Coin[3];
	float SizeBoss;
};

struct ITEM_BAG_INFO_SEND
{
	int Index;
	int MinLevel;
	int MaxLevel;
	int Option1;
	int Option2;
	int Option3;
	int NewOption;
	int SetOption;
	int SocketOption;
	int RateDrop;
};
// count list de send
struct PMSG_SAUEVENT_SEND
{
	PSWMSG_HEAD header; // C2:F3:E2
	BYTE count;
};


class CSauEventTime
{
public:
	CSauEventTime();
	virtual ~CSauEventTime();
	void Init();
	void LoadFileXML(char* FilePath);
	void CSauEventTime::UserSendClientInfo(int aIndex);
	void CSauEventTime::SendInfoItemBag(int aIndex, int BagId);

private:
	int Enable;
	int Firework;
	int Notice;
	std::map<int, SAU_EVENT_TIME> m_DataSauEvent;
};

extern CSauEventTime gSauEventTime;