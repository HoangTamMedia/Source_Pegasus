#pragma once

// Item collection. Display only: the entry table is local, and handing an item
// in does nothing yet. The server hookup lands in OnDonate() and in whatever
// replaces FillMockSets().
class CPegasusCollection
{
public:
	enum
	{
		SET_COUNT  = 6,		// collections listed down the side
		SLOT_COUNT = 12,	// slots in the grid, 4 x 3
		RARITY_COUNT = 4,	// the atlas ships four frame colours
	};

	struct SLOT
	{
		int  iItemType;		// -1 for an empty pedestal
		int  iItemLevel;
		int  iRarity;		// which frame to use, 0..3
		bool bFilled;
	};

	struct SET
	{
		const char* szName;
		int   iBonus;		// percent bonus once complete
		SLOT  aSlot[SLOT_COUNT];
	};

	CPegasusCollection();
	~CPegasusCollection();

	void LoadImages();
	void ReleaseImages();

	void Toggle();
	void Open();
	void Close();
	bool IsVisible() const { return m_bShow; }

	bool UpdateMouseEvent();
	bool IsMouseOver();
	void Render();

private:
	void FillMockSets();
	void OnDonate(int iSlot);
	int  CountFilled(int iSet) const;

	bool m_bShow;
	bool m_bLoaded;
	int  m_iSelectedSet;
	int  m_iHoverSlot;

	SET  m_Set[SET_COUNT];
};

extern CPegasusCollection gPegasusCollection;
