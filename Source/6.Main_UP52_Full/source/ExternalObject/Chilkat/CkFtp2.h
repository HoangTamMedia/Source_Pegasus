
#ifndef _CKFTP2_H
#define _CKFTP2_H

#pragma once

class CkByteData;

class CkCert;

#ifndef WIN32
#include "int64.h"
#endif

#include "CkString.h"
#include "CkObject.h"

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#include "FileTime.h"
#endif

#pragma warning( disable : 4068 )
#pragma unmanaged

class CkFtpProgress;

#pragma pack (push, 8)

class CkFtp2  : public CkObject
{
    public:
	CkFtp2();
	virtual ~CkFtp2();

	bool PutTree(const char *localDir, CkFtpProgress &progress);
	bool AppendFile2(const char *localFilename,  const char *remoteFilename, CkFtpProgress &progress);
	bool AppendFileFromBinaryData2(const char *remoteFilename, const CkByteData &binaryData, CkFtpProgress &progress);
	bool AppendFileFromTextData2(const char *remoteFilename, const char *textData, CkFtpProgress &progress);

	long MGetFiles2(const char *remotePattern, const char *localDir, CkFtpProgress &progress);

	long MPutFiles2(const char *pattern, CkFtpProgress &progress);

	bool GetFile2(const char *remoteFilename,  const char *localFilename, CkFtpProgress &progress);
	bool PutFile2(const char *localFilename,  const char *remoteFilename, CkFtpProgress &progress);
	bool PutFileFromBinaryData2(const char *remoteFilename, const CkByteData &binaryData, CkFtpProgress &progress);
	bool PutFileFromTextData2(const char *remoteFilename, const char *textData, CkFtpProgress &progress);

	bool GetRemoteFileBinaryData2(const char *remoteFilename, CkByteData &data, CkFtpProgress &progress);
	bool GetRemoteFileTextData2(const char *remoteFilename, CkString &str, CkFtpProgress &progress);

	void SleepMs(int millisec) const;

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool UnlockComponent(const char *code);
	bool IsUnlocked(void) const;

	long get_BandwidthThrottleUp(void);
	void put_BandwidthThrottleUp(long bytesPerSec);
	long get_BandwidthThrottleDown(void);
	void put_BandwidthThrottleDown(long bytesPerSec);

	CkFtpProgress *get_EventCallbackObject(void) const;
	void put_EventCallbackObject(CkFtpProgress *progress);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	bool get_IsConnected(void);

	bool get_PartialTransfer(void);
	bool get_LoginVerified(void);
	bool get_ConnectVerified(void);

	long DeleteMatching(const char *remotePattern);

	long MGetFiles(const char *remotePattern, const char *localDir);

	long MPutFiles(const char *pattern);

	bool PutTree(const char *localDir);

	bool GetFile(const char *remoteFilename,  const char *localFilename);
	bool PutFile(const char *localFilename,  const char *remoteFilename);

	bool PutFileFromBinaryData(const char *remoteFilename, const CkByteData &binaryData);
	bool PutFileFromTextData(const char *remoteFilename, const char *textData);

	bool GetRemoteFileBinaryData(const char *remoteFilename, CkByteData &data);
	bool GetRemoteFileTextData(const char *remoteFilename, CkString &str);
	bool GetRemoteFileTextC(const char *remoteFilename, const char *charset, CkString &str);

	bool AppendFile(const char *localFilename,  const char *remoteFilename);
	bool AppendFileFromBinaryData(const char *remoteFilename, const CkByteData &binaryData);
	bool AppendFileFromTextData(const char *remoteFilename, const char *textData);

	void get_Version(CkString &str);
	bool RenameRemoteFile(const char *existingFilename, const char *newFilename);
	bool DeleteRemoteFile(const char *filename);
	bool RemoveRemoteDir(const char *dir);
	bool CreateRemoteDir(const char *dir);

	bool Disconnect(void);
	bool Connect(void);

	bool GetCurrentRemoteDir(CkString &str);
	bool ChangeRemoteDir(const char *relativeDirPath);

	bool ClearControlChannel(void);

	void get_DirListingCharset(CkString &str);
	void put_DirListingCharset(const char *newVal);

	void get_Account(CkString &str);
	void put_Account(const char *newVal);
	const char *account(void);

