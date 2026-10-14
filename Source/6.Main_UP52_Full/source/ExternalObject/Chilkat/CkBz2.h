
#ifndef _CkBz2_H
#define _CkBz2_H

#pragma once

#include "CkString.h"
class CkByteData;

#include "CkObject.h"

class CkBz2Progress;

#pragma pack (push, 8)

class CkBz2  : public CkObject
{
    private:
	CkBz2Progress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkBz2(const CkBz2 &) { }
	CkBz2 &operator=(const CkBz2 &) { return *this; }
	CkBz2(void *impl) : m_impl(impl),m_callback(0) { }

    public:
	void *getImpl(void) const { return m_impl; }

	CkBz2();
	virtual ~CkBz2();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	CkBz2Progress *get_EventCallbackObject(void) const;
	void put_EventCallbackObject(CkBz2Progress *progress);

	bool SaveLastError(const char *filename);
	void LastErrorXml(CkString &str);
	void LastErrorHtml(CkString &str);
	void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	bool UncompressFile(const char *inFilename, const char *outFilename);

	bool CompressFile(const char *inFilename, const char *outFilename);

	bool UncompressFileToMem(const char *inFilename, CkByteData &outBytes);

	bool CompressFileToMem(const char *inFilename, CkByteData &outBytes);

	bool CompressMemToFile(CkByteData &inData, const char *outFilename);

	bool UncompressMemToFile(CkByteData &inData, const char *outFilename);

	bool CompressMemory(CkByteData &inData, CkByteData &outBytes);

	bool UncompressMemory(CkByteData &inData, CkByteData &outBytes);

	bool UnlockComponent(const char *regCode);

	CkString m_resultString;

};
#pragma pack (pop)

#endif
