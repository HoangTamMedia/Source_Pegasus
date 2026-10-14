
#ifdef WIN32

#ifndef _CKSERVICE_H
#define _CKSERVICE_H

#pragma once

#include "CkString.h"
class CkByteData;

#include "CkObject.h"

#pragma pack (push, 8)

class CkService  : public CkObject
{
    private:

	void *m_impl;
	bool m_utf8;

	CkService(const CkService &) { }
	CkService &operator=(const CkService &) { return *this; }
	CkService(void *impl) : m_impl(impl) { }

    public:
	void *getImpl(void) const { return m_impl; }

	CkService();
	virtual ~CkService();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
	void LastErrorXml(CkString &str);
	void LastErrorHtml(CkString &str);
	void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	bool Install(void);

	bool Uninstall(void);

	bool Start(void);

	bool Stop(void);

	void get_ServiceName(CkString &str);
	const char *serviceName(void);
	void put_ServiceName(const char *newVal);

	void get_DisplayName(CkString &str);
	const char *displayName(void);
	void put_DisplayName(const char *newVal);

	void get_ExeFilename(CkString &str);
	const char *exeFilename(void);
	void put_ExeFilename(const char *newVal);

	bool get_AutoStart(void);
	void put_AutoStart(bool newVal);

	bool SetAutoStart(void);

	bool SetDemandStart(void);

	bool Disable(void);

	int IsAutoStart(void);

	int IsDemandStart(void);

	int IsDisabled(void);

	int IsInstalled(void);

	int IsRunning(void);

	CkString m_resultString;

};
#pragma pack (pop)

#endif

#endif
