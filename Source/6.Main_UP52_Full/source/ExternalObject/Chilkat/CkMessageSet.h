
#ifndef _CKMESSAGESET_H
#define _CKMESSAGESET_H

#pragma once

class CkByteData;
#include "CkString.h"
#include "CkObject.h"

#pragma pack (push, 8)

class CkMessageSet  : public CkObject
{
    public:
	CkMessageSet();
	virtual ~CkMessageSet();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	void ToString(CkString &str);
	void ToCompactString(CkString &str);

	bool ContainsId(long id);

	void RemoveId(long id);

	void InsertId(long id);

	long GetId(long index);

	long get_Count(void);

	void put_HasUids(bool value);
	bool get_HasUids(void) const;

	CkString m_resultString;
	const char *toString(void);
	const char *toCompactString(void);

	bool FromCompactString(const char *str);

    private:
	void *m_impl;

	bool m_utf8;

	CkMessageSet(const CkMessageSet &);
	CkMessageSet &operator=(const CkMessageSet &);
	CkMessageSet(void *impl);

    public:
	void *getImpl(void) const { return m_impl; }
	void inject(void *impl);

};
#pragma pack (pop)

#endif
