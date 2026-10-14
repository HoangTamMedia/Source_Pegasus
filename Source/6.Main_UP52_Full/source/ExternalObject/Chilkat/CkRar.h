
#ifndef _CKRAR_H
#define _CKRAR_H

#pragma once

#include "CkString.h"
class CkByteData;

class CkRarEntry;
class CkStringArray;
#include "CkObject.h"

#pragma pack (push, 8)

class CkRar  : public CkObject
{
    public:
	CkRar();
	virtual ~CkRar();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool Unrar(const char *dirPath);

	bool FastOpen(const char *filename);

	bool Open(const char *filename);

	bool Close(void);

	long get_NumEntries(void);
	CkRarEntry *GetEntryByName(const char *filename);
	CkRarEntry *GetEntryByIndex(long index);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	CkString m_resultString;
        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);

    private:
	void *m_impl;
	bool m_utf8;

	CkRar(const CkRar &);
	CkRar &operator=(const CkRar &);
	CkRar(void *impl);

    public:
	void *getImpl(void) const;

};
#pragma pack (pop)

#endif
