
#ifndef _CkHtmlToText_H
#define _CkHtmlToText_H

#pragma once

class CkByteData;
#include "CkString.h"
#include "CkObject.h"

#pragma warning( disable : 4068 )
#pragma unmanaged

#pragma pack (push, 8)

class CkHtmlToText  : public CkObject
{
    public:
	CkHtmlToText();
	virtual ~CkHtmlToText();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool UnlockComponent(const char *code);
	bool IsUnlocked(void) const;

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);

	const char *readFileToString(const char *filename, const char *srcCharset);
	bool ReadFileToString(const char *filename, const char *srcCharset, CkString &str);
	bool WriteStringToFile(const char *str, const char *filename, const char *charset);

	bool ToText(const char *html, CkString &outStr);
	const char *toText(const char *html);

	int get_RightMargin(void);
	void put_RightMargin(int newVal);

    private:
	void *m_impl;
	bool m_utf8;

	CkHtmlToText(const CkHtmlToText &) { }
	CkHtmlToText &operator=(const CkHtmlToText &) { return *this; }

	CkString m_resultString;

};
#pragma pack (pop)

#endif
