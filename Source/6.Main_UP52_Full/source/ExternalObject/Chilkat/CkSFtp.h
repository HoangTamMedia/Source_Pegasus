
#ifndef _CkSFtp_H
#define _CkSFtp_H

#pragma once

#ifndef WIN32
#include "int64.h"
#endif

#include "CkString.h"
class CkByteData;
class CkSFtpProgress;
class CkSshKey;
class CkSFtpDir;
#include "CkObject.h"

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

#pragma pack (push, 8)

class CkSFtp  : public CkObject
{
    private:
	CkSFtpProgress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkSFtp(const CkSFtp &);
	CkSFtp &operator=(const CkSFtp &);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

	CkSFtp(void *impl);

	CkSFtp();
	virtual ~CkSFtp();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
    void LastErrorXml(CkString &str);
    void LastErrorHtml(CkString &str);
    void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	CkSFtpProgress *get_EventCallbackObject(void) const;
	void put_EventCallbackObject(CkSFtpProgress *progress);

	bool UnlockComponent(const char *unlockCode);

	int get_ConnectTimeoutMs(void);
	void put_ConnectTimeoutMs(int newVal);

	int get_DisconnectCode(void);

	int get_InitializeFailCode(void);

	int get_MaxPacketSize(void);
	void put_MaxPacketSize(int newVal);

	int get_IdleTimeoutMs(void);
	void put_IdleTimeoutMs(int newVal);

	void get_InitializeFailReason(CkString &str);
	const char *initializeFailReason(void);

	void get_DisconnectReason(CkString &str);
	const char *disconnectReason(void);

	void get_Version(CkString &str);
	const char *version(void);

	bool get_IsConnected(void);

	bool get_KeepSessionLog(void);
	void put_KeepSessionLog(bool newVal);

	void get_SessionLog(CkString &str);
	const char *sessionLog(void);

	void Disconnect(void);

	bool Connect(const char *hostname, int port);

	bool AuthenticatePk(const char *username, CkSshKey &privateKey);

	bool AuthenticatePw(const char *login, const char *password);

	bool InitializeSftp(void);

	bool OpenFile(const char *filename, const char *access, const char *createDisp, CkString &outStr);
	const char *openFile(const char *filename, const char *access, const char *createDisp);

	bool OpenDir(const char *path, CkString &outStr);
	const char *openDir(const char *path);

	bool CloseHandle(const char *handle);

	unsigned long GetFileSize32(const char *filenameOrHandle, bool bFollowLinks, bool bIsHandle);

	__int64 GetFileSize64(const char *filenameOrHandle, bool bFollowLinks, bool bIsHandle);

	bool GetFileSizeStr(const char *filenameOrHandle, bool bFollowLinks, bool bIsHandle, CkString &outStr);
	const char *getFileSizeStr(const char *filenameOrHandle, bool bFollowLinks, bool bIsHandle);

	bool GetFileLastAccess(const char *filenameOrHandle, bool bFollowLinks, bool bIsHandle, SYSTEMTIME &sysTime);
	bool GetFileLastModified(const char *filenameOrHandle, bool bFollowLinks, bool bIsHandle, SYSTEMTIME &sysTime);
	bool GetFileCreateTime(const char *filenameOrHandle, bool bFollowLinks, bool bIsHandle, SYSTEMTIME &sysTime);

	bool GetFileOwner(const char *filenameOrHandle, bool bFollowLinks, bool bIsHandle, CkString &outStr);
	const char *getFileOwner(const char *filenameOrHandle, bool bFollowLinks, bool bIsHandle);

	bool GetFileGroup(const char *filenameOrHandle, bool bFollowLinks, bool bIsHandle, CkString &outStr);
	const char *getFileGroup(const char *filenameOrHandle, bool bFollowLinks, bool bIsHandle);

	int GetFilePermissions(const char *filenameOrHandle, bool bFollowLinks, bool bIsHandle);

	void Add64(const char *n1, const char *n2, CkString &outStr);
	const char *add64(const char *n1, const char *n2);

	bool ReadFileBytes32(const char *handle, int offset, int numBytes, CkByteData &outBytes);

	bool ReadFileBytes64(const char *handle, __int64 offset64, int numBytes, CkByteData &outBytes);

	bool ReadFileBytes64s(const char *handle, const char *offset64, int numBytes, CkByteData &outBytes);

	bool ReadFileText64s(const char *handle, const char *offset64, int numBytes, const char *charset, CkString &outStr);
	const char *readFileText64s(const char *handle, const char *offset64, int numBytes, const char *charset);

