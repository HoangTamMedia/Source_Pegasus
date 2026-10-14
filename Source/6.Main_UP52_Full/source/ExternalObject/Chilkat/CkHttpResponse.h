
#ifndef _CkHttpResponse_H
#define _CkHttpResponse_H

#pragma once

#ifndef WIN32
#include "int64.h"
#endif

#include "CkString.h"
class CkByteData;

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

#include "CkObject.h"

#pragma pack (push, 8)

class CkHttpResponse  : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkHttpResponse(const CkHttpResponse &) { }
	CkHttpResponse &operator=(const CkHttpResponse &) { return *this; }
	CkHttpResponse(void *impl) : m_impl(impl) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }
	void inject(void *impl);

	CkHttpResponse();
	virtual ~CkHttpResponse();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	void get_Header(CkString &str);
	void get_Body(CkByteData &data);
	void get_BodyStr(CkString &str);
	void get_StatusLine(CkString &str);
	int get_StatusCode(void);
	void get_Charset(CkString &str);
	void get_Domain(CkString &str);
	unsigned long get_ContentLength(void);
	__int64 get_ContentLength64(void);

	void get_Date(SYSTEMTIME &sysTime);

	bool GetHeaderField(const char *fieldName, CkString &str);
	bool GetHeaderFieldAttr(const char *fieldName, const char *attrName, CkString &str);

	int get_NumHeaderFields(void);
	bool GetHeaderName(int index, CkString &str);
	bool GetHeaderValue(int index, CkString &str);

	int get_NumCookies(void);
	bool GetCookieDomain(int index, CkString &str);
	bool GetCookiePath(int index, CkString &str);
	bool GetCookieExpiresStr(int index, CkString &str);
	bool GetCookieExpires(int index, SYSTEMTIME &sysTime);
	bool GetCookieName(int index, CkString &str);
	bool GetCookieValue(int index, CkString &str);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);

	const char *header(void);
	const char *bodyStr(void);
	const char *statusLine(void);
	const char *charset(void);
	const char *domain(void);
	const char *getHeaderField(const char *fieldName);
	const char *getHeaderFieldAttr(const char *fieldName, const char *attrName);
	const char *getHeaderName(int index);
	const char *getHeaderValue(int index);
	const char *getCookieDomain(int index);
	const char *getCookiePath(int index);
	const char *getCookieExpiresStr(int index);
	const char *getCookieName(int index);
	const char *getCookieValue(int index);

	void get_BodyQP(CkString &str);
	const char *bodyQP(void);

};
#pragma pack (pop)

#endif
