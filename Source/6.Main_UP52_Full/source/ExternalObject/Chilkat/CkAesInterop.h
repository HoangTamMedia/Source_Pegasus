
#ifndef _CKAESINTEROP_H
#define _CKAESINTEROP_H

#pragma once

class CkString;
class CkByteData;
#include "CkObject.h"

class CkAesInterop : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkAesInterop(const CkAesInterop &) { }
	CkAesInterop &operator=(const CkAesInterop &) { return *this; }

    public:
	void *getImpl(void) const { return m_impl; }

	CkAesInterop(void *impl) : m_impl(impl) { }

	CkAesInterop();
	virtual ~CkAesInterop();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	void put_Key(const unsigned char *key, long keyLen);
	void get_Key(CkByteData &key);

	void put_IV(const unsigned char *iv, long ivLen);
	void get_IV(CkByteData &iv);

	void put_KeyLength(long v);
	long get_KeyLength(void);

	void put_PassPhraseAlg(long v);
	long get_PassPhraseAlg(void);

	void put_PaddingScheme(long v);
	long get_PaddingScheme(void);

	void put_CipherMode(long v);
	long get_CipherMode(void);

	void SetModeCBC(void) { put_CipherMode(0); }
	void SetModeECB(void) { put_CipherMode(1); }

	void Decrypt(const unsigned char *eData, unsigned long eDataLen, CkByteData &data);
	void Encrypt(const unsigned char *data, unsigned long dataLen, CkByteData &eData);

	void BlowfishDecrypt(const unsigned char *eData, unsigned long eDataLen, CkByteData &data);
	void BlowfishEncrypt(const unsigned char *data, unsigned long dataLen, CkByteData &eData);

	bool DecryptFile(const char *inAnsiFilename, const char *outAnsiFilename);
	bool EncryptFile(const char *inAnsiFilename, const char *outAnsiFilename);

	void Sha1(const unsigned char *data, unsigned long dataLen, CkByteData &hash);
	void Sha256(const unsigned char *data, unsigned long dataLen, CkByteData &hash);
	void Sha384(const unsigned char *data, unsigned long dataLen, CkByteData &hash);
	void Sha512(const unsigned char *data, unsigned long dataLen, CkByteData &hash);
	void Haval(const unsigned char *data, unsigned long dataLen, int bitLength, CkByteData &hash);
	void Md5(const unsigned char *data, unsigned long dataLen, CkByteData &digest);
	void Md2(const unsigned char *data, unsigned long dataLen, CkByteData &hash);

	void put_PassPhrase(const char *p);
	void get_PassPhrase(CkString &sb);

	bool UnlockComponent(const char *unlockCode);
	bool IsUnlocked(void) const;

	void get_Version(CkString &str);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	void EncodeToHex(const unsigned char *data, unsigned long dataLen, CkString &str);
	void DecodeFromHex(const char *hexStr, CkByteData &data);

};

#endif
