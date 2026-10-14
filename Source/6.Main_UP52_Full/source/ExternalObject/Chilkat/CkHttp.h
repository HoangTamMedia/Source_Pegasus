
#ifndef _CKHTTP_H
#define _CKHTTP_H

#pragma once

#include "CkString.h"
class CkByteData;

#include "CkObject.h"

class CkHttpRequest;
class CkHttpResponse;
class CkHttpProgress;
class CkCert;

#pragma pack (push, 8)

class CkHttp  : public CkObject
{
    private:
	CkHttpProgress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkHttp(const CkHttp &) { }
	CkHttp &operator=(const CkHttp &) { return *this; }
	CkHttp(void *impl) : m_impl(impl),m_callback(0) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

	CkHttp();
	virtual ~CkHttp();

	CkHttpProgress *get_EventCallbackObject(void) const;
	void put_EventCallbackObject(CkHttpProgress *progress);

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool UnlockComponent(const char *unlockCode);
	bool IsUnlocked(void) const;

	void get_Version(CkString &str);

	long get_MaxConnections(void);
	void put_MaxConnections(long n);

	void put_CookieDir(const char *dir);
	void get_CookieDir(CkString &str);

	void put_SaveCookies(bool b);
	bool get_SaveCookies(void);

	void put_SendCookies(bool b);
	bool get_SendCookies(void);

	void put_UseIEProxy(bool b);
	bool get_UseIEProxy(void);

	bool Download(const char *url, const char *filename);
	bool ResumeDownload(const char *url, const char *filename);

	bool QuickGet(const char *url, CkByteData &data);

	bool QuickGetStr(const char *url, CkString &str);

	bool SetSslClientCert(CkCert &cert);

	CkHttpResponse *SynchronousRequest(const char *domain, long port, bool ssl, const CkHttpRequest &req);

	CkHttpResponse *PostMime(const char *url, const char *mime);

	void get_LastHeader(CkString &str);
	const char *lastHeader(void);

	void put_Login(const char *v);
	void get_Login(CkString &str);

	void put_Password(const char *v);
	void get_Password(CkString &str);

	void put_NtlmAuth(bool b);
	bool get_NtlmAuth(void);

	long get_ProxyPort(void);
	void put_ProxyPort(long n);

	void put_ProxyDomain(const char *v);
	void get_ProxyDomain(CkString &str);

	void get_ProxyLogin(CkString &str);
	void put_ProxyLogin(const char *newVal);

	void get_ProxyPassword(CkString &str);
	void put_ProxyPassword(const char *newVal);

	const char *proxyLogin(void);
	const char *proxyPassword(void);
	const char *proxyDomain(void);

	long get_HeartbeatMs(void);
	void put_HeartbeatMs(long newVal);

	void SetRequestHeader(const char *name, const char *value);
	bool GetRequestHeader(const char *name, CkString &str);
	bool HasRequestHeader(const char *name);
	void RemoveRequestHeader(const char *name);

	void put_UserAgent(const char *v);
	void get_UserAgent(CkString &str);

	void put_Referer(const char *v);
	void get_Referer(CkString &str);

	void put_Accept(const char *v);
	void get_Accept(CkString &str);

	void put_AcceptCharset(const char *v);
	void get_AcceptCharset(CkString &str);

	void put_AcceptLanguage(const char *v);
	void get_AcceptLanguage(CkString &str);

	void put_Connection(const char *v);
	void get_Connection(CkString &str);

	long get_MaxUrlLen(void);
	void put_MaxUrlLen(long n);

	unsigned long get_MaxResponseSize(void);
	void put_MaxResponseSize(unsigned long n);

	void put_MimicIE(bool b);
	bool get_MimicIE(void);
	void put_MimicFireFox(bool b);
	bool get_MimicFireFox(void);

	void put_AutoAddHostHeader(bool b);
	bool get_AutoAddHostHeader(void);

	long get_ConnectTimeout(void);
	void put_ConnectTimeout(long numSeconds);

	long get_ReadTimeout(void);
	void put_ReadTimeout(long numSeconds);

	void GetDomain(const char *url, CkString &str);

	bool get_WasRedirected(void);
	void get_FinalRedirectUrl(CkString &str);

	void put_FollowRedirects(bool b);
	bool get_FollowRedirects(void);

	void put_NumCacheLevels(long v);
	long get_NumCacheLevels(void);

	long get_NumCacheRoots(void);
	bool GetCacheRoot(int index, CkString &str);
	void AddCacheRoot(const char *dir);

	bool get_UpdateCache(void);
	void put_UpdateCache(bool b);

	bool get_FetchFromCache(void);
	void put_FetchFromCache(bool b);

	bool get_IgnoreNoCache(void);
	void put_IgnoreNoCache(bool b);

	bool get_IgnoreMustRevalidate(void);
	void put_IgnoreMustRevalidate(bool b);

 	long get_DefaultFreshPeriod(void);
	void put_DefaultFreshPeriod(long numMinutes);

	long get_FreshnessAlgorithm(void);
	void put_FreshnessAlgorithm(long v);

	long get_LMFactor(void);
	void put_LMFactor(long v);

	long get_MaxFreshPeriod(void);
	void put_MaxFreshPeriod(long numMinutes);

	long get_MinFreshPeriod(void);
	void put_MinFreshPeriod(long numMinutes);

	void GetUrlPath(const char *url, CkString &str);

	long get_LastStatus(void);
	void get_LastContentType(CkString &strContentType);
	void get_LastResponseHeader(CkString &strHeader);

