
#ifndef _CKIMAP_H
#define _CKIMAP_H

#pragma once

class CkByteData;
#include "CkString.h"
class CkMailboxes;
class CkMessageSet;
class CkEmail;
class CkEmailBundle;
class CkStringArray;
class CkImapProgress;
class CkCert;
class CkPrivateKey;
class CkSshKey;

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

#include "CkObject.h"

#undef Copy

#pragma pack (push, 8)

class CkImap  : public CkObject
{
    public:
	CkImap();
	virtual ~CkImap();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SetDecryptCert2(const CkCert &cert, CkPrivateKey &key);

	bool Connect(const char *hostname);
	bool Disconnect(void);
	bool IsConnected(void);

	bool Login(const char *login, const char *password);
	bool Logout(void);
	bool IsLoggedIn(void);

	void get_LoggedInUser(CkString &str);
	void get_ConnectedToHost(CkString &str);

	bool CreateMailbox(const char *mailbox);
	bool DeleteMailbox(const char *mailbox);
	bool RenameMailbox(const char *fromMailbox, const char *toMailbox);

	void LastResponse(CkString &str);

	CkMailboxes *ListMailboxes(const char *reference, const char *wildcardedMailbox);

	bool SelectMailbox(const char *mailbox);
	bool ExamineMailbox(const char *mailbox);

	bool Subscribe(const char *mailbox);
	bool Unsubscribe(const char *mailbox);

	bool SetSslClientCert(CkCert &cert);
	CkCert *GetSslServerCert(void);
	bool get_SslServerCertVerified(void);

	CkMessageSet *Search(const char *criteria, bool bUid);

	CkMessageSet *GetAllUids(void);

	CkEmail *FetchSingle(long msgId, bool bUid);
	CkEmail *FetchSingleHeader(long msgId, bool bUid);
	CkEmailBundle *FetchBundle(const CkMessageSet &messageSet);
	CkEmailBundle *FetchHeaders(const CkMessageSet &messageSet);
	CkEmailBundle *FetchChunk(int startSeqNum, int count, CkMessageSet &failedSet, CkMessageSet &fetchedSet);

	CkStringArray *FetchSequenceAsMime(int startSeqNum, int count);
	CkEmailBundle *FetchSequence(int startSeqNum, int numMessages);
	CkEmailBundle *FetchSequenceHeaders(int startSeqNum, int numMessages);

	bool FetchSingleAsMime(long msgId, bool bUid, CkString &strMime);
	CkStringArray *FetchBundleAsMime(const CkMessageSet &messageSet);

	bool Expunge(void);
	bool ExpungeAndClose(void);

	bool StoreFlags(long msgId, bool bUid, const char *flagName, long value);
	bool FetchFlags(long msgId, bool bUid, CkString &strFlags);

	bool SetFlag(long msgId, bool bUid, const char *flagName, long value);
	bool SetFlags(const CkMessageSet &messageSet, const char *flagName, long value);

	void get_AuthMethod(CkString &str);
	void put_AuthMethod(const char *str);

	char get_SeparatorChar(void);
	void put_SeparatorChar(char c);

	void get_Domain(CkString &str);
	void put_Domain(const char *str);

	int get_Port(void);
	void put_Port(int port);

	bool get_Ssl(void);
	void put_Ssl(bool s);

	void ClearSessionLog(void);
	void get_SessionLog(CkString &str);
	bool get_KeepSessionLog(void);
	void put_KeepSessionLog(bool newVal);

	void get_LastResponse(CkString &str);
	void get_LastIntermediateResponse(CkString &str);
	void get_LastCommand(CkString &str);

	long get_HeartbeatMs(void);
	void put_HeartbeatMs(long newVal);

	bool AppendMime(const char *mailbox, const char *mimeText);
	bool AppendMail(const char *mailbox, const CkEmail &email);

	bool Copy(long msgId, bool bUid, const char *copyToMailbox);
	bool CopyMultiple(CkMessageSet &messageSet, const char *copyToMailbox);
	bool CopySequence(int startSeqNum, int count, const char *copyToMailbox);

	bool SetMailFlag(const CkEmail &email, const char *flagName, long value);
	long GetMailFlag(const CkEmail &email, const char *flagName);

