
#ifndef _CKMHT_H
#define _CKMHT_H

#pragma once

class CkMime;
class CkEmail;
#include "CkString.h"
class CkMhtProgress;

#include "CkObject.h"

#pragma pack (push, 8)

class CkMht  : public CkObject
{
    private:
	CkMhtProgress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkMht(const CkMht &) { }
	CkMht &operator=(const CkMht &) { return *this; }
	CkMht(void *impl) : m_impl(impl) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

	CkMht();
	virtual ~CkMht();

	CkMhtProgress *get_EventCallbackObject(void) const { return m_callback; }
	void put_EventCallbackObject(CkMhtProgress *progress) { m_callback = progress; }

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool UnpackMHT(const char *mhtFilename, const char *unpackDir, const char *htmlFilename, const char *partsSubDir);
	bool UnpackMHTString(const char *mhtContents, const char *unpackDir, const char *htmlFilename, const char *partsSubDir);

	bool GetAndZipMHT(const char *url, const char *zipEntryFilename, const char *zipFilename);
	bool GetAndZipEML(const char *url, const char *zipEntryFilename, const char *zipFilename);
	CkEmail *GetEmail(const char *url);
	bool GetAndSaveMHT(const char *url, const char *mhtFilename);
	bool GetAndSaveEML(const char *url, const char *emlFilename);
	bool GetMHT(const char *url, CkString &mht);
	bool GetEML(const char *url, CkString &eml);

	CkEmail *HtmlToEmail(const char *htmlText);

	bool HtmlToMHT(const char *htmlText, CkString &mht);
	bool HtmlToEML(const char *htmlText, CkString &eml);

	bool UnlockComponent(const char *unlockCode);
	bool IsUnlocked(void) const;

	void AddExternalStyleSheet(const char *url);
	void ExcludeImagesMatching(const char *pattern);
	void RestoreDefaults();

	long get_HeartbeatMs(void);
	void put_HeartbeatMs(long newVal);

	bool get_PreferMHTScripts(void);
	void put_PreferMHTScripts(bool newVal);

	bool get_NoScripts(void);
	void put_NoScripts(bool newVal);

	bool get_UseCids(void);
	void put_UseCids(bool newVal);

	bool get_EmbedImages(void);
	void put_EmbedImages(bool newVal);

	bool get_NtlmAuth(void);
	void put_NtlmAuth(bool newVal);

	bool get_DebugTagCleaning(void);
	void put_DebugTagCleaning(bool newVal);

	void get_WebSiteLogin(CkString &str);
	void put_WebSiteLogin(const char *newVal);

	void get_WebSitePassword(CkString &str);
	void put_WebSitePassword(const char *newVal);

	void get_ProxyLogin(CkString &str);
	void put_ProxyLogin(const char *newVal);

	void get_ProxyPassword(CkString &str);
	void put_ProxyPassword(const char *newVal);

	const char *proxyLogin(void);
	const char *proxyPassword(void);

	void get_DebugHtmlBefore(CkString &str);
	void put_DebugHtmlBefore(const char *newVal);

	void get_DebugHtmlAfter(CkString &str);
	void put_DebugHtmlAfter(const char *newVal);

	void get_BaseUrl(CkString &str);
	void put_BaseUrl(const char *newVal);

	void get_Proxy(CkString &str);
	void put_Proxy(const char *newVal);

	void get_Version(CkString &str);

	long get_ReadTimeout(void);
	void put_ReadTimeout(long numSeconds);

	long get_ConnectTimeout(void);
	void put_ConnectTimeout(long numSeconds);

	void AddCustomHeader(const char *name, const char *value);
	void RemoveCustomHeader(const char *name);
	void ClearCustomHeaders(void);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);

	const char *getMHT(const char *url);
	const char *getEML(const char *url);
	const char *htmlToMHT(const char *htmlText);
	const char *htmlToEML(const char *htmlText);
	const char *webSiteLogin(void);
	const char *webSitePassword(void);
	const char *debugHtmlBefore(void);
	const char *debugHtmlAfter(void);
	const char *baseUrl(void);
	const char *proxy(void);
	const char *version(void);

	void put_NumCacheLevels(long v);
	long get_NumCacheLevels(void);

	long get_NumCacheRoots(void);
	bool GetCacheRoot(int index, CkString &str);
	const char *getCacheRoot(int index);
	void AddCacheRoot(const char *dir);

	bool get_UpdateCache(void);
	void put_UpdateCache(bool b);

	bool get_FetchFromCache(void);
	void put_FetchFromCache(bool b);

	bool get_IgnoreNoCache(void);
	void put_IgnoreNoCache(bool b);

	bool get_IgnoreMustRevalidate(void);
	void put_IgnoreMustRevalidate(bool b);

	bool get_UseIEProxy(void);
	void put_UseIEProxy(bool newVal);

	bool get_UseFilename(void);
	void put_UseFilename(bool newVal);

	bool get_UseInline(void);
	void put_UseInline(bool newVal);

	CkMime *GetMime(const char *url);

	bool HtmlToEMLFile(const char *html, const char *emlFilename);

	bool HtmlToMHTFile(const char *html, const char *mhtFilename);

	bool get_EmbedLocalOnly(void);
	void put_EmbedLocalOnly(bool newVal);

	void get_SocksUsername(CkString &str);
	const char *socksUsername(void);
	void put_SocksUsername(const char *newVal);

	void get_SocksPassword(CkString &str);
	const char *socksPassword(void);
	void put_SocksPassword(const char *newVal);

	void get_SocksHostname(CkString &str);
	const char *socksHostname(void);
	void put_SocksHostname(const char *newVal);

	int get_SocksPort(void);
	void put_SocksPort(int newVal);

	int get_SocksVersion(void);
	void put_SocksVersion(int newVal);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

};

#pragma pack (pop)

#endif
