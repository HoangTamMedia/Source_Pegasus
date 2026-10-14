
#ifndef _CkSsh_H
#define _CkSsh_H

#pragma once

#include "CkString.h"
class CkByteData;
class CkSshProgress;
class CkSshKey;
#include "CkObject.h"

#pragma pack (push, 8)

class CkSsh  : public CkObject
{
    private:
	CkSshProgress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkSsh(const CkSsh &);
	CkSsh &operator=(const CkSsh &);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

	CkSsh(void *impl);

	CkSsh();
	virtual ~CkSsh();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
    void LastErrorXml(CkString &str);
    void LastErrorHtml(CkString &str);
    void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	CkSshProgress *get_EventCallbackObject(void) const;
	void put_EventCallbackObject(CkSshProgress *progress);

	bool Connect(const char *hostname, int port);

	bool UnlockComponent(const char *unlockCode);

	bool AuthenticatePw(const char *login, const char *password);

	void get_Version(CkString &str);
	const char *version(void);

	bool get_KeepSessionLog(void);
	void put_KeepSessionLog(bool newVal);

	void get_SessionLog(CkString &str);
	const char *sessionLog(void);

	int get_IdleTimeoutMs(void);
	void put_IdleTimeoutMs(int newVal);

	int get_ConnectTimeoutMs(void);
	void put_ConnectTimeoutMs(int newVal);

	int get_ChannelOpenFailCode(void);

	int get_DisconnectCode(void);

	void get_DisconnectReason(CkString &str);
	const char *disconnectReason(void);

	void get_ChannelOpenFailReason(CkString &str);
	const char *channelOpenFailReason(void);

	int get_MaxPacketSize(void);
	void put_MaxPacketSize(int newVal);

	void Disconnect(void);

	int OpenSessionChannel(void);

	int OpenCustomChannel(const char *channelType);

	int get_NumOpenChannels(void);

	int GetChannelNumber(int index);

	bool GetChannelType(int index, CkString &outStr);
	const char *getChannelType(int index);

	bool SendReqPty(int channelNum, const char *xTermEnvVar, int widthInChars, int heightInRows, int pixWidth, int pixHeight);

	bool SendReqX11Forwarding(int channelNum, bool singleConnection, const char *authProt, const char *authCookie, int screenNum);

	bool SendReqSetEnv(int channelNum, const char *name, const char *value);

	bool SendReqShell(int channelNum);

	bool SendReqExec(int channelNum, const char *command);

	bool SendReqSubsystem(int channelNum, const char *subsystemName);

	bool SendReqWindowChange(int channelNum, int widthInChars, int heightInRows, int pixWidth, int pixHeight);

	bool SendReqXonXoff(int channelNum, bool clientCanDo);

	bool SendReqSignal(int channelNum, const char *signalName);

	bool ChannelSendData(int channelNum, CkByteData &data);

	bool ChannelSendString(int channelNum, const char *strData, const char *charset);

	int ChannelPoll(int channelNum, int pollTimeoutMs);

	int ChannelReadAndPoll(int channelNum, int pollTimeoutMs);

	int ChannelRead(int channelNum);

	void GetReceivedData(int channelNum, CkByteData &outBytes);

	void GetReceivedStderr(int channelNum, CkByteData &outBytes);

	bool ChannelReceivedEof(int channelNum);

	bool ChannelReceivedClose(int channelNum);

	bool ChannelSendClose(int channelNum);

	bool ChannelSendEof(int channelNum);

	bool ChannelIsOpen(int channelNum);

	bool ChannelReceiveToClose(int channelNum);

	void ClearTtyModes(void);

	bool SetTtyMode(const char *name, int value);

	bool get_IsConnected(void);

	bool ReKey(void);

	bool AuthenticatePk(const char *username, CkSshKey &privateKey);

	bool GetReceivedText(int channelNum, const char *charset, CkString &outStr);
	const char *getReceivedText(int channelNum, const char *charset);

	int GetReceivedNumBytes(int channelNum);

	bool ChannelReceiveUntilMatch(int channelNum, const char *matchPattern, const char *charset, bool caseSensitive);

	bool SendIgnore(void);

	int OpenDirectTcpIpChannel(const char *hostname, int port);

	bool GetReceivedTextS(int channelNum, const char *substr, const char *charset, CkString &outStr);
	const char *getReceivedTextS(int channelNum, const char *substr, const char *charset);

	bool GetReceivedDataN(int channelNum, int numBytes, CkByteData &outBytes);

	bool PeekReceivedText(int channelNum, const char *charset, CkString &outStr);
	const char *peekReceivedText(int channelNum, const char *charset);

	int get_HeartbeatMs(void);
	void put_HeartbeatMs(int newVal);

	bool ChannelReceivedExitStatus(int channelNum);

	int GetChannelExitStatus(int channelNum);

	void get_ClientIdentifier(CkString &str);
	const char *clientIdentifier(void);
	void put_ClientIdentifier(const char *newVal);

	int get_ReadTimeoutMs(void);
	void put_ReadTimeoutMs(int newVal);

	bool get_TcpNoDelay(void);
	void put_TcpNoDelay(bool newVal);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

	void get_HostKeyFingerprint(CkString &str);
	const char *hostKeyFingerprint(void);

	int get_SocksVersion(void);
	void put_SocksVersion(int newVal);

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

	void get_HttpProxyAuthMethod(CkString &str);
	const char *httpProxyAuthMethod(void);
	void put_HttpProxyAuthMethod(const char *newVal);

	void get_HttpProxyUsername(CkString &str);
	const char *httpProxyUsername(void);
	void put_HttpProxyUsername(const char *newVal);

	void get_HttpProxyPassword(CkString &str);
	const char *httpProxyPassword(void);
	void put_HttpProxyPassword(const char *newVal);

	void get_HttpProxyHostname(CkString &str);
	const char *httpProxyHostname(void);
	void put_HttpProxyHostname(const char *newVal);

	int get_HttpProxyPort(void);
	void put_HttpProxyPort(int newVal);

	bool ChannelReceiveUntilMatchN(int channelNum, CkStringArray &matchPatterns, const char *charset, bool caseSensitive);

	int ChannelReadAndPoll2(int channelNum, int pollTimeoutMs, int maxNumBytes);

	bool get_PasswordChangeRequested(void);

	bool AuthenticatePwPk(const char *username, const char *password, CkSshKey &privateKey);

};
#pragma pack (pop)

#endif
