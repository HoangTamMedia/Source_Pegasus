
#ifndef _CkDirTree_H
#define _CkDirTree_H

#pragma once

#include "CkString.h"
#include "CkObject.h"

#ifndef WIN32
#include "int64.h"
#endif

#pragma pack (push, 8)

class CkDirTree  : public CkObject
{
    private:

	void *m_impl;
	bool m_utf8;

	CkDirTree(const CkDirTree &);
	CkDirTree &operator=(const CkDirTree &);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

	CkDirTree(void *impl);

	CkDirTree();
	virtual ~CkDirTree();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
    void LastErrorXml(CkString &str);
    void LastErrorHtml(CkString &str);
    void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	void get_BaseDir(CkString &str);
	const char *baseDir(void);
	void put_BaseDir(const char *newVal);

	bool get_Recurse(void);
	void put_Recurse(bool newVal);

	bool get_DoneIterating(void);

	bool BeginIterate(void);

	bool AdvancePosition(void);

	bool get_IsDirectory(void);

	void get_RelativePath(CkString &str);
	const char *relativePath(void);

	void get_FullPath(CkString &str);
	const char *fullPath(void);

	void get_FullUncPath(CkString &str);
	const char *fullUncPath(void);

	int get_FileSize32(void);

	__int64 get_FileSize64(void);

};
#pragma pack (pop)

#endif
