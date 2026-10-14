
#ifndef _CKMAILMAN_H
#define _CKMAILMAN_H

#pragma once

#include "CkString.h"
class CkByteData;
class CkEmail;
class CkEmailBundle;
class CkStringArray;
class CkMailProgress;
class CkPrivateKey;
class CkCert;
class CkSshKey;

#include "CkObject.h"

#pragma pack (push, 8)

class CkMailMan  : public CkObject
{
    public:
	CkMailMan();
	virtual ~CkMailMan();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);

        void LastErrorXml(CkString &str);

        void LastErrorHtml(CkString &str);

        void LastErrorText(CkString &str);

	bool SetDecryptCert2(const CkCert &cert, CkPrivateKey &key);

	void get_SmtpPassword(CkString &str);
	void put_SmtpPassword(const char *str);

	void get_SmtpUsername(CkString &str);
	void put_SmtpUsername(const char *str);

	void get_DsnNotify(CkString &str);
	void put_DsnNotify(const char *str);

	void get_DsnEnvid(CkString &str);
	void put_DsnEnvid(const char *str);

	void get_DsnRet(CkString &str);
	void put_DsnRet(const char *str);

	long get_LastSmtpStatus(void);

	bool SetSslClientCert(CkCert &cert);
	CkCert *GetPop3SslServerCert(void);
	CkCert *GetSmtpSslServerCert(void);
	bool get_Pop3SslServerCertVerified(void);
	bool get_SmtpSslServerCertVerified(void);

	long get_HeartbeatMs(void);
	void put_HeartbeatMs(long millisec);

	long get_ReadTimeout(void);
	void put_ReadTimeout(long numSeconds);

	long get_ConnectTimeout(void);
	void put_ConnectTimeout(long numSeconds);

	bool get_AutoUnwrapSecurity(void);
	void put_AutoUnwrapSecurity(bool newVal);

	bool get_ResetDateOnLoad(void);
	void put_ResetDateOnLoad(bool newVal);

	bool get_OpaqueSigning(void);
	void put_OpaqueSigning(bool newVal);

	bool get_AllOrNone(void);
	void put_AllOrNone(bool newVal);

	bool get_Pop3SPA(void);
	void put_Pop3SPA(bool newVal);

	bool get_StartTLS(void);
	void put_StartTLS(bool newVal);

	bool get_EmbedCertChain(void);
	void put_EmbedCertChain(bool newVal);

	bool get_AutoSaveCerts(void);
	void put_AutoSaveCerts(bool newVal);

	bool get_PopSsl(void);
	void put_PopSsl(bool newVal);

	bool get_SmtpSsl(void);
	void put_SmtpSsl(bool newVal);

	bool get_SendIndividual(void);
	void put_SendIndividual(bool newVal);

	void get_SmtpAuthMethod(CkString &str);
	void put_SmtpAuthMethod(const char *str);

	void get_SmtpLoginDomain(CkString &str);
	void put_SmtpLoginDomain(const char *str);

	void get_Filter(CkString &str);
	void put_Filter(const char *str);

	long get_SizeLimit(void);
	void put_SizeLimit(long newVal);

	long get_MaxCount(void);
	void put_MaxCount(long newVal);

	long get_MailPort(void);
	void put_MailPort(long newVal);

	long get_SmtpPort(void);
	void put_SmtpPort(long newVal);

	void get_PopPassword(CkString &str);
	void put_PopPassword(const char *str);
	void put_PopPasswordBase64(const char *strBase64);

	void get_PopUsername(CkString &str);
	void put_PopUsername(const char *str);

	void get_MailHost(CkString &str);
	void put_MailHost(const char *str);

	void get_HeloHostname(CkString &str);
	void put_HeloHostname(const char *str);

	void get_LastSendQFilename(CkString &str);

	void get_Version(CkString &str);

	void get_SmtpHost(CkString &str);
	void put_SmtpHost(const char *str);

	bool get_AutoGenMessageId(void);
	void put_AutoGenMessageId(bool newVal);

	void get_LogMailReceivedFilename(CkString &str);
	void put_LogMailReceivedFilename(const char *str);

	void get_LogMailSentFilename(CkString &str);
	void put_LogMailSentFilename(const char *str);

        void ClearBadEmailAddresses(void);

        CkStringArray *GetBadEmailAddresses(void);

	CkStringArray *GetUidls(void);

	bool IsSmtpDsnCapable(void);

	bool VerifySmtpLogin(void);

	bool VerifySmtpConnection(void);

	bool VerifyPopLogin(void);

	bool VerifyPopConnection(void);

	bool FetchMime(const char *uidl, CkByteData &mimeBytes);

	int GetSizeByUidl(const char *uidl);

	unsigned long GetMailboxSize(void);
	int GetMailboxCount(void);
	bool Pop3BeginSession(void);
	bool Pop3EndSession(void);
	bool Pop3Noop(void);
	bool Pop3Reset(void);

	CkEmailBundle *FetchMultiple(const CkStringArray *uidlArray);
	CkEmailBundle *FetchMultiple(const CkStringArray &uidlArray) { return this->FetchMultiple(&uidlArray); }

	CkStringArray *FetchMultipleMime(const CkStringArray *uidlArray);
	CkStringArray *TransferMultipleMime(const CkStringArray *uidlArray);
	CkStringArray *FetchMultipleMime(const CkStringArray &uidlArray) { return this->FetchMultipleMime(&uidlArray); }
	CkStringArray *TransferMultipleMime(const CkStringArray &uidlArray) { return this->TransferMultipleMime(&uidlArray); }

	CkEmailBundle *FetchMultipleHeaders(const CkStringArray *uidlArray, long numBodyLines);
	CkEmailBundle *FetchMultipleHeaders(const CkStringArray &uidlArray, long numBodyLines) { return this->FetchMultipleHeaders(&uidlArray,numBodyLines); }

	bool DeleteByUidl(const char *uidl);

	CkEmail *FetchEmail(const char *uidl);

	bool DeleteMultiple(const CkStringArray *uidlArray);
	bool DeleteMultiple(const CkStringArray &uidlArray) { return this->DeleteMultiple(&uidlArray); }

	bool DeleteBundle(const CkEmailBundle *bundle);
	bool DeleteBundle(const CkEmailBundle &bundle) { return this->DeleteBundle(&bundle); }

	CkEmail *GetFullEmail(const CkEmail *email);
	CkEmail *GetFullEmail(const CkEmail &email) { return this->GetFullEmail(&email); }

	CkEmailBundle *GetHeaders(long numBodyLines, long fromIndex, long toIndex);

	CkEmail *FetchSingleHeader(long numBodyLines, long index);
	CkEmail *FetchSingleHeaderByUidl(long numBodyLines, const char *uidl);

	bool DeleteEmail(const CkEmail *email);
	bool DeleteEmail(const CkEmail &email) { return this->DeleteEmail(&email); }

	bool get_ImmediateDelete(void);
	void put_ImmediateDelete(bool newVal);

	CkEmailBundle *GetAllHeaders(long numBodyLines);

	CkEmailBundle *LoadXmlString(const char *xmlString);

	CkEmailBundle *LoadXmlFile(const char *folderPath);

	CkEmail *LoadXmlEmail(const char *folderPath);
