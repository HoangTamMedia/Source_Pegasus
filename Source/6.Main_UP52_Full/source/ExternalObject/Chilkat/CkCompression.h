
#ifndef _CKCOMPRESSION_H
#define _CKCOMPRESSION_H

#pragma once

#include "CkString.h"
class CkByteData;

#include "CkObject.h"

#pragma pack (push, 8)

class CkCompression  : public CkObject
{
    public:
	CkCompression();
	virtual ~CkCompression();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	void get_Algorithm(CkString &out);
	void put_Algorithm(const char *alg);

	bool CompressFile(const char *inFile, const char *outFile);
	bool DecompressFile(const char *inFile, const char *outFile);

	bool DecompressStringENC(const char *str, CkString &out);
	bool DecompressString(const CkByteData &bData, CkString &out);
	bool DecompressBytesENC(const char *str, CkByteData &out);
	bool DecompressBytes(const CkByteData &bData, CkByteData &out);

	bool CompressStringENC(const char *str, CkString &out);
	bool CompressBytesENC(const CkByteData &bData, CkString &out);
	bool CompressString(const char *str, CkByteData &out);
	bool CompressBytes(const CkByteData &bData, CkByteData &out);

	const char *compressStringENC(const char *str);
	const char *compressBytesENC(const CkByteData &bData);

	const char *decompressString(const CkByteData &bData);
	const char *decompressStringENC(const char *str);

	bool BeginDecompressStringENC(const char *str, CkString &out);
	bool BeginDecompressString(const CkByteData &bData, CkString &out);
	bool BeginDecompressBytesENC(const char *str, CkByteData &out);
	bool BeginDecompressBytes(const CkByteData &bData, CkByteData &out);

	bool BeginCompressStringENC(const char *str, CkString &out);
	bool BeginCompressBytesENC(const CkByteData &bData, CkString &out);
	bool BeginCompressString(const char *str, CkByteData &out);
	bool BeginCompressBytes(const CkByteData &bData, CkByteData &out);

	const char *beginCompressStringENC(const char *str);
	const char *beginCompressBytesENC(const CkByteData &bData);

	const char *beginDecompressString(const CkByteData &bData);
	const char *beginDecompressStringENC(const char *str);

	bool MoreDecompressStringENC(const char *str, CkString &out);
	bool MoreDecompressString(const CkByteData &bData, CkString &out);
	bool MoreDecompressBytesENC(const char *str, CkByteData &out);
	bool MoreDecompressBytes(const CkByteData &bData, CkByteData &out);

	bool MoreCompressStringENC(const char *str, CkString &out);
	bool MoreCompressBytesENC(const CkByteData &bData, CkString &out);
	bool MoreCompressString(const char *str, CkByteData &out);
	bool MoreCompressBytes(const CkByteData &bData, CkByteData &out);

	const char *moreCompressStringENC(const char *str);
	const char *moreCompressBytesENC(const CkByteData &bData);

	const char *moreDecompressString(const CkByteData &bData);
	const char *moreDecompressStringENC(const char *str);

	bool EndDecompressStringENC(CkString &out);
	bool EndDecompressString(CkString &out);
	bool EndDecompressBytesENC(CkByteData &out);
	bool EndDecompressBytes(CkByteData &out);

	bool EndCompressStringENC(CkString &out);
	bool EndCompressBytesENC(CkString &out);
	bool EndCompressString(CkByteData &out);
	bool EndCompressBytes(CkByteData &out);

	const char *endCompressStringENC(void);
	const char *endCompressBytesENC(void);

	const char *endDecompressString(void);
	const char *endDecompressStringENC(void);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	void get_Charset(CkString &str);
	const char *charset(void);
	void put_Charset(const char *newVal);

	void get_EncodingMode(CkString &str);
	const char *encodingMode(void);
	void put_EncodingMode(const char *newVal);

	bool UnlockComponent(const char *unlockCode);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);
	const char *algorithm(void);

	void get_Version(CkString &str);
	const char *version(void);

    private:
	void *m_impl;
	bool m_utf8;

	CkCompression(const CkCompression &);
	CkCompression &operator=(const CkCompression &);
	CkCompression(void *impl);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

};
#pragma pack (pop)

#endif