	void get_LastModDate(CkString &str);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);
	const char *version(void);
	const char *cookieDir(void);
	const char *quickGetStr(const char *url);
	const char *password(void);
	const char *login(void);
	const char *getRequestHeader(const char *name);
	const char *userAgent(void);
	const char *referer(void);
	const char *ck_accept(void);
	const char *acceptCharset(void);
	const char *acceptLanguage(void);
	const char *connection(void);
	const char *getDomain(const char *url);
	const char *finalRedirectUrl(void);
	const char *getCacheRoot(int index);
	const char *getUrlPath(const char *url);
	const char *lastContentType(void);
	const char *lastResponseHeader(void);
	const char *lastModDate(void);

	bool RenderGet(const char *url, CkString &outStr);
	const char *renderGet(const char *url);

	bool GetCookieXml(const char *domain, CkString &outStr);
	const char *getCookieXml(const char *domain);

	void get_RequiredContentType(CkString &str);
	const char *requiredContentType(void);
	void put_RequiredContentType(const char *newVal);

	bool DownloadAppend(const char *url, const char *filename);

	bool UrlDecode(const char *str, CkString &outStr);
	const char *urlDecode(const char *str);

	bool UrlEncode(const char *str, CkString &outStr);
	const char *urlEncode(const char *str);

	void get_SslProtocol(CkString &str);
	const char *sslProtocol(void);
	void put_SslProtocol(const char *newVal);

	CkHttpResponse *GetHead(const char *url);

	bool XmlRpc(const char *urlEndpoint, const char *xmlIn, CkString &outStr);
	const char *xmlRpc(const char *urlEndpoint, const char *xmlIn);

	void GenTimeStamp(CkString &outStr);
	const char *genTimeStamp(void);

	bool XmlRpcPut(const char *urlEndpoint, const char *xmlIn, CkString &outStr);
	const char *xmlRpcPut(const char *urlEndpoint, const char *xmlIn);

	bool QuickPutStr(const char *url, CkString &outStr);
	const char *quickPutStr(const char *url);

	bool QuickDeleteStr(const char *url, CkString &outStr);
	const char *quickDeleteStr(const char *url);

	bool PutText(const char *url, const char *textData, const char *charset, const char *contentType, bool md5, bool gzip, CkString &outStr);
	const char *putText(const char *url, const char *textData, const char *charset, const char *contentType, bool md5, bool gzip);

	bool PutBinary(const char *url, CkByteData &byteData, const char *contentType, bool md5, bool gzip, CkString &outStr);
	const char *putBinary(const char *url, CkByteData &byteData, const char *contentType, bool md5, bool gzip);

	int get_SendBufferSize(void);
	void put_SendBufferSize(int newVal);

	bool get_NegotiateAuth(void);
	void put_NegotiateAuth(bool newVal);

	void get_LoginDomain(CkString &str);
	const char *loginDomain(void);
	void put_LoginDomain(const char *newVal);

	int get_SocksVersion(void);
	void put_SocksVersion(int newVal);

	int get_SocksPort(void);
	void put_SocksPort(int newVal);

	void get_SocksUsername(CkString &str);
	const char *socksUsername(void);
	void put_SocksUsername(const char *newVal);

	void get_SocksPassword(CkString &str);
	const char *socksPassword(void);
	void put_SocksPassword(const char *newVal);

	void get_SocksHostname(CkString &str);
	const char *socksHostname(void);
	void put_SocksHostname(const char *newVal);

	CkHttpResponse *PostXml(const char *url, const char *xmlDoc, const char *charset);

	bool get_ProxyPartialUrl(void);
	void put_ProxyPartialUrl(bool newVal);

	CkCert *GetServerSslCert(const char *domain, int port);

	bool get_DigestAuth(void);
	void put_DigestAuth(bool newVal);

	bool SetSslClientCertPfx(const char *pfxFilename, const char *pfxPassword, const char *certSubjectCN);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

	void ClearInMemoryCookies(void);

	bool SetCookieXml(const char *domain, const char *cookieXml);

	CkHttpResponse *PostUrlEncoded(const char *url, CkHttpRequest &req);

	bool PostBinary(const char *url, CkByteData &byteData, const char *contentType, bool md5, bool gzip, CkString &outStr);
	const char *postBinary(const char *url, CkByteData &byteData, const char *contentType, bool md5, bool gzip);

	CkHttpResponse *QuickGetObj(const char *url);

	void get_SessionLogFilename(CkString &str);
	const char *sessionLogFilename(void);
	void put_SessionLogFilename(const char *newVal);

	bool get_UseBgThread(void);
	void put_UseBgThread(bool newVal);

	bool get_BgTaskRunning(void);

	bool get_BgTaskFinished(void);

	bool get_BgTaskSuccess(void);

	void get_BgLastErrorText(CkString &str);
	const char *bgLastErrorText(void);

	void BgTaskAbort(void);

	bool get_KeepEventLog(void);
	void put_KeepEventLog(bool newVal);

	int get_EventLogCount(void);

	bool EventLogName(int index, CkString &outStr);
	const char *eventLogName(int index);

	bool EventLogValue(int index, CkString &outStr);
	const char *eventLogValue(int index);

	void ClearBgEventLog(void);

	void get_BgResultString(CkString &str);
	const char *bgResultString(void);

	int get_BgResultInt(void);

	CkHttpResponse *BgResponseObject(void);

	void get_BgResultData(CkByteData &data);

	void SleepMs(int millisec);

	void get_ClientIpAddress(CkString &str);
	const char *clientIpAddress(void);
	void put_ClientIpAddress(const char *newVal);

	void get_ProxyAuthMethod(CkString &str);
	const char *proxyAuthMethod(void);
	void put_ProxyAuthMethod(const char *newVal);

	bool AddQuickHeader(const char *name, const char *value);

	bool RemoveQuickHeader(const char *name);

};
#pragma pack (pop)

#endif
