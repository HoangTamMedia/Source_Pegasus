
#ifndef _CKDH_H
#define _CKDH_H

#pragma once

#include "CkString.h"
class CkByteData;

#include "CkObject.h"

class CkDhProgress;

#pragma pack (push, 8)

class CkDh  : public CkObject
{
    private:
	CkDhProgress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkDh(const CkDh &) { }
	CkDh &operator=(const CkDh &) { return *this; }
	CkDh(void *impl) : m_impl(impl),m_callback(0) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

	CkDh();
	virtual ~CkDh();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	CkDhProgress *get_EventCallbackObject(void) const;
	void put_EventCallbackObject(CkDhProgress *progress);

	bool SaveLastError(const char *filename);
	void LastErrorXml(CkString &str);
	void LastErrorHtml(CkString &str);
	void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);
	void UseKnownPrime(int index);

	int get_G(void);

	void get_P(CkString &str);
	const char *p(void);

	bool SetPG(const char *p, int g);

	bool CreateE(int numBits, CkString &outStr);
	const char *createE(int numBits);

	bool FindK(const char *e, CkString &outStr);
	const char *findK(const char *e);

	bool GenPG(int numBits, int g);

	void get_Version(CkString &str);
	const char *version(void);

	bool UnlockComponent(const char *unlockCode);

};
#pragma pack (pop)

#endif