	bool ReadFileText64(const char *handle, __int64 offset64, int numBytes, const char *charset, CkString &outStr);
	const char *readFileText64(const char *handle, __int64 offset64, int numBytes, const char *charset);

	bool ReadFileText32(const char *handle, int offset32, int numBytes, const char *charset, CkString &outStr);
	const char *readFileText32(const char *handle, int offset32, int numBytes, const char *charset);

	bool DownloadFile(const char *handle, const char *toFilename);

	bool Eof(const char *handle);

	bool ReadFileBytes(const char *handle, int numBytes, CkByteData &outBytes);

	bool ReadFileText(const char *handle, int numBytes, const char *charset, CkString &outStr);
	const char *readFileText(const char *handle, int numBytes, const char *charset);

	bool LastReadFailed(const char *handle);

	int LastReadNumBytes(const char *handle);

	bool WriteFileBytes(const char *handle, CkByteData &data);

	bool WriteFileBytes32(const char *handle, int offset, CkByteData &data);

	bool WriteFileBytes64(const char *handle, __int64 offset64, CkByteData &data);

	bool WriteFileBytes64s(const char *handle, const char *offset64, CkByteData &data);

	bool WriteFileText(const char *handle, const char *charset, const char *textData);

	bool WriteFileText32(const char *handle, int offset32, const char *charset, const char *textData);

	bool WriteFileText64(const char *handle, __int64 offset64, const char *charset, const char *textData);

	bool WriteFileText64s(const char *handle, const char *offset64, const char *charset, const char *textData);

	bool UploadFile(const char *handle, const char *fromFilename);

	bool RealPath(const char *originalPath, const char *composePath, CkString &outStr);
	const char *realPath(const char *originalPath, const char *composePath);

	CkSFtpDir *ReadDir(const char *handle);

	bool RemoveFile(const char *filename);

	bool RemoveDir(const char *path);

	bool RenameFileOrDir(const char *oldPath, const char *newPath);

	bool CreateDir(const char *path);

	bool SetCreateTime(const char *pathOrHandle, bool bIsHandle, SYSTEMTIME &createTime);
	bool SetLastModifiedTime(const char *pathOrHandle, bool bIsHandle, SYSTEMTIME &lastModTime);
	bool SetLastAccessTime(const char *pathOrHandle, bool bIsHandle, SYSTEMTIME &lastAccessTime);

	bool SetOwnerAndGroup(const char *pathOrHandle, bool bIsHandle, const char *owner, const char *group);

	bool SetPermissions(const char *pathOrHandle, bool bIsHandle, int perm);

	bool CopyFileAttr(const char *localFilename, const char *remoteFilenameOrHandle, bool bIsHandle);

	int get_ProtocolVersion(void);

	bool get_EnableCache(void);
	void put_EnableCache(bool newVal);

	void ClearCache(void);

	bool DownloadFileByName(const char *remoteFilePath, const char *localFilePath);

	int get_HeartbeatMs(void);
	void put_HeartbeatMs(int newVal);

	bool UploadFileByName(const char *remoteFilePath, const char *localFilePath);

	bool ResumeUploadFileByName(const char *remoteFilePath, const char *localFilePath);

	bool ResumeDownloadFileByName(const char *remoteFilePath, const char *localFilePath);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

	void get_FilenameCharset(CkString &str);
	const char *filenameCharset(void);
	void put_FilenameCharset(const char *newVal);

	void ClearSessionLog(void);

	bool get_ForceV3(void);
	void put_ForceV3(bool newVal);

	bool get_UtcMode(void);
	void put_UtcMode(bool newVal);

	bool get_PreserveDate(void);
	void put_PreserveDate(bool newVal);

	void get_ClientIdentifier(CkString &str);
	const char *clientIdentifier(void);
	void put_ClientIdentifier(const char *newVal);

	void get_HostKeyFingerprint(CkString &str);
	const char *hostKeyFingerprint(void);

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

	int get_HttpProxyPort(void);
	void put_HttpProxyPort(int newVal);

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

	bool get_TcpNoDelay(void);
	void put_TcpNoDelay(bool newVal);

	void get_AccumulateBuffer(CkByteData &data);

	void ClearAccumulateBuffer(void);

	int AccumulateBytes(const char *handle, int maxBytes);

	bool get_PasswordChangeRequested(void);

	bool AuthenticatePk(const char *username, const char *password, CkSshKey &privateKey);

	bool AuthenticatePwPk(const char *username, const char *password, CkSshKey &privateKey);

};
#pragma pack (pop)

#endif
