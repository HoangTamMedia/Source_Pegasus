
#ifndef _CKZIP_H
#define _CKZIP_H

#pragma once

#include "CkString.h"
class CkByteData;
class CkZipProgress;
class CkZipEntry;
class CkStringArray;
#include "CkObject.h"

#pragma pack (push, 8)

class CkZip  : public CkObject
{
    public:
	CkZip();
	virtual ~CkZip();

	bool OpenFromMemory(const char *zipFileInMemory, unsigned long dataLen);
	bool AddEmbeddedFromMemory(const char *exeFilename, const char *resourceName, const char *data, unsigned long dataLen);
	CkZipEntry *AppendData(const char *fileName, const unsigned char *data, unsigned long dataLen);

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool get_PasswordProtect(void);
	void put_PasswordProtect(bool newVal);

	long get_Encryption(void);
	void put_Encryption(long newVal);

	long get_OemCodePage(void);
	void put_OemCodePage(long newVal);

	long get_EncryptKeyLength(void);
	void put_EncryptKeyLength(long newVal);

	void get_Version(CkString &pVal);

	void get_TempDir(CkString &pVal);
	void put_TempDir(const char *newVal);

	void get_AppendFromDir(CkString &pVal);
	void put_AppendFromDir(const char *newVal);

	void get_Proxy(CkString &pVal);
	void put_Proxy(const char *newVal);

#ifdef WIN32
#ifndef WINCE

	void get_AutoRun(CkString &pVal);
	void put_AutoRun(const char *newVal);

	void get_ExeXmlConfig(CkString &pVal);
	void put_ExeXmlConfig(const char *newVal);
	const char *exeXmlConfig(void);

	bool get_AutoTemp(void);
	void put_AutoTemp(bool newVal);

	bool get_ExeNoInterface(void);
	void put_ExeNoInterface(bool newVal);

	void get_AutoRunParams(CkString &pVal);
	void put_AutoRunParams(const char *newVal);

	void get_ExeSourceUrl(CkString &pVal);
	void put_ExeSourceUrl(const char *newVal);
	const char *exeSourceUrl(void);

	void get_ExeTitle(CkString &pVal);
	void put_ExeTitle(const char *newVal);

	void get_ExeUnzipCaption(CkString &pVal);
	void put_ExeUnzipCaption(const char *newVal);

	void get_ExeUnzipDir(CkString &pVal);
	void put_ExeUnzipDir(const char *newVal);

	void get_ExeDefaultDir(CkString &pVal);
	void put_ExeDefaultDir(const char *newVal);
	const char *exeDefaultDir(void);

	void get_ExeIconFile(CkString &pVal);
	void put_ExeIconFile(const char *newVal);

	void put_ExeFinishNotifier(bool newVal);
	bool get_ExeFinishNotifier(void);

	void put_ExeSilentProgress(bool newVal);
	bool get_ExeSilentProgress(void);

#endif
#endif

	bool get_ClearArchiveAttribute(void);
	void put_ClearArchiveAttribute(bool newVal);

	long get_NumEntries(void);

	void get_FileName(CkString &pVal);
	void put_FileName(const char *newVal);

	void get_Comment(CkString &pVal);
	void put_Comment(const char *newVal);

	void get_PathPrefix(CkString &pVal);
	void put_PathPrefix(const char *newVal);

	bool get_DiscardPaths(void);
	void put_DiscardPaths(bool newVal);

	void get_DecryptPassword(CkString &pVal);
	void put_DecryptPassword(const char *newVal);
	void get_EncryptPassword(CkString &pVal);
	void put_EncryptPassword(const char *newVal);
	const char *encryptPassword(void);
	const char *decryptPassword(void);

	CkZipEntry *FirstEntry(void);

	void ExcludeDir(const char *dirName);

	bool IsPasswordProtected(const char *zipFilename);

	bool OpenFromMemory(const unsigned char *zipFileInMemory, unsigned long dataLen);

