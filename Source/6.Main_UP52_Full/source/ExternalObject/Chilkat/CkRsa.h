
#ifndef _CkRsa_H
#define _CkRsa_H

#pragma once

#include "CkString.h"
class CkByteData;
#include "CkObject.h"

#pragma pack (push, 8)

class CkRsa  : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkRsa(const CkRsa &);
	CkRsa &operator=(const CkRsa &);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

	CkRsa(void *impl);

	CkRsa();
	virtual ~CkRsa();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool UnlockComponent(const char *unlockCode);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	bool DecryptStringENC(const char *str, bool bUsePrivateKey, CkString &out);
	bool DecryptString(const CkByteData &bData, bool bUsePrivateKey, CkString &out);
	bool DecryptBytesENC(const char *str, bool bUsePrivateKey, CkByteData &out);
	bool DecryptBytes(const CkByteData &bData, bool bUsePrivateKey, CkByteData &out);

	bool EncryptStringENC(const char *str, bool bUsePrivateKey, CkString &out);
	bool EncryptBytesENC(const CkByteData &bData, bool bUsePrivateKey, CkString &out);
	bool EncryptString(const char *str, bool bUsePrivateKey, CkByteData &out);
	bool EncryptBytes(const CkByteData &bData, bool bUsePrivateKey, CkByteData &out);

	void get_EncodingMode(CkString &out);
	void put_EncodingMode(const char *str);
	void get_Charset(CkString &out);
	void put_Charset(const char *str);

	bool GenerateKey(int numBits);

	bool ExportPublicKey(CkString &strXml);
	bool ExportPrivateKey(CkString &strXml);

	bool ImportPublicKey(const char *strXml);
	bool ImportPrivateKey(const char *strXml);

	bool SnkToXml(const char *filename, CkString &strXml);

	long get_NumBits(void);

	bool get_OaepPadding(void);
	void put_OaepPadding(bool newVal);

	bool get_LittleEndian(void);
	void put_LittleEndian(bool newVal);

	bool VerifyStringENC(const char *str, const char *hashAlg, const char *sig);
	bool VerifyString(const char *str, const char *hashAlg, const CkByteData &sigData);
	bool VerifyBytesENC(const CkByteData &bData, const char *hashAlg, const char *encodedSig);
	bool VerifyBytes(const CkByteData &bData, const char *hashAlg, const CkByteData &sigData);

	const char *signStringENC(const char *str, const char *hashAlg);
	const char *signBytesENC(const CkByteData &bData, const char *hashAlg);

	bool SignStringENC(const char *str, const char *hashAlg, CkString &out);
	bool SignBytesENC(const CkByteData &bData, const char *hashAlg, CkString &out);
	bool SignString(const char *str, const char *hashAlg, CkByteData &out);
	bool SignBytes(const CkByteData &bData, const char *hashAlg, CkByteData &out);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);

	const char *decryptStringENC(const char *str, bool bUsePrivateKey);
	const char *decryptString(const CkByteData &bData, bool bUsePrivateKey);
	const char *encryptStringENC(const char *str, bool bUsePrivateKey);
	const char *encryptBytesENC(const CkByteData &bData, bool bUsePrivateKey);
	const char *encodingMode(void);
	const char *charset(void);
	const char *exportPublicKey(void);
	const char *exportPrivateKey(void);
	const char *snkToXml(const char *filename);

	void get_Version(CkString &str);
	const char *version(void);

	bool OpenSslVerifyBytes(CkByteData &signature, CkByteData &outBytes);

	bool OpenSslSignBytes(CkByteData &data, CkByteData &outBytes);

	bool OpenSslSignBytesENC(CkByteData &data, CkString &outStr);
	const char *openSslSignBytesENC(CkByteData &data);

	bool OpenSslSignString(const char *str, CkByteData &outBytes);

	bool OpenSslSignStringENC(const char *str, CkString &outStr);
	const char *openSslSignStringENC(const char *str);

	bool OpenSslVerifyBytesENC(const char *str, CkByteData &outBytes);

	bool OpenSslVerifyString(CkByteData &data, CkString &outStr);
	const char *openSslVerifyString(CkByteData &data);

	bool OpenSslVerifyStringENC(const char *str, CkString &outStr);
	const char *openSslVerifyStringENC(const char *str);

	bool VerifyPrivateKey(const char *xml);

	bool VerifyHash(CkByteData &hashBytes, const char *hashAlg, CkByteData &sigBytes);

	bool VerifyHashENC(const char *encodedHash, const char *hashAlg, const char *encodedSig);

	bool SignHash(CkByteData &hashBytes, const char *hashAlg, CkByteData &outBytes);

	bool SignHashENC(const char *encodedHash, const char *hashAlg, CkString &outStr);
	const char *signHashENC(const char *encodedHash, const char *hashAlg);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

	bool get_NoUnpad(void);
	void put_NoUnpad(bool newVal);

};
#pragma pack (pop)

#endif
