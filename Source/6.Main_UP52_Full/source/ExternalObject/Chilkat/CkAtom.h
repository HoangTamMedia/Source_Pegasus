
#ifndef _CKAtom_H
#define _CKAtom_H

#pragma once

#include "CkString.h"
class CkByteData;

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

#include "CkObject.h"

class CkAtomProgress;

#pragma pack (push, 8)

class CkAtom  : public CkObject
{
    private:
	CkAtomProgress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkAtom(const CkAtom &) { }
	CkAtom &operator=(const CkAtom &) { return *this; }
	CkAtom(void *impl) : m_impl(impl),m_callback(0) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

	CkAtom();
	virtual ~CkAtom();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	CkAtomProgress *get_EventCallbackObject(void) const;
	void put_EventCallbackObject(CkAtomProgress *progress);

	bool SaveLastError(const char *filename);
	void LastErrorXml(CkString &str);
	void LastErrorHtml(CkString &str);
	void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	int get_NumEntries(void);

	int AddElement(const char *tag, const char *value);

	int AddElementDate(const char *tag, SYSTEMTIME &dateTime);

	int AddElementHtml(const char *tag, const char *htmlStr);

	int AddElementXHtml(const char *tag, const char *xmlStr);

	int AddElementXml(const char *tag, const char *xmlStr);

	void AddEntry(const char *xmlStr);

	void AddLink(const char *rel, const char *href, const char *title, const char *typ);

	void AddPerson(const char *tag, const char *name, const char *uri, const char *email);

	void DeleteElement(const char *tag, int index);

	void DeleteElementAttr(const char *tag, int index, const char *attrName);

	void DeletePerson(const char *tag, int index);

	bool DownloadAtom(const char *url);

	bool GetElement(const char *tag, int index, CkString &outStr);
	const char *getElement(const char *tag, int index);

	bool GetElementAttr(const char *tag, int index, const char *attrName, CkString &outStr);
	const char *getElementAttr(const char *tag, int index, const char *attrName);

	int GetElementCount(const char *tag);

	bool GetElementDate(const char *tag, int index, SYSTEMTIME &sysTime);

	CkAtom *GetEntry(int index);

	bool GetLinkHref(const char *relName, CkString &outStr);
	const char *getLinkHref(const char *relName);

	bool GetPersonInfo(const char *tag, int index, const char *tag2, CkString &outStr);
	const char *getPersonInfo(const char *tag, int index, const char *tag2);

	bool GetTopAttr(const char *attrName, CkString &outStr);
	const char *getTopAttr(const char *attrName);

	bool HasElement(const char *tag);

	bool LoadXml(const char *xmlStr);

	void NewEntry(void);

	void NewFeed(void);

	void SetElementAttr(const char *tag, int index, const char *attrName, const char *attrValue);

	void SetTopAttr(const char *attrName, const char *value);

	bool ToXmlString(CkString &outStr);
	const char *toXmlString(void);

	void UpdateElement(const char *tag, int index, const char *value);

	void UpdateElementDate(const char *tag, int index, SYSTEMTIME &dateTime);

	void UpdateElementHtml(const char *tag, int index, const char *htmlStr);

	void UpdateElementXHtml(const char *tag, int index, const char *xmlStr);

	void UpdateElementXml(const char *tag, int index, const char *xmlStr);

	void UpdatePerson(const char *tag, int index, const char *name, const char *uri, const char *email);

};
#pragma pack (pop)

#endif
