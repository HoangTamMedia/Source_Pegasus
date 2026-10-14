#pragma once

// Daily attendance.
//
// The window draws, but it decides nothing. Which day is claimable and which
// days are already spent both come from the server on 0xF3:0x60, a click sends
// 0xF3:0x61, and 0xF3:0x62 brings back the authoritative mask - so the calendar
// is redrawn from what the server holds rather than from what the click hoped
// would happen. A client that lies to itself here only lies to itself.
//
// Until that first sync arrives the calendar has nothing to show, and says so
// instead of inventing a month.
class CPegasusDaily
{
public:
	enum
	{
		DAY_COUNT = 28,		// four weeks
		DAYS_PER_ROW = 7,
	};

	enum eDayState
	{
		DAY_LOCKED = 0,		// still ahead
		DAY_READY,			// today, claimable
		DAY_CLAIMED,
		DAY_MISSED,
	};

	struct DAY
	{
		int       iItemType;	// reward shown on the card, -1 for none
		int       iItemLevel;
		int       iIcon;		// 0..2, which calendar colour
		bool      bPremium;		// paid track only
		eDayState eState;
	};

	CPegasusDaily();
	~CPegasusDaily();

	void LoadImages();
	void ReleaseImages();

	void Toggle();
	void Open();
	void Close();
	bool IsVisible() const { return m_bShow; }

	bool UpdateMouseEvent();
	bool IsMouseOver();
	void Render();

	// Called from WSclient.cpp's 0xF3 dispatch. The raw packet is handed over
	// so its layout stays in one file next to the struct that describes it.
	void OnSyncPacket(const BYTE* pRecv);
	void OnClaimResultPacket(const BYTE* pRecv);

	// The connection dropping has to put the window back to "nothing known",
	// or a reconnect would show the previous account's calendar.
	void Reset();

private:
	// The rewards themselves are not in the sync packet - it carries state, not
	// contents - so the card art comes from a local table that mirrors
	// Data\Custom\DailyReward.txt. Change one and change the other.
	void BuildRewardTable();

	// Turns Today plus the claimed mask into the four card states.
	void ApplyServerState();

	void OnClaim(int iDay);

	// Drawing, split the way the panel art is: a band up top, the grid, the
	// button on the lower band, a status strip along the bottom. fPulse is the
	// shared 0..1 breath, passed down so nothing animates on a clock of its own.
	void RenderHeader(float fPulse);
	void RenderCard(int iDay, float fPulse);
	void RenderClaimButton(float fPulse);
	void RenderStatus();

	// How many of the 28 have been taken. Header furniture only - the mask
	// itself stays the authority on what is claimed.
	int  ClaimedCount() const;

	bool  m_bShow;
	bool  m_bLoaded;
	bool  m_bPremium;
	bool  m_bSynced;		// false until the first 0x60 arrives
	int   m_iToday;			// 0-based
	int   m_iHoverDay;
	bool  m_bHoverClaim;	// cursor over the claim button
	int   m_iLastResult;	// eDailyResult from the last claim, -1 for none
	DWORD m_dwClaimedMask;

	DAY  m_Day[DAY_COUNT];
};

// Mirrors GameServer\PegasusDaily.h. Refused claims come back as one of these.
enum eDailyResult
{
	DAILY_OK = 0,
	DAILY_ERR_NOT_TODAY,
	DAILY_ERR_ALREADY,
	DAILY_ERR_PREMIUM,
	DAILY_ERR_NO_ROOM,
	DAILY_ERR_DISABLED,
};

extern CPegasusDaily gPegasusDaily;