#ifndef WINCE
	bool MxLookup(const char *emailAddr, CkString &strHostname);
	CkStringArray *MxLookupAll(const char *emailAddress);
#endif

	CkEmail *LoadXmlEmailString(const char *xmlString);

	bool QuickSend(const char *from, const char *to, const char *subject, const char *body, const char *smtpServer);

	bool UnlockComponent(const char *code);

	long CheckMail(void);

	CkEmailBundle *TransferMail(void);

	CkEmailBundle *CopyMail(void);

	bool SendBundle(const CkEmailBundle *bundle);
	bool SendBundle(const CkEmailBundle &bundle) { return this->SendBundle(&bundle); }

	bool SendEmail(const CkEmail *email);
	bool SendEmail(const CkEmail &email) { return this->SendEmail(&email); }

	bool VerifyRecipients(const CkEmail &email, CkStringArray &saBadAddrs);

#ifndef WINCE
	bool SendQ(const CkEmail *email);
	bool SendQ2(const CkEmail *email, const char *queueDir);
	bool SendQ(const CkEmail &email) { return this->SendQ(&email); }
	bool SendQ2(const CkEmail &email, const char *queueDir) { return this->SendQ2(&email,queueDir); }
#endif

	CkEmailBundle *LoadMbx(const char *mbxFileName);

	CkEmail *LoadEml(const char *emlFilename);

	CkEmail *LoadMime(const char *mimeText);

	bool IsUnlocked(void) const;

	bool RenderToMime(const CkEmail *email, CkString &str);
	bool RenderToMime(const CkEmail &email, CkString &str) { return this->RenderToMime(&email,str); }

	bool SendMime(const char *from, const char *recipients, const char *mimeMsg);
	bool SendMimeQ(const char *from, const char *recipients, const char *mimeMsg);

	bool OpenSmtpConnection(void);
	bool CloseSmtpConnection(void);
	bool SmtpReset(void);
	bool SmtpNoop(void);

	bool GetMailboxInfoXml(CkString &xml);

	void get_SmtpSessionLog(CkString &log);
	void ClearSmtpSessionLog(void);
        const char *smtpSessionLog(void);

	void get_Pop3SessionLog(CkString &log);
	void ClearPop3SessionLog(void);
        const char *pop3SessionLog(void);

	void SetProgressCallback(CkMailProgress *cb) { m_callback = cb; }

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);
	const char *smtpPassword(void);
	const char *smtpUsername(void);
	const char *dsnNotify(void);
	const char *dsnEnvid(void);
	const char *dsnRet(void);
	const char *smtpAuthMethod(void);
	const char *smtpLoginDomain(void);
	const char *filter(void);
	const char *popPassword(void);
	const char *popUsername(void);
	const char *mailHost(void);
	const char *heloHostname(void);
	const char *lastSendQFilename(void);
	const char *version(void);
	const char *smtpHost(void);
	const char *logMailReceivedFilename(void);
	const char *logMailSentFilename(void);
