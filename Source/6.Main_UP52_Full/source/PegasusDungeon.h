#pragma once

// Dungeon gateway. Display only for now: picking a dungeon and a difficulty
// works, but Enter does not send anything yet. Hooking it to the server means
// filling in OnEnter() and swapping the static table below for whatever the
// server reports as unlocked.
class CPegasusDungeon
{
public:
	enum
	{
		DUNGEON_COUNT   = 4,
		DIFFICULTY_COUNT = 4,	// the art ships four crest colours
	};

	struct DUNGEON
	{
		const char* szName;
		int   iMinLevel;
		int   iPartyNeeded;		// 0 = solo allowed
		bool  bUnlocked;
	};

	CPegasusDungeon();
	~CPegasusDungeon();

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
	void OnEnter();
	void RenderDungeonTab(int iIndex, float x, float y, float w, float h);
	void RenderDifficulty(int iIndex, float cx, float cy, float sz);

	bool m_bShow;
	bool m_bLoaded;
	int  m_iSelected;
	int  m_iDifficulty;

	DUNGEON m_Dungeon[DUNGEON_COUNT];
};

extern CPegasusDungeon gPegasusDungeon;
