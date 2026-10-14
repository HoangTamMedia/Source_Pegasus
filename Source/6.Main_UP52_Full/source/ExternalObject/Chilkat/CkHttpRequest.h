
#ifndef _CKHTTPREQUEST_H
#define _CKHTTPREQUEST_H

#pragma once

#include "CkString.h"
class CkByteData;

#include "CkObject.h"

#pragma pack (push, 8)

class CkHttpRequest  : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkHttpRequest(const CkHttpRequest &) { }
	CkHttpRequest &operator=(const CkHttpRequest &) { return *this; }
	CkHttpRequest(void *impl) : m_impl(impl) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

	CkHttpRequest();
	virtual ~CkHttpRequest();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	void UsePost(void);
	void UseGet(void);
	void UseHead(void);
	void UseXmlHttp(const char *xmlBody);
	void UseUpload(void);

	void SetFromUrl(const char *url);

	void put_Path(const char *path);
	void get_Path(CkString &str);

	void put_Charset(const char *charset);
	void get_Charset(CkString &str);

	void put_SendCharset(bool val);
	bool get_SendCharset(void);

	void put_HttpVersion(const char *version);
	void get_HttpVersion(CkString &str);

	void AddHeader(const char *name, const char *value);

	void AddParam(const char *name, const char *value);
	void RemoveParam(const char *name);

	bool AddFileForUpload(const char *name, const char *filename);

	void GenerateRequestText(CkString &str);

	bool GetHeaderField(const char *name, CkString &str);
	int get_NumHeaderFields(void);
	bool GetHeaderName(int index, CkString &str);
	bool GetHeaderValue(int index, CkString &str);

	bool GetParam(const char *name, CkString &str);
	int get_NumParams(void);
	bool GetParamName(int index, CkString &str);
	bool GetParamValue(int index, CkString &str);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	const char *path(void);
	const char *charset(void);
	const char *httpVersion(void);
	const char *getHeaderField(const char *name);
	const char *getHeaderName(int index);
	const char *getHeaderValue(int index);
	const char *getParam(const char *name);
	const char *getParamName(int index);
	const char *getParamValue(int index);
        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);
	const char *generateRequestText(void);

	void RemoveAllParams(void);

	bool LoadBodyFromFile(const char *filename);

	bool LoadBodyFromBytes(CkByteData &binaryData);

	bool LoadBodyFromString(const char *bodyStr, const char *charset);

	void UsePut(void);

	bool AddBytesForUpload(const char *name, const char *filename, CkByteData &byteData);

	void UsePostMultipartForm(void);

	bool GetUrlEncodedParams(CkString &outStr);
	const char *getUrlEncodedParams(void);

	bool AddStringForUpload(const char *name, const char *filename, const char *strData, const char *charset);

	bool AddFileForUpload2(const char *name, const char *filename, const char *contentType);

	bool AddStringForUpload2(const char *name, const char *filename, const char *strData, const char *charset, const char *contentType);

	bool AddBytesForUpload2(const char *name, const char *filename, CkByteData &byteData, const char *contentType);

};
#pragma pack (pop)

#endif
