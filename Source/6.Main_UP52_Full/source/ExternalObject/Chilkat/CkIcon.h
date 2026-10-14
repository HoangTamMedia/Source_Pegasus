
#ifndef _CkIcon_H
#define _CkIcon_H

#pragma once

#include "CkString.h"
class CkByteData;

#include "CkObject.h"

#pragma pack (push, 8)

class CkIcon  : public CkObject
{
    private:

	void *m_impl;
	bool m_utf8;

	CkIcon(const CkIcon &) { }
	CkIcon &operator=(const CkIcon &) { return *this; }
	CkIcon(void *impl) : m_impl(impl) { }

    public:
	void *getImpl(void) const { return m_impl; }

	CkIcon();
	virtual ~CkIcon();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
	void LastErrorXml(CkString &str);
	void LastErrorHtml(CkString &str);
	void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	CkString m_resultString;

};
#pragma pack (pop)

#endif
