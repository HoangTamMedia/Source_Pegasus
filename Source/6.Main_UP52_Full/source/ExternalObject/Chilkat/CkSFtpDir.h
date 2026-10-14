
#ifndef _CkSFtpDir_H
#define _CkSFtpDir_H

#pragma once

#include "CkString.h"
#include "CkObject.h"

class CkSFtpFile;

#pragma pack (push, 8)

class CkSFtpDir  : public CkObject
{
    private:

	void *m_impl;
	bool m_utf8;

	CkSFtpDir(const CkSFtpDir &);
	CkSFtpDir &operator=(const CkSFtpDir &);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

	CkSFtpDir(void *impl);

	CkSFtpDir();
	virtual ~CkSFtpDir();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
    void LastErrorXml(CkString &str);
    void LastErrorHtml(CkString &str);
    void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	int get_NumFilesAndDirs(void);

	void get_OriginalPath(CkString &str);
	const char *originalPath(void);

	CkSFtpFile *GetFileObject(int index);

	bool GetFilename(int index, CkString &outStr);
	const char *getFilename(int index);

};
#pragma pack (pop)

#endif
