
#ifndef _CKZIPCRC_H
#define _CKZIPCRC_H

#pragma once

class CkByteData;
#include "CkString.h"
#include "CkObject.h"

#pragma warning( disable : 4068 )
#pragma unmanaged

#pragma pack (push, 8)

class CkZipCrc  : public CkObject
{
    public:
	CkZipCrc();
	virtual ~CkZipCrc();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	void ToHex(unsigned long crc, CkString &strHex) const;
	unsigned long FileCrc(const char *filename);
	unsigned long CalculateCrc(CkByteData &byteData);
	void MoreData(CkByteData &byteData);
	unsigned long EndStream(void);
	void BeginStream(void);

	CkString m_resultString;
	const char *toHex(unsigned long crc);

    private:
	void *m_impl;
	bool m_utf8;

	CkZipCrc(const CkZipCrc &) { }
	CkZipCrc &operator=(const CkZipCrc &) { return *this; }

    public:
	void *getImpl(void) const { return m_impl; }

	CkZipCrc(void *impl);

};
#pragma pack (pop)

#endif