	bool RefetchMailFlags(CkEmail &emailInOut);

	long get_NumMessages(void);

	long GetMailSize(const CkEmail &email);
	long GetMailNumAttach(const CkEmail &email);
	long GetMailAttachSize(const CkEmail &email, long attachIndex);
	bool GetMailAttachFilename(const CkEmail &email, long attachIndex, CkString &strFilename);

	bool get_AutoUnwrapSecurity(void);
	void put_AutoUnwrapSecurity(bool bUnwrap);

	bool get_PeekMode(void);
	void put_PeekMode(bool bPeek);

	bool get_AppendSeen(void);
	void put_AppendSeen(bool alreadySeen);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	bool UnlockComponent(const char *unlockCode);
	bool IsUnlocked(void) const;

	int get_ReadTimeout(void) const;
	void put_ReadTimeout(int numSeconds);

	int get_ConnectTimeout(void) const;
	void put_ConnectTimeout(int numSeconds);

	bool SendRawCommand(const char *rawCommand, CkString &rawResponse);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);

	const char *getMailAttachFilename(const CkEmail &email, long attachIndex);
	const char *fetchFlags(long msgId, bool bUid);
        const char *authMethod(void);
        const char *domain(void);
	const char *fetchSingleAsMime(long msgId, bool bUid);
	const char *loggedInUser(void);
	const char *connectedToHost(void);
	const char *sendRawCommand(const char *rawCommand);
	const char *sessionLog(void);

	const char *lastResponse(void);
	const char *lastCommand(void);
	const char *lastIntermediateResponse(void);

	void get_LastAppendedMime(CkString &str);
	const char *lastAppendedMime(void);

	void get_Version(CkString &str);
	const char *version(void);

	bool AppendMimeWithFlags(const char *mailbox, const char *mimeText, bool seen, bool flagged, bool answered, bool draft);

	bool FetchSingleHeaderAsMime(int msgId, bool bUID, CkString &outStr);
	const char *fetchSingleHeaderAsMime(int msgId, bool bUID);

	CkImapProgress *get_EventCallbackObject(void) const { return m_callback; }
	void put_EventCallbackObject(CkImapProgress *progress) { m_callback = progress; }

	bool AppendMimeWithDate(const char *mailbox, const char *mimeText, SYSTEMTIME &internalDate);

	void get_SelectedMailbox(CkString &str);
	const char *selectedMailbox(void);

	int get_AppendUid(void);

	void get_AuthzId(CkString &str);
	const char *authzId(void);
	void put_AuthzId(const char *newVal);

	int get_SendBufferSize(void);
	void put_SendBufferSize(int newVal);

	bool SshTunnel(const char *sshServerHostname, int sshServerPort);

	bool SshAuthenticatePw(const char *sshLogin, const char *sshPassword);

	bool SshAuthenticatePk(const char *sshLogin, CkSshKey &privateKey);

	bool Noop(void);

	void get_SocksHostname(CkString &str);
	const char *socksHostname(void);
	void put_SocksHostname(const char *newVal);

	void get_SocksUsername(CkString &str);
	const char *socksUsername(void);
	void put_SocksUsername(const char *newVal);

	void get_SocksPassword(CkString &str);
	const char *socksPassword(void);
	void put_SocksPassword(const char *newVal);

	int get_SocksPort(void);
	void put_SocksPort(int newVal);

	int get_SocksVersion(void);
	void put_SocksVersion(int newVal);

	bool SetSslClientCertPfx(const char *pfxFilename, const char *pfxPassword, const char *certSubjectCN);

	bool get_StartTls(void);
	void put_StartTls(bool newVal);

	void get_SslProtocol(CkString &str);
	const char *sslProtocol(void);
	void put_SslProtocol(const char *newVal);

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

	int get_UidNext(void);

	bool get_AutoFix(void);
	void put_AutoFix(bool newVal);

	bool AddPfxSourceData(CkByteData &pfxData, const char *password);

	bool AddPfxSourceFile(const char *pfxFilePath, const char *password);

    private:
	CkImapProgress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkImap(const CkImap &) { }
	CkImap &operator=(const CkImap &) { return *this; }
	CkImap(void *impl) : m_impl(impl) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

};
#pragma pack (pop)

#endif
