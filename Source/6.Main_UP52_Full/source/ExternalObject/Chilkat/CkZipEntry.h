
#ifndef _CKZIPENTRY__H
#define _CKZIPENTRY__H

#pragma once

#ifndef WIN32
#include "int64.h"
#endif

#include "CkString.h"
class CkByteData;
class CkZip;
#include "CkObject.h"

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

#undef Copy

#pragma pack (push, 8)

class CkZipEntry  : public CkObject
{
    public:

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	void get_FileName(CkString &str);
	void put_FileName(const char *pStr);

	unsigned long get_UncompressedLength(void);

	long get_CompressionLevel(void);
	void put_CompressionLevel(long newVal);

	long get_CompressionMethod(void);
	void put_CompressionMethod(long newVal);

	unsigned long get_CompressedLength(void);

	void get_Comment(CkString &str);
	void put_Comment(const char *pStr);

	long get_EntryType(void);

	void get_FileDateTime(SYSTEMTIME &sysTime);
	void put_FileDateTime(const SYSTEMTIME &sysTime);

	bool get_IsDirectory(void);

	long get_EntryID(void);

	bool Extract(const char *dirPath);

	bool ExtractInto(const char *dirPath);

	bool InflateToString(bool addCR, CkString &str);

	bool InflateToString2(CkString &str);

	bool Inflate(CkByteData &bdata);

#if defined(WIN32) | defined(WINCE)
	bool InflateToHandle(HANDLE handle);
#endif

	bool ReplaceData(const CkByteData &bdata);
	bool ReplaceData(const CkString &sdata);

	bool AppendData(const CkByteData &bdata);
	bool AppendData(const CkString &sdata);

	bool CopyToBase64(CkString &str);

	bool CopyToHex(CkString &str);

	bool Copy(CkByteData &bdata);

	CkZipEntry *NextEntry(void);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	CkString m_resultString;
        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);
	const char *fileName(void);
	const char *comment(void);
	const char *inflateToString(bool addCR);
	const char *inflateToString2(void);
	const char *copyToBase64(void);
	const char *copyToHex(void);

	int get_Crc(void);

	__int64 get_CompressedLength64(void);

	__int64 get_UncompressedLength64(void);

    private:
	void *m_impl;
	bool m_utf8;

	CkZipEntry(const CkZipEntry &) { }
	CkZipEntry &operator=(const CkZipEntry &) { return *this; }
	CkZipEntry(void *impl);

    public:
	void *getImpl(void) const { return m_impl; }
	void inject(void *impl);

	CkZipEntry();

	virtual ~CkZipEntry();

};
#pragma pack (pop)

#endif
