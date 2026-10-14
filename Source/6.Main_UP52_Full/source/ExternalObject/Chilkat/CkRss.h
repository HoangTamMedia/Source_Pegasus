
#ifndef _CKRss_H
#define _CKRss_H

#pragma once

#include "CkString.h"
class CkByteData;

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

#include "CkObject.h"

class CkRssProgress;

#pragma pack (push, 8)

class CkRss  : public CkObject
{
    private:
	CkRssProgress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkRss(const CkRss &) { }
	CkRss &operator=(const CkRss &) { return *this; }
	CkRss(void *impl) : m_impl(impl),m_callback(0) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

	CkRss();
	virtual ~CkRss();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	CkRssProgress *get_EventCallbackObject(void) const;
	void put_EventCallbackObject(CkRssProgress *progress);

	bool SaveLastError(const char *filename);
	void LastErrorXml(CkString &str);
	void LastErrorHtml(CkString &str);
	void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	int get_NumChannels(void);

	int get_NumItems(void);

	CkRss *AddNewChannel(void);

	CkRss *AddNewImage(void);

	CkRss *AddNewItem(void);

	bool DownloadRss(const char *url);

	bool GetAttr(const char *tag, const char *attrName, CkString &outStr);
	const char *getAttr(const char *tag, const char *attrName);

	CkRss *GetChannel(int index);

	int GetCount(const char *tag);

	bool GetDate(const char *tag, SYSTEMTIME &sysTime);

	CkRss *GetImage(void);

	int GetInt(const char *tag);

	CkRss *GetItem(int index);

	bool GetString(const char *tag, CkString &outStr);
	const char *getString(const char *tag);

	bool LoadRssFile(const char *filename);

	bool LoadRssString(const char *rssString);

	bool MGetAttr(const char *tag, int index, const char *attrName, CkString &outStr);
	const char *mGetAttr(const char *tag, int index, const char *attrName);

	bool MGetString(const char *tag, int index, CkString &outStr);
	const char *mGetString(const char *tag, int index);

	bool MSetAttr(const char *tag, int index, const char *attrName, const char *value);

	bool MSetString(const char *tag, int index, const char *value);

	void NewRss(void);

	void Remove(const char *tag);

	void SetAttr(const char *tag, const char *attrName, const char *value);

	void SetDate(const char *tag, SYSTEMTIME &d);

	void SetDateNow(const char *tag);

	void SetInt(const char *tag, int value);

	void SetString(const char *tag, const char *value);

	bool ToXmlString(CkString &outStr);
	const char *toXmlString(void);

};
#pragma pack (pop)

#endif
