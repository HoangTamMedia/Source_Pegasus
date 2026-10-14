#pragma once

// Picker for the effect and colour on your own name.
//
// The effect list is drawn live: every row renders your actual name with that
// effect running, at the size it appears in the world, so the choice is made by
// looking rather than by reading a label. The list is paged rather than scrolled
// because the rows are animating - a moving list is hard to read, and paging
// holds them still.
//
// Colour is a real pick, not a set of presets: three channel sliders, each
// drawn as the gradient it controls, with the swatches kept alongside as quick
// starting points.
//
// What it sets is g_dwPgNameFx and the three colour globals in ZzzInterface,
// which is what RenderHPBar reads when it draws the hero's name.
class CPegasusNameFx
{
public:
	CPegasusNameFx();
	~CPegasusNameFx();

	void Toggle();
	void Open();
	void Close();
	bool IsVisible() const { return m_bShow; }

	bool UpdateMouseEvent();
	bool IsMouseOver();
	void Render();

	// Read at start-up and written when the pick changes, so a chosen look
	// survives a restart.
	void LoadConfig();
	void SaveConfig();

private:
	void Apply();

	bool  m_bShow;
	int   m_iSelected;
	int   m_iHover;
	int   m_iPage;

	float m_fR, m_fG, m_fB;

	// Which slider the button went down on, so a drag keeps hold of it even
	// when the pointer wanders off the strip.
	int   m_iDragging;
};

extern CPegasusNameFx gPegasusNameFx;
