
#ifndef _CKXMP_H
#define _CKXMP_H

#pragma once

#include "CkObject.h"
#include "CkString.h"

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

class CkXml;
class CkStringArray;
class CkByteData;

#pragma pack (push, 8)

class CkXmp : public CkObject
{
    private:
	void *m_impl;

	bool m_utf8;

	bool m_structInnerDescrip;

	CkString m_errorInfo;

	CkXml **m_xml;

	CkXmp(const CkXmp &) { }
	CkXmp &operator=(const CkXmp &) { return *this; }

    public:

	CkXmp();
	virtual ~CkXmp();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
	void LastErrorXml(CkString &str);
	void LastErrorHtml(CkString &str);
	void LastErrorText(CkString &str);

	bool UnlockComponent(const char *unlockCode);

	void get_Version(CkString &strOut);

	long get_NumEmbedded(void);

	bool LoadAppFile(const char *filename);
	bool SaveAppFile(const char *filename);

	bool LoadFromBuffer(CkByteData &byteData, const char *ext);
	bool SaveToBuffer(CkByteData &byteData);

	CkXml *GetEmbedded(int index);

	bool GetSimpleStr(CkXml &xml, const char *propName, CkString &strOut);
	int GetSimpleInt(CkXml &xml, const char *propName);
	bool GetSimpleDate(CkXml &xml, const char *propName, SYSTEMTIME &sysTime);

	bool AddSimpleStr(CkXml &xml, const char *propName, const char *propVal);
	bool AddSimpleInt(CkXml &xml, const char *propName, int propVal);
	bool AddSimpleDate(CkXml &xml, const char *propName, SYSTEMTIME &sysTime);

	void AddNsMapping(const char *ns, const char *uri);
	void RemoveNsMapping(const char *ns);

	void NewXmp(CkXml &xmlOut);

	bool StringToDate(const char *str, SYSTEMTIME &sysTime);
	bool DateToString(SYSTEMTIME &sysTime, CkString &strOut);

	bool RemoveSimple(CkXml &xml, const char *propName);
	bool Append(CkXml &xml);

	void RemoveAllEmbedded(void);
	void RemoveEmbedded(int index);

	bool get_StructInnerDescrip(void);
	void put_StructInnerDescrip(bool val);

	bool GetArray(CkXml &xml, const char *propName, CkStringArray &array);
	bool RemoveArray(CkXml &xml, const char *propName);
	bool AddArray(CkXml &xml, const char *arrType, const char *propName, CkStringArray &values);

	bool GetStructValue(CkXml &xml, const char *structName, const char *propName, CkString &strOut);
	bool GetStructPropNames(CkXml &xml, const char *structName, CkStringArray &array);

	bool RemoveStruct(CkXml &xml, const char *structName);
	bool RemoveStructProp(CkXml &xml, const char *structName, const char *propName);
	bool AddStructProp(CkXml &xml, const char *structName, const char *propName, const char *propVal);

	CkString m_resultString;
        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);
	const char *getSimpleStr(CkXml &xml, const char *propName);
	const char *getStructValue(CkXml &xml, const char *structName, const char *propName);

	const char *simpleStr(CkXml &xml, const char *propName);
	const char *structValue(CkXml &xml, const char *structName, const char *propName);

	const char *version(void);

	const char *dateToString(SYSTEMTIME &sysTime);

};

#pragma pack (pop)

#endif