	bool OpenFromByteData(CkByteData &byteData);

#ifdef WIN32
#ifndef WINCE

	bool OpenEmbedded(const char *exeFilename, const char *resourceName);

	bool OpenMyEmbedded(const char *resourceName);

	bool AddEmbedded(const char *exeFilename, const char *resourceName, const char *zipFilename);

	bool ReplaceEmbedded(const char *exeFilename, const char *resourceName, const char *zipFilename);

	bool RemoveEmbedded(const char *exeFilename, const char *resourceName);
#endif
#endif

	void SetPassword(const char *password);

	void SetExclusions(const CkStringArray &excludePatterns) { this->SetExclusions(&excludePatterns); }

	CkStringArray *GetExclusions(void);

	CkZipEntry *FirstMatchingEntry(const char *pattern);

#ifndef WINCE
	void GetDirectoryAsXML(CkString &xml);
#endif

	bool ExtractMatching(const char *dirPath, const char *pattern, CkZipProgress *progress);

	CkZipEntry *GetEntryByID(long entryID);

	bool WriteZip(CkZipProgress *progress);

	bool WriteZipAndClose(CkZipProgress *progress);

	bool OpenZip(const char *zipFileName);

#ifndef WINCE
	bool OpenFromWeb(const char *url, CkZipProgress *progress);
#endif

	bool NewZip(const char *zipFileName);

	CkZipEntry *GetEntryByName(const char *entryName);

	CkZipEntry *GetEntryByIndex(long index);

	bool Extract(const char *dirPath, CkZipProgress *progress);

	bool ExtractNewer(const char *dirPath, CkZipProgress *progress);

	bool AppendZip(const char *zipFileName);

	bool AppendOneFileOrDir(const char *fileOrDirName, bool saveExtraPath);

	bool AppendFiles(const char *filePattern, bool recurse, CkZipProgress *progress);

	bool AppendFilesEx(const char *filePattern, bool recurse, bool saveExtraPath, bool archiveOnly, bool includeHidden, bool includeSystem, CkZipProgress *progress);

	bool AppendMultiple(const CkStringArray &fileSpecs, bool recurse, CkZipProgress *progress){return this->AppendMultiple(&fileSpecs,recurse,progress);}

	CkZipEntry *AppendData(const char *fileName, const CkByteData &data);
	CkZipEntry *AppendData(const char *fileName, const char *data, unsigned long dataLen);
	CkZipEntry *AppendString(const char *fileName, const char *str);
	CkZipEntry *AppendString2(const char *fileName, const char *str, const char *charset);

	CkZipEntry *AppendNew(const char *fileName);
	CkZipEntry *AppendNewDir(const char *dirName);

	CkZipEntry *InsertNew(const char *fileName, long beforeIndex);

	CkZipEntry *AppendCompressed(const char *fileName, const CkByteData &data);

	bool DeleteEntry(const CkZipEntry &entry) { return this->DeleteEntry(&entry); }

	CkZipEntry *AppendBase64(const char *fileName, const char *data);

	CkZipEntry *AppendHex(const char *fileName, const char *data);

#ifdef WIN32
#ifndef WINCE
	bool WriteExe(const char *filename, CkZipProgress *progress);
#endif
#endif

	bool WriteToMemory(CkByteData &bData);

	void CloseZip(void);

	bool UnlockComponent(const char *regCode);
	bool IsUnlocked(void) const;

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	int Unzip(const char *dirPath, CkZipProgress *progress);
	int UnzipNewer(const char *dirPath, CkZipProgress *progress);
	int UnzipInto(const char *dirPath, CkZipProgress *progress);
	int UnzipMatching(const char *dirPath, const char *pattern, bool verbose, CkZipProgress *progress);
	int UnzipMatchingInto(const char *dirPath, const char *pattern, bool verbose, CkZipProgress *progress);

#ifdef WIN32
#ifndef WINCE
	void SetExeConfigParam(const char *name, const char *value);
	void GetExeConfigParam(const char *name, CkString &value);
	const char *getExeConfigParam(const char *name);
#endif
#endif

