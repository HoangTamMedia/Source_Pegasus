
#ifndef _CkSshTunnel_H
#define _CkSshTunnel_H

#pragma once

#include "CkString.h"
class CkSshTunnelProgress;
#include "CkObject.h"

class CkSshKey;

#pragma pack (push, 8)

class CkSshTunnel  : public CkObject
{
	void *m_impl;
	bool m_utf8;

	CkSshTunnel(const CkSshTunnel &);
	CkSshTunnel &operator=(const CkSshTunnel &);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

	CkSshTunnel(void *impl);

	CkSshTunnel();
	virtual ~CkSshTunnel();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
    void LastErrorXml(CkString &str);
    void LastErrorHtml(CkString &str);
    void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	bool UnlockComponent(const char *unlockCode);

	void get_SshLogin(CkString &str);
	const char *sshLogin(void);
	void put_SshLogin(const char *newVal);

	void get_SshPassword(CkString &str);
	const char *sshPassword(void);
	void put_SshPassword(const char *newVal);

	void get_ConnectLog(CkString &str);
	const char *connectLog(void);
	void put_ConnectLog(const char *newVal);

	void get_DestHostname(CkString &str);
	const char *destHostname(void);
	void put_DestHostname(const char *newVal);

	void get_SshHostname(CkString &str);
	const char *sshHostname(void);
	void put_SshHostname(const char *newVal);

	int get_SshPort(void);
	void put_SshPort(int newVal);

	int get_DestPort(void);
	void put_DestPort(int newVal);

	bool get_IsAccepting(void);

	bool StopAllTunnels(int maxWaitMs);

	bool GetTunnelsXml(CkString &outStr);
	const char *getTunnelsXml(void);

	bool StopAccepting(void);

	bool BeginAccepting(int listenPort);

	bool SetSshAuthenticationKey(CkSshKey &key);

	void get_ListenBindIpAddress(CkString &str);
	const char *listenBindIpAddress(void);
	void put_ListenBindIpAddress(const char *newVal);

	void get_OutboundBindIpAddress(CkString &str);
	const char *outboundBindIpAddress(void);
	void put_OutboundBindIpAddress(const char *newVal);

	int get_OutboundBindPort(void);
	void put_OutboundBindPort(int newVal);

	int get_MaxPacketSize(void);
	void put_MaxPacketSize(int newVal);

	bool get_TcpNoDelay(void);
	void put_TcpNoDelay(bool newVal);

	void get_HttpProxyHostname(CkString &str);
	const char *httpProxyHostname(void);
	void put_HttpProxyHostname(const char *newVal);

	void get_HttpProxyUsername(CkString &str);
	const char *httpProxyUsername(void);
	void put_HttpProxyUsername(const char *newVal);

	void get_HttpProxyPassword(CkString &str);
	const char *httpProxyPassword(void);
	void put_HttpProxyPassword(const char *newVal);

	void get_HttpProxyAuthMethod(CkString &str);
	const char *httpProxyAuthMethod(void);
	void put_HttpProxyAuthMethod(const char *newVal);

	int get_HttpProxyPort(void);
	void put_HttpProxyPort(int newVal);

	int get_SocksPort(void);
	void put_SocksPort(int newVal);

	void get_SocksHostname(CkString &str);
	const char *socksHostname(void);
	void put_SocksHostname(const char *newVal);

	void get_SocksUsername(CkString &str);
	const char *socksUsername(void);
	void put_SocksUsername(const char *newVal);

	void get_SocksPassword(CkString &str);
	const char *socksPassword(void);
	void put_SocksPassword(const char *newVal);

	int get_SocksVersion(void);
	void put_SocksVersion(int newVal);

	int get_ConnectTimeoutMs(void);
	void put_ConnectTimeoutMs(int newVal);

	int get_ListenPort(void);

};
#pragma pack (pop)

#endif
