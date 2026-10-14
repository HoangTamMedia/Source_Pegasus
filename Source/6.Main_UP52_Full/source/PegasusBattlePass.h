#pragma once

// Battle Pass window. Display only for now - the tier table below is local
// mock data, and nothing is sent or received. Wiring it to the server means
// replacing FillMockSeason() and the claim handling, not the drawing.
class CPegasusBattlePass
{
public:
	enum
	{
		TIER_COUNT   = 30,		// tiers in a season
		TIERS_ON_VIEW = 5,		// columns visible at once
	};

	struct TIER
	{
		int  iFreeType;			// item type shown in the free slot, -1 for none
		int  iFreeLevel;
		int  iPremType;			// premium slot
		int  iPremLevel;
		bool bFreeClaimed;
		bool bPremClaimed;
	};

	CPegasusBattlePass();
	~CPegasusBattlePass();

	void LoadImages();
	void ReleaseImages();

	void Toggle();
	void Open();
	void Close();
	bool IsVisible() const { return m_bShow; }

	// Returns true when it swallowed the click, so the world doesn't also get it.
	bool UpdateMouseEvent();
	bool IsMouseOver();
	void Render();

private:
	void FillMockSeason();
	void RenderHeader(float x, float y, float w);
	void RenderTier(int iTier, float cx, float yRail);

	bool  m_bShow;
	bool  m_bLoaded;
	int   m_iFirstOnView;		// left-most tier column
	int   m_iLevel;				// season level reached
	int   m_iPoints;			// progress into the next level
	int   m_iPointsPerLevel;
	bool  m_bPremium;			// has the paid track

	// Which reward slot the cursor is over, so Render can name it. -1 for none;
	// the flag picks the premium row from the free one.
	int   m_iHoverTier;
	bool  m_bHoverPrem;

	TIER  m_Tier[TIER_COUNT];
};

extern CPegasusBattlePass gPegasusBattlePass;
