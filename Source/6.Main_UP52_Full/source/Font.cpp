#include "stdafx.h"
#include "Font.h"

CGlFont* g_Font[100];

CGlFont::CGlFont(char* Name, int Size, int Weight, BOOL bItalic, BOOL bUnderline) {
	m_Font = CreateFont(Size, 0, 0, 0, Weight, bItalic, bUnderline, FALSE, VIETNAMESE_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, Name);
	width_ = 128;
	height_ = 128;
	spacing_ = Size / 3;
	fontSize_ = Size;

	if (!m_Font) {
		return;
	}

	HDC hDC = CreateCompatibleDC(nullptr);
	SetMapMode(hDC, MM_TEXT);
	HFONT hFontOld = static_cast<HFONT>(SelectObject(hDC, m_Font));
	PAINT_RESULT p;

	while (PAINT_MORE_DATA == (p = PaintAlphabet(hDC, true))) {
		width_ *= 2;
		height_ *= 2;
	}

	DWORD* pBitmapBits;
	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = static_cast<int>(width_);
	bmi.bmiHeader.biHeight = -static_cast<int>(height_);
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount = 32;

	HBITMAP hbmBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, reinterpret_cast<void**>(&pBitmapBits), nullptr, 0);
	HGDIOBJ hbmOld = SelectObject(hDC, hbmBitmap);

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkColor(hDC, RGB(0, 0, 0));
	SetTextAlign(hDC, TA_TOP);

	p = PaintAlphabet(hDC, false);

	if (p == PAINT_SUCCESS) {
		glGenTextures(1, &texture_);
		BITMAP bitmap;
		GetObject(hbmBitmap, sizeof(BITMAP), &bitmap);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap.bmBits);
	}

	SelectObject(hDC, hbmOld);
	SelectObject(hDC, hFontOld);
	DeleteObject(hbmBitmap);
	DeleteDC(hDC);
}

CGlFont::~CGlFont() {
	glDeleteTextures(1, &texture_);
}

PAINT_RESULT CGlFont::PaintAlphabet(HDC hDC, bool bMeasureOnly) {
	SIZE size;
	char str[2] = TEXT("x");

	if (GetTextExtentPoint32(hDC, str, 1, &size) == 0) return PAINT_FAIL;
	spacing_ = static_cast<int>(ceil(size.cy * 0.3f));

	DWORD x = spacing_;
	DWORD y = 0;

	for (int i = 32; i < 255; i++) {
		str[0] = i;
		if (GetTextExtentPoint32(hDC, str, 1, &size) == 0) return PAINT_FAIL;

		if (x + size.cx + spacing_ > width_) {
			x = spacing_;
			y += size.cy + 1;
		}

		if (y + size.cy > height_) return PAINT_MORE_DATA;
		if (!bMeasureOnly) {
			if (ExtTextOut(hDC, x, y, ETO_OPAQUE, nullptr, str, 1, nullptr) == 0) return PAINT_FAIL;
			texCoords_[i - 32][0] = static_cast<FLOAT>(x) / width_;
			texCoords_[i - 32][1] = static_cast<FLOAT>(y) / height_;
			texCoords_[i - 32][2] = static_cast<FLOAT>(x + size.cx) / width_;
			texCoords_[i - 32][3] = static_cast<FLOAT>(y + size.cy) / height_;
		}

		x += size.cx + (2 * spacing_);
	}

	return PAINT_SUCCESS;
}

float CGlFont::RenderChar(BYTE ch, float x, float y) const {
	float tx1 = texCoords_[ch - 32][0];
	float ty1 = texCoords_[ch - 32][1];
	float tx2 = texCoords_[ch - 32][2];
	float ty2 = texCoords_[ch - 32][3];
	float w = (tx2 - tx1) * width_;
	float h = (ty2 - ty1) * height_;

	glTexCoord2f(tx1, ty2);
	glVertex2f(x, y);
	glTexCoord2f(tx2, ty2);
	glVertex2f(x + w, y);
	glTexCoord2f(tx2, ty1);
	glVertex2f(x + w, y + h);
	glTexCoord2f(tx1, ty1);
	glVertex2f(x, y + h);

	return w;
}