	bool QuickAppend(const char *zipFilename);

	void SetCompressionLevel(int level);

#ifdef WIN32
#ifndef WINCE
	bool WriteExe2(const char *exeFilename, const char *outExeFilename, bool bAesEncrypt, int keyLength, const char *password, CkZipProgress *progress);
#endif
#endif

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);

	const char *version(void);
	const char *tempDir(void);
	const char *appendFromDir(void);
	const char *proxy(void);
	const char *autoRun(void);
	const char *autoRunParams(void);
	const char *exeTitle(void);
	const char *exeUnzipCaption(void);
	const char *exeUnzipDir(void);
	const char *exeIconFile(void);
	const char *fileName(void);
	const char *pathPrefix(void);
	const char *getDirectoryAsXML(void);

#ifdef WIN32
#ifndef WINCE
	bool get_ExeWaitForSetup(void);
	void put_ExeWaitForSetup(bool newVal);
#endif
#endif

	int get_FileCount(void);

	bool get_HasZipFormatErrors(void);

	bool get_TextFlag(void);
	void put_TextFlag(bool newVal);

	bool ExtractInto(const char *dirPath);

	bool get_CaseSensitive(void);
	void put_CaseSensitive(bool newVal);

	bool VerifyPassword(void);

#ifdef WIN32
#ifndef WINCE
	bool WriteExeToMemory(CkByteData &outBytes);
#endif
#endif

	bool get_OverwriteExisting(void);
	void put_OverwriteExisting(bool newVal);

	void AddNoCompressExtension(const char *fileExtension);

	void RemoveNoCompressExtension(const char *fileExtension);

	bool IsNoCompressExtension(const char *fileExtension);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

	bool get_IgnoreAccessDenied(void);
	void put_IgnoreAccessDenied(bool newVal);

	bool get_ClearReadOnlyAttr(void);
	void put_ClearReadOnlyAttr(bool newVal);

	bool AppendMultiple(const CkStringArray *fileSpecs, bool recurse);
#ifndef WINCE
	bool OpenFromWeb(const char *url);
#ifdef WIN32
	bool WriteExe2(const char *exeFilename, const char *outExeFilename, bool bAesEncrypt, int keyLength, const char *password);

	bool AddEmbeddedFromMemory(const char *exeFilename, const char *resourceName, const unsigned char *data, unsigned long dataLen);
#endif
#endif
	void SetExclusions(const CkStringArray *excludePatterns);
	bool AppendMultiple(const CkStringArray *fileSpecs, bool recurse, CkZipProgress *progress);
	bool DeleteEntry(const CkZipEntry *entry);
	bool ExtractMatching(const char *dirPath, const char *pattern);
	bool WriteZip(void);
	bool WriteZipAndClose(void);
	bool Extract(const char *dirPath);
	bool ExtractNewer(const char *dirPath);
	bool AppendOneFileOrDir(const char *fileOrDirName);
	bool AppendFiles(const char *filePattern, bool recurse);
	bool AppendFilesEx(const char *filePattern, bool recurse, bool saveExtraPath, bool archiveOnly, bool includeHidden, bool includeSystem);
	bool AppendMultiple(const CkStringArray &fileSpecs, bool recurse) { return this->AppendMultiple(&fileSpecs,recurse); }
	bool WriteExe(const char *filename);
	int Unzip(const char *dirPath);
	int UnzipNewer(const char *dirPath);
	int UnzipInto(const char *dirPath);
	int UnzipMatching(const char *dirPath, const char *pattern, bool verbose);
	int UnzipMatchingInto(const char *dirPath, const char *pattern, bool verbose);

    private:
	void *m_impl;
	bool m_utf8;

	CkZip(const CkZip &) { }
	CkZip &operator=(const CkZip &) { return *this; }
	CkZip(void *impl) : m_impl(impl) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

};
#pragma pack (pop)

#endif
