
#ifndef _CKRARENTRY__H
#define _CKRARENTRY__H

#pragma once

#include "CkString.h"
class CkByteData;
class CkRar;
#include "CkObject.h"

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

#undef Copy

#pragma pack (push, 8)

class CkRarEntry  : public CkObject
{
    public:

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	void get_Filename(CkString &str);

	unsigned long get_UncompressedSize(void);

	unsigned long get_CompressedSize(void);

	void get_LastModified(SYSTEMTIME &sysTime);

	bool get_IsDirectory(void);

	bool get_IsReadOnly(void);

	bool Unrar(const char *dirPath);

	unsigned long get_Crc(void);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	CkString m_resultString;
        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);
	const char *filename(void);

    private:
	void *m_impl;
	bool m_utf8;

	CkRarEntry(const CkRarEntry &);
	CkRarEntry &operator=(const CkRarEntry &);

    public:
	void *getImpl(void) const;

	CkRarEntry();
	CkRarEntry(void *impl);

	virtual ~CkRarEntry();

};
#pragma pack (pop)

#endif
