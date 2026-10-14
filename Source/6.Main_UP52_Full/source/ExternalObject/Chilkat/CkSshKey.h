
#ifndef _CkSshKey_H
#define _CkSshKey_H

#pragma once

#include "CkString.h"
class CkByteData;
#include "CkObject.h"

#pragma pack (push, 8)

class CkSshKey  : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkSshKey(const CkSshKey &);
	CkSshKey &operator=(const CkSshKey &);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

	CkSshKey(void *impl);

	CkSshKey();
	virtual ~CkSshKey();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
    void LastErrorXml(CkString &str);
    void LastErrorHtml(CkString &str);
    void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	bool get_IsRsaKey(void);

	bool get_IsDsaKey(void);

	bool get_IsPrivateKey(void);

	void get_Password(CkString &str);
	const char *password(void);
	void put_Password(const char *newVal);

	void get_Comment(CkString &str);
	const char *comment(void);
	void put_Comment(const char *newVal);

	bool GenFingerprint(CkString &outStr);
	const char *genFingerprint(void);

	bool ToXml(CkString &outStr);
	const char *toXml(void);

	bool ToRfc4716PublicKey(CkString &outStr);
	const char *toRfc4716PublicKey(void);

	bool ToOpenSshPublicKey(CkString &outStr);
	const char *toOpenSshPublicKey(void);

	bool FromXml(const char *xmlKey);

	bool FromRfc4716PublicKey(const char *keyStr);

	bool FromOpenSshPublicKey(const char *keyStr);

	bool FromPuttyPrivateKey(const char *keyStr);

	bool FromOpenSshPrivateKey(const char *keyStr);

	bool GenerateRsaKey(int numBits, int exponent);

	bool GenerateDsaKey(int numBits);

	bool SaveText(const char *strToSave, const char *filename);

	bool LoadText(const char *filename, CkString &outStr);
	const char *loadText(const char *filename);

	bool ToOpenSshPrivateKey(bool bEncrypt, CkString &outStr);
	const char *toOpenSshPrivateKey(bool bEncrypt);

	bool ToPuttyPrivateKey(bool bEncrypt, CkString &outStr);
	const char *toPuttyPrivateKey(bool bEncrypt);

};
#pragma pack (pop)

#endif