	void get_ListPattern(CkString &strPattern);
	void put_ListPattern(const char *pattern);
	void get_Password(CkString &str);
	void put_Password(const char *newVal);
	void get_Username(CkString &str);
	void put_Username(const char *newVal);
	unsigned long get_AllocateSize(void);
	void put_AllocateSize(unsigned long newVal);
	long get_Port(void);
	void put_Port(long newVal);
	long get_ActivePortRangeStart(void);
	void put_ActivePortRangeStart(long newVal);
	long get_ActivePortRangeEnd(void);
	void put_ActivePortRangeEnd(long newVal);
	long get_ProxyPort(void);
	void put_ProxyPort(long newVal);
	void get_Hostname(CkString &str);
	void put_Hostname(const char *newVal);
	void get_SessionLog(CkString &str);
	bool get_Passive(void);
	void put_Passive(bool newVal);
	bool get_Ssl(void);
	void put_Ssl(bool newVal);
	bool get_AuthTls(void);
	void put_AuthTls(bool newVal);
	bool get_KeepSessionLog(void);
	void put_KeepSessionLog(bool newVal);
	bool get_PassiveUseHostAddr(void);
	void put_PassiveUseHostAddr(bool newVal);

	bool get_HasModeZ(void);

	bool get_RestartNext(void);
	void put_RestartNext(bool newVal);

	bool get_AutoXcrc(void);
	void put_AutoXcrc(bool newVal);
	bool get_AutoFeat(void);
	void put_AutoFeat(bool newVal);
	bool get_AutoSyst(void);
	void put_AutoSyst(bool newVal);
	bool Feat(CkString &sOut);
	const char *feat(void);

	bool GetSizeStr(long index, CkString &str);
	long GetSize(long index);
	bool GetFilename(long index, CkString &str);
	bool GetIsDirectory(long index);
	bool GetIsSymbolicLink(long index);

	void ClearDirCache(void);

	long GetSizeByName(const char *filname);

	bool GetCreateTime(long index, FILETIME &fTime);
	bool GetLastAccessTime(long index, FILETIME &fTime);
	bool GetLastModifiedTime(long index, FILETIME &fTime);

	bool GetCreateTime(long index, SYSTEMTIME &sysTime);
	bool GetLastAccessTime(long index, SYSTEMTIME &sysTime);
	bool GetLastModifiedTime(long index, SYSTEMTIME &sysTime);

	long get_NumFilesAndDirs(void);

	void get_ProxyHostname(CkString &str);
	void put_ProxyHostname(const char *newVal);
	void get_ProxyUsername(CkString &str);
	void put_ProxyUsername(const char *newVal);
	void get_ProxyPassword(CkString &str);
	void put_ProxyPassword(const char *newVal);
	long get_ProxyMethod(void);
	void put_ProxyMethod(long newVal);

	void ClearSessionLog(void);

	bool SetTypeBinary(void);
	bool SetTypeAscii(void);
	bool SetModeZ(void);

	bool Quote(const char *cmd);
	bool Site(const char *params);
	bool Syst(CkString &sOut);
	bool Stat(CkString &sOut);

	long get_ConnectFailReason(void);

	long get_ReadTimeout(void);
	void put_ReadTimeout(long numSeconds);

	long get_ConnectTimeout(void);
	void put_ConnectTimeout(long numSeconds);

	long get_HeartbeatMs(void);
	void put_HeartbeatMs(long millisec);

	long get_IdleTimeoutMs(void);
	void put_IdleTimeoutMs(long millisec);

	long get_CrlfMode(void);
	void put_CrlfMode(long newVal);

	long get_UploadRate(void);
	long get_DownloadRate(void);

	long get_ProgressMonSize(void);
	void put_ProgressMonSize(long newVal);

	__int64 get_ProgressMonSize64(void);
	void put_ProgressMonSize64(__int64 newVal);

	bool GetXmlDirListing(const char *pattern, CkString &strXmlListing);
	bool GetTextDirListing(const char *pattern, CkString &strRawListing);

	bool DownloadTree(const char *localRoot);
	bool DeleteTree(void);

	bool DirTreeXml(CkString &strXml);
	const char *dirTreeXml(void);

	bool SyncRemoteTree(const char *localRoot, int mode);

	bool SyncLocalTree(const char *localRoot, int mode);

