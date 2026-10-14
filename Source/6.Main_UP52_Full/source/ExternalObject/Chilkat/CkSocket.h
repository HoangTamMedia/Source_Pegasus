
#ifndef _CKSOCKET_H
#define _CKSOCKET_H

#pragma once

class CkByteData;
#include "CkString.h"
#include "CkObject.h"

class CkCert;
class CkSocketProgress;

#pragma warning( disable : 4068 )
#pragma unmanaged

#pragma pack (push, 8)

class CkSocket  : public CkObject
{
    public:
	CkSocket();
	virtual ~CkSocket();

	bool SendBytes(const char *byteData, unsigned long numBytes);

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	CkSocketProgress *get_EventCallbackObject(void) const { return m_callback; }
	void put_EventCallbackObject(CkSocketProgress *progress) { m_callback = progress; }

	CkCert *GetSslServerCert(void);
	CkCert *GetMyCert(void);

	bool UnlockComponent(const char *code);
	bool IsUnlocked(void) const;

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	void get_Version(CkString &str);
	long get_ObjectId(void);

	bool AsyncDnsStart(const char *hostname, int maxWaitMs);
	bool get_AsyncDnsFinished(void);
	void get_AsyncDnsResult(CkString &str);
	void AsyncDnsAbort(void);
	void get_AsyncDnsLog(CkString &str);
	bool get_AsyncDnsSuccess(void);

	bool AsyncConnectStart(const char *hostname, int port, bool ssl, int maxWaitMs);
	bool get_AsyncConnectFinished(void);
	void AsyncConnectAbort(void);
	void get_AsyncConnectLog(CkString &str);
	bool get_AsyncConnectSuccess(void);

	void SleepMs(int millisec) const;

	void get_MyIpAddress(CkString &str);

	bool BindAndListen(int port, int backlog);

	bool Connect(const char *hostname, int port, bool ssl, int maxWaitMs);
	CkSocket *AcceptNextConnection(int maxWaitMs);

	void Close(int maxWaitMs);

	bool get_Ssl(void);
	void put_Ssl(bool newVal);

	bool InitSslServer(CkCert &cert);

	long get_ConnectFailReason(void);

	long get_HeartbeatMs(void);
	void put_HeartbeatMs(long millisec);

	long get_MaxSendIdleMs(void);
	void put_MaxSendIdleMs(long millisec);

	long get_MaxReadIdleMs(void);
	void put_MaxReadIdleMs(long millisec);

	void get_StringCharset(CkString &str);
	void put_StringCharset(const char *str);

	long get_DebugDnsDelayMs(void);
	void put_DebugDnsDelayMs(long millisec);

	long get_DebugConnectDelayMs(void);
	void put_DebugConnectDelayMs(long millisec);

	bool get_IsConnected(void);

	long get_RemotePort(void);
	void get_RemoteIpAddress(CkString &str);

	bool AsyncAcceptStart(int maxWaitMs);
	bool get_AsyncAcceptFinished(void);
	void AsyncAcceptAbort(void);
	void get_AsyncAcceptLog(CkString &str);
	bool get_AsyncAcceptSuccess(void);
	CkSocket *AsyncAcceptSocket(void);

	bool AsyncSendBytes(const unsigned char *byteData, unsigned long numBytes);
	bool AsyncSendString(const char *str);

	bool get_AsyncSendFinished(void);
	void AsyncSendAbort(void);
	void get_AsyncSendLog(CkString &str);
	bool get_AsyncSendSuccess(void);

	bool AsyncReceiveBytes(void);
	bool AsyncReceiveBytesN(unsigned long numBytes);
	bool AsyncReceiveString(void);
	bool AsyncReceiveToCRLF(void);
	bool AsyncReceiveUntilMatch(const char *matchStr);

	bool get_AsyncReceiveFinished(void);
	void AsyncReceiveAbort(void);
	void get_AsyncReceiveLog(CkString &str);
	bool get_AsyncReceiveSuccess(void);

	void get_AsyncReceivedString(CkString &str);
	void get_AsyncReceivedBytes(CkByteData &byteData);

	const char *asyncReceivedString(void);
	const char *asyncReceiveLog(void);
	const char *asyncSendLog(void);

	void BuildHttpGetRequest(const char *url, CkString &str);

	long get_SendPacketSize(void);
	void put_SendPacketSize(long sizeInBytes);

	long get_ReceivePacketSize(void);
	void put_ReceivePacketSize(long sizeInBytes);

	bool SendString(const char *str);
	bool SendBytes(const unsigned char *byteData, unsigned long numBytes);
	bool ReceiveString(CkString &strOut);
	bool ReceiveUntilMatch(const char *matchStr, CkString &strOut);
	bool ReceiveBytes(CkByteData &byteData);
	bool ReceiveBytesN(unsigned long numBytes, CkByteData &byteData);

