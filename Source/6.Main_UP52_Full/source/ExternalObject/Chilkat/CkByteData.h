
#ifndef _CKBYTEDATA_H
#define _CKBYTEDATA_H

#pragma once

#include "CkObject.h"

#include "CkString.h"

#pragma pack (push, 8)

class CkByteData : public CkObject
{
    public:
	CkByteData();
	~CkByteData();

	CkByteData(const CkByteData &);
	CkByteData &operator=(const CkByteData &);

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	void clear(void);

        unsigned long getSize(void) const;

        const unsigned char *getData(void) const;
        const unsigned char *getBytes(void) const;
        const unsigned char *getDataAt(unsigned long byteIndex) const;
        const unsigned char *getRange(unsigned long byteIndex, unsigned long numBytes);
        const char *getRangeStr(unsigned long byteIndex, unsigned long numAnsiChars);

	unsigned char getByte(unsigned long byteIndex) const;
	char getChar(unsigned long byteIndex) const;
	unsigned int getUInt(unsigned long byteIndex) const;
	int getInt(unsigned long byteIndex) const;
	unsigned short getUShort(unsigned long byteIndex) const;
	short getShort(unsigned long byteIndex) const;

	void appendRandom(int numBytes);

	void appendInt(int v, bool littleEndian);
	void appendShort(short v, bool littleEndian);
	const char *getEncodedRange(const char *encoding, unsigned long index, unsigned long numBytes);
	void appendRange(const CkByteData &byteData, unsigned long index, unsigned long numBytes);
	void ensureBuffer(unsigned long numBytes);

	int findBytes2(const char *byteData, unsigned long byteDataLen);
	int findBytes(const CkByteData &byteData);
	bool beginsWith2(const char *byteData, unsigned long byteDataLen);
	bool beginsWith(const CkByteData &byteData);
	void removeChunk(unsigned long index, unsigned long numBytes);
	void byteSwap4321(void);
	void pad(int blockSize, int paddingScheme);
	void unpad(int blockSize, int paddingScheme);
	bool is7bit(void) const;
	const char *computeHash(const char *hashAlg, const char *outputEncoding);

	const char *getEncoded(const char *encoding);

	void replaceChar(unsigned char c, unsigned char replacement);

        void append(const unsigned char *data, unsigned long numBytes);
	void append(const char *byteData, unsigned long numBytes);

	void appendStr(const char *str);
	void appendChar(char ch);
	void appendCharN(char ch, int numTimes);

	void appendEncoded(const char *str, const char *encoding);
	void encode(const char *encoding, CkString &str);

        bool loadFile(const char *filename);
	bool saveFile(const char *filename);

	bool appendFile(const char *filename);

        void shorten(unsigned long numBytes);

	void borrowData(unsigned char *yourData, unsigned long numBytes);

	unsigned char *removeData(void);

	void preAllocate(unsigned long expectedNumBytes);

        const char *to_s(void);

	const char *getXmlCharset(void);

    private:

	void *m_impl;
	bool m_utf8;
	void *m_pResultString;

    public:

	void *getImpl(void) const;
	const void *getImplC(void) const;
	void setImpl(void *impl);

};

#pragma pack (pop)

#endif
