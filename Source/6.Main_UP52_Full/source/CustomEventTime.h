
#include "Protocol.h"
class SauEventTime
{
public:
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

	struct PMSG_SAUEVENT_SEND
	{
		PSWMSG_HEAD header;
		BYTE count;
	};

	struct ITEM_BAG_INFO
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

	SauEventTime();
	virtual ~SauEventTime();
	void SauEventTime::RecvListEventTime(BYTE* Recv);
	void SauEventTime::RecvListItem(BYTE* Recv);
	void Init();

	void PrintDropBox(float PosX, float PosY, float Width, float Height, float Arg5, int Arg6);
	void SauEventTime::OpenSauEvent();
	void SauEventTime::DrawSauEventTime();
	void SauEventTime::UpdateTime();
	std::vector<SendEventTime> m_ListEventTime;
	std::vector<ITEM_BAG_INFO> m_ListItemBag;
};

extern SauEventTime gSauEventTime;