	const char *stringCharset(void);
	const char *remoteIpAddress(void);
	const char *asyncAcceptLog(void);
	const char *buildHttpGetRequest(const char *url);
	const char *receiveString(void);
	const char *receiveUntilMatch(const char *matchStr);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);

	const char *asyncDnsResult(void);
	const char *asyncDnsLog(void);
	const char *asyncConnectLog(void);

	const char *myIpAddress(void);
	const char *version(void);

	bool DnsLookup(const char *hostname, int maxWaitMs, CkString &outStr);
	const char *dnsLookup(const char *hostname, int maxWaitMs);

	bool ReceiveToCRLF(CkString &outStr);
	const char *receiveToCRLF(void);

	void get_SessionLog(CkString &str);
	const char *sessionLog(void);

	bool get_KeepSessionLog(void);
	void put_KeepSessionLog(bool newVal);

	void get_SessionLogEncoding(CkString &str);
	const char *sessionLogEncoding(void);
	void put_SessionLogEncoding(const char *newVal);

	bool ReceiveUntilByte(int byteValue, CkByteData &outBytes);

	void ClearSessionLog(void);

	bool ReceiveStringUntilByte(int byteValue, CkString &outStr);
	const char *receiveStringUntilByte(int byteValue);

	bool ReceiveStringMaxN(int maxBytes, CkString &outStr);
	const char *receiveStringMaxN(int maxBytes);

	void get_SslProtocol(CkString &str);
	const char *sslProtocol(void);
	void put_SslProtocol(const char *newVal);

	bool SetSslClientCert(CkCert &cert);

	void get_ClientIpAddress(CkString &str);
	const char *clientIpAddress(void);
	void put_ClientIpAddress(const char *newVal);

	bool SendCount(int byteCount);

	int ReceiveCount(void);

	bool get_LastMethodFailed(void);

	bool ConvertToSsl(void);

	bool ConvertFromSsl(void);

	int get_SoSndBuf(void);
	void put_SoSndBuf(int newVal);

	int get_SoRcvBuf(void);
	void put_SoRcvBuf(int newVal);

	int get_ClientPort(void);
	void put_ClientPort(int newVal);

	void get_LocalIpAddress(CkString &str);
	const char *localIpAddress(void);

	int get_LocalPort(void);

	int get_SocksPort(void);
	void put_SocksPort(int newVal);

	int get_SocksVersion(void);
	void put_SocksVersion(int newVal);

	void get_SocksUsername(CkString &str);
	const char *socksUsername(void);
	void put_SocksUsername(const char *newVal);

	void get_SocksPassword(CkString &str);
	const char *socksPassword(void);
	void put_SocksPassword(const char *newVal);

	void get_SocksHostname(CkString &str);
	const char *socksHostname(void);
	void put_SocksHostname(const char *newVal);

	int CheckWriteable(int maxWaitMs);

	bool SetSslClientCertPfx(const char *pfxFilename, const char *pfxPassword, const char *certSubjectCN);

	bool get_TcpNoDelay(void);
	void put_TcpNoDelay(bool newVal);

	bool get_BigEndian(void);
	void put_BigEndian(bool newVal);

	bool PollDataAvailable(void);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

	bool TakeSocket(CkSocket &sock);

	int SelectForReading(int timeoutMs);

	int SelectForWriting(int timeoutMs);

	int get_SelectorIndex(void);
	void put_SelectorIndex(int newVal);

	int get_SelectorReadIndex(void);
	void put_SelectorReadIndex(int newVal);

	int get_SelectorWriteIndex(void);
	void put_SelectorWriteIndex(int newVal);

	int get_NumSocketsInSet(void);

	void get_UserData(CkString &str);
	const char *userData(void);
	void put_UserData(const char *newVal);

	void StartTiming(void);

	int get_ElapsedSeconds(void);

	bool ReceiveBytesToFile(const char *appendFilename);

	void get_HttpProxyUsername(CkString &str);
	const char *httpProxyUsername(void);
	void put_HttpProxyUsername(const char *newVal);

	void get_HttpProxyPassword(CkString &str);
	const char *httpProxyPassword(void);
	void put_HttpProxyPassword(const char *newVal);

	void get_HttpProxyAuthMethod(CkString &str);
	const char *httpProxyAuthMethod(void);
	void put_HttpProxyAuthMethod(const char *newVal);

	void get_HttpProxyHostname(CkString &str);
	const char *httpProxyHostname(void);
	void put_HttpProxyHostname(const char *newVal);

	int get_HttpProxyPort(void);
	void put_HttpProxyPort(int newVal);

	int get_NumSslAcceptableClientCAs(void);

	bool GetSslAcceptableClientCaDn(int index, CkString &outStr);
	const char *getSslAcceptableClientCaDn(int index);

	bool AddSslAcceptableClientCaDn(const char *certAuthDN);

	int get_ReceivedCount(void);
	void put_ReceivedCount(int newVal);

    private:
	CkSocketProgress *m_callback;

	void *m_implCb;
	void *m_impl;
	bool m_utf8;

	CkSocket(const CkSocket &) { }
	CkSocket &operator=(const CkSocket &) { return *this; }
	CkSocket(void *impl);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

};

#pragma pack (pop)

#endif
