
#ifndef _CKFILEACCESS_H
#define _CKFILEACCESS_H

#pragma once

class CkByteData;
#include "CkString.h"
#include "CkObject.h"

#pragma warning( disable : 4068 )
#pragma unmanaged

#pragma pack (push, 8)

class CkFileAccess  : public CkObject
{
    public:
	CkFileAccess();
	virtual ~CkFileAccess();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);

        bool ReadEntireTextFile(const char *filename, const char *charset, CkString &strFileContents);

        const char *readEntireTextFile(const char *filename, const char *charset);

	void get_CurrentDir(CkString &str);
	const char *currentDir(void);

	bool get_EndOfFile(void);

	bool SetCurrentDir(const char *path);

	bool AppendAnsi(const char *text);

	bool AppendText(const char *text, const char *charset);

	bool AppendUnicodeBOM(void);

	bool AppendUtf8BOM(void);

	bool DirAutoCreate(const char *path);

	bool DirCreate(const char *path);

	bool DirDelete(const char *path);

	bool DirEnsureExists(const char *filePath);

	void FileClose(void);

	bool FileCopy(const char *existing, const char *newFilename, bool failIfExists);

	bool FileDelete(const char *filename);

	bool FileExists(const char *filename);

	bool FileOpen(const char *filename, int accessMode, int shareMode, int createDisp, int attr);

	bool FileRead(int numBytes, CkByteData &outBytes);

	bool FileRename(const char *existing, const char *newFilename);

	bool FileSeek(int offset, int origin);

	int FileSize(const char *filename);

	bool FileWrite(CkByteData &data);

	bool GetTempFilename(const char *dirName, const char *prefix, CkString &outStr);
	const char *getTempFilename(const char *dirName, const char *prefix);

	bool ReadEntireFile(const char *filename, CkByteData &outBytes);

	bool TreeDelete(const char *path);

	bool WriteEntireFile(const char *filename, CkByteData &fileData);

	bool WriteEntireTextFile(const char *filename, const char *fileData, const char *charset, bool includePreamble);

	bool SplitFile(const char *fileToSplit, const char *partPrefix, const char *partExtension, int partSize, const char *outputDirPath);

	bool ReassembleFile(const char *partsDirPath, const char *partPrefix, const char *partExtension, const char *reassembledFilename);

	bool ReadBinaryToEncoded(const char *filename, const char *encoding, CkString &outStr);
	const char *readBinaryToEncoded(const char *filename, const char *encoding);

    private:
	void *m_impl;
	bool m_utf8;

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

	CkFileAccess(const CkFileAccess &) { }
	CkFileAccess &operator=(const CkFileAccess &) { return *this; }
	CkFileAccess(void *impl);

    public:
	void *getImpl(void) const { return m_impl; }

};

#pragma pack (pop)

#endif
