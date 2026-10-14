
#ifndef _CkSocksProxy_H
#define _CkSocksProxy_H

#pragma once

#include "CkString.h"
class CkSocksProxyProgress;
#include "CkObject.h"

#pragma pack (push, 8)

class CkSocksProxy  : public CkObject
{
    private:
	CkSocksProxyProgress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkSocksProxy(const CkSocksProxy &);
	CkSocksProxy &operator=(const CkSocksProxy &);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

	CkSocksProxy(void *impl);

	CkSocksProxy();
	virtual ~CkSocksProxy();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
    void LastErrorXml(CkString &str);
    void LastErrorHtml(CkString &str);
    void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	CkSocksProxyProgress *get_EventCallbackObject(void) const;
	void put_EventCallbackObject(CkSocksProxyProgress *progress);

	bool UnlockComponent(const char *unlockCode);

	int get_SocksVersion(void);

	bool get_AuthenticatedSocks5(void);

	bool get_ConnectionPending(void);

	int get_ClientPort(void);

	int get_ServerPort(void);

	void get_ClientIp(CkString &str);
	const char *clientIp(void);

	void get_ServerIp(CkString &str);
	const char *serverIp(void);

	void get_Login(CkString &str);
	const char *login(void);

	void get_Password(CkString &str);
	const char *password(void);

	bool Initialize(int port);

	bool WaitForConnection(int maxWaitMs);

	bool AllowConnection(void);

	bool ProceedSocks5(void);

	bool RejectConnection(void);

	bool get_AllowUnauthenticatedSocks5(void);
	void put_AllowUnauthenticatedSocks5(bool newVal);

	void get_ListenBindIpAddress(CkString &str);
	const char *listenBindIpAddress(void);
	void put_ListenBindIpAddress(const char *newVal);

	void get_OutboundBindIpAddress(CkString &str);
	const char *outboundBindIpAddress(void);
	void put_OutboundBindIpAddress(const char *newVal);

	int get_OutboundBindPort(void);
	void put_OutboundBindPort(int newVal);

	bool GetTunnelsXml(CkString &outStr);
	const char *getTunnelsXml(void);

	bool StopAllTunnels(int maxWaitMs);

};
#pragma pack (pop)

#endif