void CGlFont::Print(int xs, int ys, const char* text, TextAlign align)
{
	float x = static_cast<float>(xs) * static_cast<float>(WindowWidth) ;
	float y = static_cast<float>(WindowHeight) - (static_cast<float>(WindowHeight) * static_cast<float>(ys) / 480.0f);
	y -= fontSize_;

	float textWidth = 0.0f;
	for (const char* ptr = text; *ptr; ++ptr)
	{
		if (*ptr == ' ')
		{
			textWidth += spacing_;
		}
		else if (*ptr == '\n')
		{
			break;
		}
		else
		{
			int ch = *ptr;
			float tx1 = texCoords_[ch - 32][0];
			float tx2 = texCoords_[ch - 32][2];
			textWidth += (tx2 - tx1) * width_;
		}
	}

	if (align == ALIGN_CENTER)
	{
		x -= textWidth / 2.0f;
	}
	else if (align == ALIGN_RIGHT)
	{
		x -= textWidth;
	}

	glBindTexture(GL_TEXTURE_2D, texture_);
	glBegin(GL_QUADS);

	for (const char* ptr = text; *ptr; ++ptr)
	{
		if (*ptr == ' ')
		{
			x += spacing_;
		}
		else if (*ptr == '\n')
		{
			y += fontSize_;
			x = static_cast<float>(xs) * static_cast<float>(WindowWidth) ;
		}
		else
		{
			x += RenderChar(*ptr, x, y);
		}
	}

	glEnd();
}

void CGlFont::Draw(int x, int y, char* text, TextAlign align) {
	glColor4f(1.f, 1.f, 1.f, 1.f);
	Print(x, y, text, align);
}

void CGlFont::DrawColor(int x, int y, COLORREF color, char* text, TextAlign align) {
	float r = static_cast<float>(GetRValue(color));
	float g = static_cast<float>(GetGValue(color));
	float b = static_cast<float>(GetBValue(color));
	glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	Print(x, y, text, align);
}

void CGlFont::DrawFormat(int x, int y, TextAlign align, char* format, ...) {
	char TextBuff[1024] = { 0 };
	va_list va;
	va_start(va, format);
	_vstprintf(TextBuff, format, va);
	va_end(va);
	Draw(x, y, TextBuff, align);
}

void CGlFont::DrawColorFormat(int x, int y, COLORREF color, TextAlign align, char* format, ...) {
	char TextBuff[1024] = { 0 };
	va_list va;
	va_start(va, format);
	_vstprintf(TextBuff, format, va);
	va_end(va);
	DrawColor(x, y, color, TextBuff, align);
}

void CGlFont::GetTextSize(char* text, SIZE& tsize) {
	float size = 0.0f;
	float maxH = 0.0f;
	for (const char* ptr = text; *ptr; ++ptr) {
		if (*ptr == ' ') {
			size += spacing_;
		}
		else if (*ptr == '\n') {
			break;
		}
		else {
			int ch = *ptr;
			float tx1 = texCoords_[ch - 32][0];
			float ty1 = texCoords_[ch - 32][1];
			float tx2 = texCoords_[ch - 32][2];
			float ty2 = texCoords_[ch - 32][3];
			float w = (tx2 - tx1) * width_;
			float h = (ty2 - ty1) * height_;
			size += w;
			if (maxH < h) maxH = h;
		}
	}

	tsize.cx = static_cast<LONG>(size *  static_cast<float>(WindowWidth));
	tsize.cy = static_cast<LONG>(maxH * 480.0f / static_cast<float>(WindowHeight));
}

int CGlFont::GetTextWidth(char* text) {
	float size = 0.0f;

	for (const char* ptr = text; *ptr; ++ptr) {
		if (*ptr == ' ') {
			size += spacing_;
		}
		else if (*ptr == '\n') {
			break;
		}
		else {
			int ch = *ptr;
			float tx1 = texCoords_[ch - 32][0];
			float ty1 = texCoords_[ch - 32][1];
			float tx2 = texCoords_[ch - 32][2];
			float ty2 = texCoords_[ch - 32][3];
			float w = (tx2 - tx1) * width_;
			size += w;
		}
	}

	return static_cast<int>(size  / WindowWidth);
}