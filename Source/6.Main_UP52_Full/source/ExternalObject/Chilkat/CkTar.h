
#ifndef _CKTAR_H
#define _CKTAR_H

#pragma once

class CkByteData;
class CkTarProgress;

#include "CkString.h"
#include "CkObject.h"

#pragma warning( disable : 4068 )
#pragma unmanaged

#pragma pack (push, 8)

class CkTar  : public CkObject
{
    private:
	CkTarProgress *m_callback;

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	CkTar();
	virtual ~CkTar();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	void get_Version(CkString &str);

	CkTarProgress *get_EventCallbackObject(void) const;
	void put_EventCallbackObject(CkTarProgress *progress);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);
	const char *version(void);

	bool get_NoAbsolutePaths(void);
	void put_NoAbsolutePaths(bool newVal);

	bool get_UntarCaseSensitive(void);
	void put_UntarCaseSensitive(bool newVal);

	bool get_UntarDebugLog(void);
	void put_UntarDebugLog(bool newVal);

	bool get_UntarDiscardPaths(void);
	void put_UntarDiscardPaths(bool newVal);

	void get_UntarFromDir(CkString &str);
	const char *untarFromDir(void);
	void put_UntarFromDir(const char *newVal);

	void get_UntarMatchPattern(CkString &str);
	const char *untarMatchPattern(void);
	void put_UntarMatchPattern(const char *newVal);

	int get_UntarMaxCount(void);
	void put_UntarMaxCount(int newVal);

	bool UnlockComponent(const char *unlockCode);

	int Untar(const char *tarFilename);

	int UntarFromMemory(CkByteData &tarFileBytes);

	bool UntarFirstMatchingToMemory(CkByteData &tarFileBytes, const char *matchPattern, CkByteData &outBytes);

	bool AddDirRoot(const char *dirPath);

	int get_NumDirRoots(void);

	bool GetDirRoot(int index, CkString &outStr);
	const char *getDirRoot(int index);

	bool WriteTar(const char *tarFilename);

	bool WriteTarGz(const char *outFilename);

	bool WriteTarBz2(const char *outFilename);

	bool WriteTarZ(const char *outFilename);

	bool ListXml(const char *tarFilename, CkString &outStr);
	const char *listXml(const char *tarFilename);

	void get_Charset(CkString &str);
	const char *charset(void);
	void put_Charset(const char *newVal);

	bool UntarZ(const char *tarFilename);

	bool UntarGz(const char *tarFilename);

	bool UntarBz2(const char *tarFilename);

	void get_UserName(CkString &str);
	const char *userName(void);
	void put_UserName(const char *newVal);

	void get_GroupName(CkString &str);
	const char *groupName(void);
	void put_GroupName(const char *newVal);

	int get_UserId(void);
	void put_UserId(int newVal);

	int get_GroupId(void);
	void put_GroupId(int newVal);

	int get_DirMode(void);
	void put_DirMode(int newVal);

	int get_FileMode(void);
	void put_FileMode(int newVal);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

	void get_WriteFormat(CkString &str);
	const char *writeFormat(void);
	void put_WriteFormat(const char *newVal);

	int get_HeartbeatMs(void);
	void put_HeartbeatMs(int newVal);

    private:
	void *m_impl;
	bool m_utf8;

	CkTar(const CkTar &) { }
	CkTar &operator=(const CkTar &) { return *this; }
	CkTar(void *impl);

    public:
	void *getImpl(void) const { return m_impl; }

};

#pragma pack (pop)

#endif
