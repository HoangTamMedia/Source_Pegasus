
#ifndef _CKMAILBOXES_H
#define _CKMAILBOXES_H

#pragma once

#include "CkString.h"
#include "CkObject.h"

#pragma pack (push, 8)

class CkMailboxes  : public CkObject
{
    private:
	bool m_utf8;

    public:
	CkMailboxes();
	virtual ~CkMailboxes();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool IsMarked(long index);
	bool HasInferiors(long index);
	bool IsSelectable(long index);

	bool GetName(long index, CkString &strName);
	long get_Count(void);

	CkString m_resultString;
	const char *getName(long index);

    private:
	void *m_impl;

	CkMailboxes(const CkMailboxes &) { }
	CkMailboxes &operator=(const CkMailboxes &) { return *this; }
	CkMailboxes(void *impl) : m_impl(impl) { }

    public:
	void *getImpl(void) const { return m_impl; }

};

#pragma pack (pop)

#endif
