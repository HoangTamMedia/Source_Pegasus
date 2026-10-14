
#if !defined(_CK_GZIP_H)
#define _CK_GZIP_H

#pragma once

#include "CkObject.h"

#include "CkString.h"
class CkByteData;

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

class CkGzipProgress;

#pragma pack (push, 8)

class CkGzip  : public CkObject
{
public:
    private:
	CkGzipProgress *m_callback;
	void *m_impl;
	bool m_utf8;

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

	CkGzip(const CkGzip &) { }
	CkGzip &operator=(const CkGzip &) { return *this; }
	CkGzip(void *impl);

    public:
	void *getImpl(void) const { return m_impl; }

	CkGzipProgress *get_EventCallbackObject(void) const { return m_callback; }
	void put_EventCallbackObject(CkGzipProgress *progress) { m_callback = progress; }

	CkGzip();
	virtual ~CkGzip();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	bool UnlockComponent(const char *unlockCode);
	bool IsUnlocked(void) const;

	bool CompressFile(const char *inFilename, const char *outFilename);
	bool UncompressFile(const char *inFilename, const char *outFilename);

	bool CompressFileToMem(const char *inFilename, CkByteData &db);
	bool UncompressFileToMem(const char *inFilename, CkByteData &db);

	bool CompressMemToFile(const CkByteData &db, const char *outFilename);
	bool UncompressMemToFile(const CkByteData &db, const char *outFilename);

	bool CompressMemory(const CkByteData &dbIn, CkByteData &dbOut);
	bool UncompressMemory(const CkByteData &dbIn, CkByteData &dbOut);

	void DeflateStringENC(const char *str, const char *charset, const char *encoding, CkString &strOut);
	void InflateStringENC(const char *str, const char *charset, const char *encoding, CkString &strOut);
	const char *deflateStringENC(const char *str, const char *charset, const char *encoding);
	const char *inflateStringENC(const char *str, const char *charset, const char *encoding);

	void get_Filename(CkString &str);
	void put_Filename(const char *str);

	void get_Comment(CkString &str);
	void put_Comment(const char *str);

	void get_ExtraData(CkByteData &data);
	void put_ExtraData(const CkByteData &data);

	void get_LastMod(SYSTEMTIME &sysTime);
	void put_LastMod(SYSTEMTIME &sysTime);

	bool UnTarGz(const char *gzFilename, const char *destDir, bool bNoAbsolute);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);
	const char *comment(void);
	const char *filename(void);

	void get_Version(CkString &str);
	const char *version(void);

	bool ReadFile(const char *filename, CkByteData &outBytes);

	bool WriteFile(const char *filename, CkByteData &binaryData);

	bool CompressFile2(const char *inFilename, const char *embeddedFilename, const char *outFilename);

	bool CompressString(const char *inStr, const char *outCharset, CkByteData &outBytes);

	bool CompressStringToFile(const char *inStr, const char *outCharset, const char *outFilename);

	bool UncompressFileToString(const char *inFilename, const char *inCharset, CkString &outStr);
	const char *uncompressFileToString(const char *inFilename, const char *inCharset);

	bool UncompressString(CkByteData &inData, const char *inCharset, CkString &outStr);
	const char *uncompressString(CkByteData &inData, const char *inCharset);

	bool ExamineFile(const char *inGzFilename);

	bool ExamineMemory(CkByteData &inGzData);

	bool Decode(const char *str, const char *encoding, CkByteData &outBytes);

	bool get_UseCurrentDate(void);
	void put_UseCurrentDate(bool newVal);

	bool XfdlToXml(const char *xfdl, CkString &outStr);
	const char *xfdlToXml(const char *xfdl);

	bool Encode(CkByteData &byteData, const char *encoding, CkString &outStr);
	const char *encode(CkByteData &byteData, const char *encoding);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

};
#pragma pack (pop)

#endif
