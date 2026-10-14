
#ifndef _CKSTRING_H
#define _CKSTRING_H

#pragma once

#include "CkObject.h"

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

class CkStringArray;

#pragma pack (push, 8)

class CkString : public CkObject
{
    public:
	CkString();
	~CkString();

	CkString(const CkString &);
	CkString &operator=(const CkString &);

	CkString &operator=(const char *);
	CkString &operator=(int);
	CkString &operator=(bool);
	CkString &operator=(char);

       operator const char *();
       operator const wchar_t *();

        void appendHexData(const char *data, int dataLen);

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

        bool loadFile(const char *fileName, const char *charset);

	char charAt(int idx) const;
	wchar_t charAtU(int idx) const;

        int intValue(void) const;
        double doubleValue(void) const;

	CkString *clone(void) const;
	void setStr(CkString &s);
	bool endsWith(const char *s) const;
	bool endsWithStr(CkString &s) const;

	bool beginsWithStr(CkString &s) const;
	int indexOf(const char *s) const;
	int indexOfStr(CkString &s) const;
        int replaceAll(CkString &str, CkString &replacement);
        bool replaceFirst(CkString &str, CkString &replacement);
	CkString *substring(int startCharIdx, int numChars) const;
	bool matchesStr(CkString &str) const;
	bool matches(const char *s) const;
	bool matchesNoCase(const char *s) const;
	CkString *getChar(int idx) const;
        int removeAll(CkString &str);
        bool removeFirst(CkString &str);
	void chopAtStr(CkString &str);
	void urlDecode(const char *charset);
	void urlEncode(const char *charset);
	void base64Decode(const char *charset);
	void base64Encode(const char *charset);
	void qpDecode(const char *charset);
	void qpEncode(const char *charset);
	void hexDecode(const char *charset);
	void hexEncode(const char *charset);
	void entityDecode(void);
	void entityEncode(void);
	void appendUtf8(const char *s);
	void appendAnsi(const char *s);
	void appendCurrentDateRfc822(void);

	void appendDateRfc822(SYSTEMTIME &sysTime);

	void clear(void);
	void prepend(const char *s);
	void appendInt(int n);
	void append(const char *s);
        void appendChar(char c);
	void appendN(const char *s, int numBytes);
        void appendStr(const CkString &str);
	void appendEnc(const char *s, const char *charEncoding);
	void appendRandom(int numBytes, const char *encoding);

        void appendHexData(const unsigned char *data, int dataLen);

        void setString(const char *s);
        void setStringAnsi(const char *s);
        void setStringUtf8(const char *s);

	void setStringU(const wchar_t *unicode);
	void appendU(const wchar_t *unicode);
	void appendNU(const wchar_t *unicode, int numChars);
	const wchar_t *getUnicode(void) const;

	const char *getAnsi(void) const;

	const char *getUtf8(void) const;

	const char *getEnc(const char *encoding) const;

	int compareStr(const CkString &str) const;

	const char *getString(void) const;

	int getSizeUtf8(void) const;
	int getSizeAnsi(void) const;
	int getSizeUnicode(void) const;
	int getNumChars(void) const;

        void trim(void);
        void trim2(void);
	void trimInsideSpaces(void);

        int replaceAllOccurances(const char *pattern, const char *replacement);
        bool replaceFirstOccurance(const char *pattern, const char *replacement);

        void toCRLF(void);
        void toLF(void);

        void eliminateChar(char ansiChar, int startIndex);

        char lastChar(void);

	int countCharOccurances(char ch);

        void shorten(int n);

        void toLowerCase(void);
        void toUpperCase(void);

        void encodeXMLSpecial(void);
        void decodeXMLSpecial(void);

        bool containsSubstring(const char *pattern) const;
        bool containsSubstringNoCase(const char *pattern) const;

#ifdef WIN32
        void appendLastWindowsError(void);
#endif

        bool equals(const char *s) const;
        bool equalsStr(CkString &s) const;
        bool equalsIgnoreCase(const char *s) const;
        bool equalsIgnoreCaseStr(CkString &s) const;

        void removeChunk(int charStartPos, int numChars);

        void removeCharOccurances(char c);

	void replaceChar(char c1, char c2);

	void chopAtFirstChar(char c1);

	void obfuscate(void);
	void unobfuscate(void);

	bool saveToFile(const char *filename, const char *charset);

	CkStringArray *split(char splitChar, bool exceptDoubleQuoted, bool exceptEscaped, bool keepEmpty) const;
	CkStringArray *split2(const char *splitCharSet, bool exceptDoubleQuoted, bool exceptEscaped, bool keepEmpty) const;
	CkStringArray *tokenize(char *punctuation) const;

	CkStringArray *splitAtWS(void) const;

	bool beginsWith(const char *sSubstr) const;

	CkString *getDelimited(const char *beginSearchAfter,
				   const char *startDelim, const char *endDelim);

	void minimizeMemory(void);

	bool isInternalPtr(const char *str);

    private:

	void *m_x;
	bool m_utf8;
	void *m_sb;

    public:

	void *getImplX(void) const { return m_x; }

};
#pragma pack (pop)

#endif
