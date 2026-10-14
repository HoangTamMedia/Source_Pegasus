
#ifndef _CkSFtpFile_H
#define _CkSFtpFile_H

#pragma once

#ifndef WIN32
#include "int64.h"
#endif

#include "CkString.h"
#include "CkObject.h"

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

#pragma pack (push, 8)

class CkSFtpFile  : public CkObject
{
    private:

	void *m_impl;
	bool m_utf8;

	CkSFtpFile(const CkSFtpFile &);
	CkSFtpFile &operator=(const CkSFtpFile &);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

	CkSFtpFile(void *impl);

	CkSFtpFile();
	virtual ~CkSFtpFile();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
    void LastErrorXml(CkString &str);
    void LastErrorHtml(CkString &str);
    void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	void get_Filename(CkString &str);
	const char *filename(void);

	void get_Owner(CkString &str);
	const char *owner(void);

	void get_Group(CkString &str);
	const char *group(void);

	void get_FileType(CkString &str);
	const char *fileType(void);

	bool get_IsRegular(void);

	bool get_IsDirectory(void);

	bool get_IsSymLink(void);

	bool get_IsReadOnly(void);

	bool get_IsSystem(void);

	bool get_IsHidden(void);

	bool get_IsCaseInsensitive(void);

	bool get_IsArchive(void);

	bool get_IsEncrypted(void);

	bool get_IsCompressed(void);

	bool get_IsSparse(void);

	bool get_IsAppendOnly(void);

	bool get_IsImmutable(void);

	bool get_IsSync(void);

	int get_Permissions(void);

	int get_Size32(void);

	__int64 get_Size64(void);

	void get_LastModifiedTime(SYSTEMTIME &sysTime);
	void get_LastAccessTime(SYSTEMTIME &sysTime);
	void get_CreateTime(SYSTEMTIME &sysTime);

	void get_SizeStr(CkString &str);
	const char *sizeStr(void);

	int get_Uid(void);

	int get_Gid(void);

};
#pragma pack (pop)

#endif
