
#ifndef _CKCACHE_H
#define _CKCACHE_H

#pragma once

#include "CkString.h"
class CkByteData;

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

#include "CkObject.h"

#pragma pack (push, 8)

class CkCache  : public CkObject
{
    private:

	void *m_impl;
	bool m_utf8;

	CkCache(const CkCache &) { }
	CkCache &operator=(const CkCache &) { return *this; }
	CkCache(void *impl) : m_impl(impl) { }

    public:
	void *getImpl(void) const { return m_impl; }

	CkCache();
	virtual ~CkCache();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
	void LastErrorXml(CkString &str);
	void LastErrorHtml(CkString &str);
	void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	void get_LastEtagFetched(CkString &str);
	const char *lastEtagFetched(void);

	void get_LastExpirationFetched(SYSTEMTIME &sysTime);

	bool get_LastHitExpired(void);

	void get_LastKeyFetched(CkString &str);
	const char *lastKeyFetched(void);

	int get_Level(void);
	void put_Level(int newVal);

	int get_NumRoots(void);

	void AddRoot(const char *path);

	int DeleteAll(void);

	int DeleteAllExpired(void);

	bool DeleteFromCache(const char *url);

	int DeleteOlder(SYSTEMTIME &dt);

	bool FetchFromCache(const char *url, CkByteData &outBytes);

	bool GetEtag(const char *url, CkString &outStr);
	const char *getEtag(const char *url);

	bool GetExpiration(const char *url, SYSTEMTIME &sysTime);

	bool GetFilename(const char *url, CkString &outStr);
	const char *getFilename(const char *url);

	bool GetRoot(int index, CkString &outStr);
	const char *getRoot(int index);

	bool IsCached(const char *url);

	bool SaveToCache(const char *url, SYSTEMTIME &expire, const char *eTag, CkByteData &data);

	bool SaveToCacheNoExpire(const char *url, const char *eTag, CkByteData &data);

	bool UpdateExpiration(const char *url, SYSTEMTIME &dt);

	bool FetchText(const char *key, CkString &outStr);
	const char *fetchText(const char *key);

	bool SaveText(const char *key, SYSTEMTIME &expire, const char *eTag, const char *strData);

	bool SaveTextNoExpire(const char *key, const char *eTag, const char *strData);

	CkString m_resultString;

};
#pragma pack (pop)

#endif