	bool AsyncGetFileStart(const char *remoteFilename,  const char *localFilename);
	bool AsyncPutFileStart(const char *localFilename,  const char *remoteFilename);
	bool AsyncAppendFileStart(const char *localFilename,  const char *remoteFilename);

	void AsyncAbort(void);

	bool get_AsyncFinished(void);
	void get_AsyncLog(CkString &strLog);
	bool get_AsyncSuccess(void);

	unsigned long get_AsyncBytesReceived(void);
	unsigned long get_AsyncBytesSent(void);

	__int64 get_AsyncBytesReceived64(void);
	__int64 get_AsyncBytesSent64(void);

	bool CreatePlan(const char *localDir, CkString &str);

	bool PutPlan(const char *planUtf8, const char *planLogFilename);

	bool get_SslServerCertVerified(void);

	CkCert *GetSslServerCert(void);
	bool SetSslClientCert(CkCert &cert);
	bool SetSslClientCertPfx(const char *pfxFilename, const char *pfxPassword, const char *certSubjectCN);

	bool get_RequireSslCertVerify(void);
	void put_RequireSslCertVerify(bool newVal);
	void SetSslCertRequirement(const char *name, const char *value);

	bool get_AuthSsl(void);
	void put_AuthSsl(bool newVal);

	int DetermineProxyMethod(void);
	bool DetermineSettings(CkString &xmlReport);
	const char *determineSettings(void);

	bool SendCommand(const char *cmd, CkString &reply);
	const char *sendCommand(const char *cmd);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);

	const char *getFilename(long index);
	const char *listPattern(void);
	const char *password(void);
	const char *username(void);
	const char *hostname(void);
	const char *sessionLog(void);
	const char *getXmlDirListing(const char *pattern);
	const char *getTextDirListing(const char *pattern);
	const char *getRemoteFileTextData(const char *remoteFilename);
	const char *getRemoteFileTextC(const char *remoteFilename, const char *charset);
	const char *createPlan(const char *localDir);

	const char *version(void);
	const char *getCurrentRemoteDir(void);
	const char *getSizeStr(long index);
	const char *proxyHostname(void);
	const char *proxyUsername(void);
	const char *proxyPassword(void);
	const char *syst(void);
	const char *ck_stat(void);
	const char *asyncLog(void);
	const char *dirListingCharset(void);

	bool GetCreateTimeByName(const char *filename, SYSTEMTIME &sysTime);
	bool GetLastAccessTimeByName(const char *filename, SYSTEMTIME &sysTime);
	bool GetLastModifiedTimeByName(const char *filename, SYSTEMTIME &sysTime);

	bool Noop(void);

	void SetOldestDate(SYSTEMTIME &oldestDateTime);

	bool ConvertToTls(void);

	bool GetSizeStrByName(const char *filename, CkString &outStr);
	const char *getSizeStrByName(const char *filename);

	bool SetRemoteFileDateTime(SYSTEMTIME &dt, const char *remoteFilename);

	int get_SendBufferSize(void);
	void put_SendBufferSize(int newVal);

	bool get_UseEpsv(void);
	void put_UseEpsv(bool newVal);

	void get_ForcePortIpAddress(CkString &str);
	const char *forcePortIpAddress(void);
	void put_ForcePortIpAddress(const char *newVal);

	bool NlstXml(const char *pattern, CkString &outStr);
	const char *nlstXml(const char *pattern);

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

	void get_Greeting(CkString &str);
	const char *greeting(void);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

	void get_SslProtocol(CkString &str);
	const char *sslProtocol(void);
	void put_SslProtocol(const char *newVal);

	bool get_AutoGetSizeForProgress(void);
	void put_AutoGetSizeForProgress(bool newVal);

	void get_SyncPreview(CkString &str);
	const char *syncPreview(void);

	bool SyncRemoteTree2(const char *localRoot, int mode, bool bDescend, bool bPreviewOnly);

	bool get_AutoFix(void);
	void put_AutoFix(bool newVal);

	void get_ClientIpAddress(CkString &str);
	const char *clientIpAddress(void);
	void put_ClientIpAddress(const char *newVal);

	void TestDirListingParse(const char *textFilename);

    private:
	CkFtpProgress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkFtp2(const CkFtp2 &) { }
	CkFtp2 &operator=(const CkFtp2 &) { return *this; }
	CkFtp2(void *impl) : m_impl(impl), m_callback(0) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

};
#pragma pack (pop)

#endif
