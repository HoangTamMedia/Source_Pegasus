#pragma once

enum PAINT_RESULT {
	PAINT_MORE_DATA,
	PAINT_FAIL,
	PAINT_SUCCESS,
};

enum FONT_TYPE {
	GL_g_hFont,
	GL_g_hFontSC,
	GL_g_hFontBold,
	GL_g_hFontBig,
	GL_g_hFixFont,
};

enum TextAlign
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
};

class CGlFont {
public:
	HFONT m_Font;
	GLuint m_List;
	float   texCoords_[255 - 32][4];
	unsigned int width_;
	unsigned int height_;
	unsigned int spacing_;
	unsigned int texture_;
	unsigned int fontSize_;

	std::string name_;
	bool bold_;
	bool italic_;
	PAINT_RESULT PaintAlphabet(HDC hDC, bool bMeasureOnly);
	float RenderChar(BYTE ch, float x, float y) const;
	void Print(int xs, int ys, const char* text, TextAlign align = ALIGN_LEFT);
public:
	CGlFont(char* Name, int Size, int Weight, BOOL bItalic, BOOL bUnderline);
	~CGlFont();

	void Draw(int x, int y, char* text, TextAlign align = ALIGN_LEFT);
	void DrawColor(int x, int y, COLORREF color, char* text, TextAlign align = ALIGN_LEFT);
	void DrawFormat(int x, int y, TextAlign align, char* format, ...);
	void DrawColorFormat(int x, int y, COLORREF color, TextAlign align, char* format, ...);

	void GetTextSize(char* text, SIZE& size);
	int GetTextWidth(char* text);
};

extern CGlFont* g_Font[100];