#ifndef WINCE
	const char *mxLookup(const char *emailAddr);
#endif
	const char *renderToMime(const CkEmail &email);
	const char *getMailboxInfoXml(void);

	bool SendMimeToList(const char *from, const char *distListFile, const char *mimeText);

	bool SendToDistributionList(CkEmail &email, CkStringArray &array);

	void get_ClientIpAddress(CkString &str);
	const char *clientIpAddress(void);
	void put_ClientIpAddress(const char *newVal);

	int get_Pop3SessionId(void);

	bool DeleteByMsgnum(int msgnum);

	CkEmail *FetchByMsgnum(int msgnum);

	bool FetchMimeByMsgnum(int msgnum, CkByteData &outBytes);

	int get_SendBufferSize(void);
	void put_SendBufferSize(int newVal);

	bool SshTunnel(bool bSmtp, const char *sshServerHostname, int sshServerPort);

	bool SshAuthenticatePw(bool bSmtp, const char *sshLogin, const char *sshPassword);

	bool SshAuthenticatePk(bool bSmtp, const char *sshLogin, CkSshKey &privateKey);

	bool SshCloseTunnel(bool bSmtp);

	bool get_IncludeRootCert(void);
	void put_IncludeRootCert(bool newVal);

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

	bool SendMimeBytes(const char *from, const char *recipients, CkByteData &mimeData);

	bool RenderToMimeBytes(CkEmail &email, CkByteData &outBytes);

#ifndef WINCE

	bool SendMimeBytesQ(const char *from, const char *recipients, CkByteData &mimeData);

#endif

	void get_HttpProxyHostname(CkString &str);
	const char *httpProxyHostname(void);
	void put_HttpProxyHostname(const char *newVal);

	void get_HttpProxyAuthMethod(CkString &str);
	const char *httpProxyAuthMethod(void);
	void put_HttpProxyAuthMethod(const char *newVal);

	void get_HttpProxyUsername(CkString &str);
	const char *httpProxyUsername(void);
	void put_HttpProxyUsername(const char *newVal);

	void get_HttpProxyPassword(CkString &str);
	const char *httpProxyPassword(void);
	void put_HttpProxyPassword(const char *newVal);

	int get_HttpProxyPort(void);
	void put_HttpProxyPort(int newVal);

	bool get_AutoFix(void);
	void put_AutoFix(bool newVal);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

	bool get_Pop3Stls(void);
	void put_Pop3Stls(bool newVal);

	bool AddPfxSourceData(CkByteData &pfxData, const char *password);

	bool AddPfxSourceFile(const char *pfxFilePath, const char *password);

	bool SmtpSendRawCommand(const char *command, const char *charset, bool bEncodeBase64, CkString &outStr);
	const char *smtpSendRawCommand(const char *command, const char *charset, bool bEncodeBase64);

	bool Pop3SendRawCommand(const char *command, const char *charset, CkString &outStr);
	const char *pop3SendRawCommand(const char *command, const char *charset);

	bool Pop3EndSessionNoQuit(void);

    private:

	void *m_impl;
	bool m_utf8;
	CkMailProgress *m_callback;

	CkMailMan(const CkMailMan &) { }
	CkMailMan &operator=(const CkMailMan &) { return *this; }
	CkMailMan(void *impl) : m_callback(0),m_impl(impl) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

};
#pragma pack (pop)

#endif
