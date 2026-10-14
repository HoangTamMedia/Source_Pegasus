
#ifndef _CkDsa_H
#define _CkDsa_H

#pragma once

#include "CkString.h"
class CkByteData;

#include "CkObject.h"

class CkDsaProgress;

#pragma pack (push, 8)

class CkDsa  : public CkObject
{
    private:
	CkDsaProgress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkDsa(const CkDsa &) { }
	CkDsa &operator=(const CkDsa &) { return *this; }
	CkDsa(void *impl) : m_impl(impl),m_callback(0) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

	CkDsa();
	virtual ~CkDsa();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	CkDsaProgress *get_EventCallbackObject(void) const;
	void put_EventCallbackObject(CkDsaProgress *progress);

	bool SaveLastError(const char *filename);
	void LastErrorXml(CkString &str);
	void LastErrorHtml(CkString &str);
	void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	int get_GroupSize(void);
	void put_GroupSize(int newVal);

	bool GenKey(int numBits);

	bool GenKeyFromParamsDerFile(const char *filename);

	bool GenKeyFromParamsPemFile(const char *filename);

	bool GenKeyFromParamsDer(CkByteData &derBytes);

	bool GenKeyFromParamsPem(const char *pem);

	bool UnlockComponent(const char *unlockCode);

	bool VerifyKey(void);

	bool ToXml(bool bPublicOnly, CkString &outStr);
	const char *toXml(bool bPublicOnly);

	bool FromXml(const char *xmlKey);

	bool FromEncryptedPem(const char *password, const char *pemData);

	bool FromPem(const char *pemData);

	bool SaveText(const char *strToSave, const char *filename);

	bool LoadText(const char *filename, CkString &outStr);
	const char *loadText(const char *filename);

	bool ToEncryptedPem(const char *password, CkString &outStr);
	const char *toEncryptedPem(const char *password);

	bool ToPem(CkString &outStr);
	const char *toPem(void);

	bool ToDer(CkByteData &outBytes);

	bool ToDerFile(const char *filename);

	bool FromDer(CkByteData &derData);

	bool FromDerFile(const char *filename);

	bool FromPublicPem(const char *pemData);

	bool ToPublicPem(CkString &outStr);
	const char *toPublicPem(void);

	void get_Hash(CkByteData &data);
	void put_Hash(CkByteData &data);

	void get_Signature(CkByteData &data);
	void put_Signature(CkByteData &data);

	bool SetEncodedHash(const char *encoding, const char *encodedHash);

	bool SetEncodedSignature(const char *encoding, const char *encodedSig);

	bool GetEncodedSignature(const char *encoding, CkString &outStr);
	const char *getEncodedSignature(const char *encoding);

	bool GetEncodedHash(const char *encoding, CkString &outStr);
	const char *getEncodedHash(const char *encoding);

	bool SignHash(void);

	bool Verify(void);

	bool SetKeyExplicit(int groupSizeInBytes, const char *pHex, const char *qHex, const char *gHex, const char *xHex);

	bool ToPublicDerFile(const char *filename);

	bool ToPublicDer(CkByteData &outBytes);

	bool FromPublicDer(CkByteData &derData);

	bool FromPublicDerFile(const char *filename);

	void get_HexP(CkString &str);
	const char *hexP(void);

	void get_HexQ(CkString &str);
	const char *hexQ(void);

	void get_HexG(CkString &str);
	const char *hexG(void);

	void get_HexX(CkString &str);
	const char *hexX(void);

	void get_HexY(CkString &str);
	const char *hexY(void);

	bool SetPubKeyExplicit(int groupSizeInBytes, const char *pHex, const char *qHex, const char *gHex, const char *yHex);

	void get_Version(CkString &str);
	const char *version(void);

};
#pragma pack (pop)

#endif
