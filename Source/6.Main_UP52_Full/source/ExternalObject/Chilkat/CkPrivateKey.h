
#ifndef _CKPRIVATEKEY_H
#define _CKPRIVATEKEY_H

#pragma once

class CkString;

#include "CkObject.h"
#include "CkString.h"
#include "CkByteData.h"

#pragma pack (push, 8)

class CkPrivateKey : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkPrivateKey(const CkPrivateKey &) { }
	CkPrivateKey &operator=(const CkPrivateKey &) { return *this; }

    public:
	void *getImpl(void) const { return m_impl; }

	CkPrivateKey();
	CkPrivateKey(void *impl);
	virtual ~CkPrivateKey();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool LoadRsaDerFile(const char *filename);
	bool LoadPkcs8File(const char *filename);
	bool LoadPemFile(const char *filename);
	bool LoadXmlFile(const char *filename);

	bool LoadPem(const char *str);
	bool LoadRsaDer(CkByteData &data);
	bool LoadPkcs8(CkByteData &data);
	bool LoadXml(const char *xml);

#ifdef WIN32
	bool LoadPvk(CkByteData &data, const char *password);
	bool LoadPvkFile(const char *filename, const char *password);
#endif

	bool SaveRsaDerFile(const char *filename);
	bool SavePkcs8File(const char *filename);
	bool SaveRsaPemFile(const char *filename);
	bool SavePkcs8PemFile(const char *filename);
	bool SaveXmlFile(const char *filename);

	bool GetRsaDer(CkByteData &data);
	bool GetPkcs8(CkByteData &data);
	bool GetRsaPem(CkString &str);
	bool GetPkcs8Pem(CkString &str);
	bool GetXml(CkString &str);

	bool SaveLastError(const char *filename);
	void LastErrorXml(CkString &str);
	void LastErrorHtml(CkString &str);
	void LastErrorText(CkString &str);

	CkString m_resultString;

	const char *lastErrorText(void);
	const char *lastErrorXml(void);
	const char *lastErrorHtml(void);

	const char *getRsaPem(void);
	const char *getPkcs8Pem(void);
	const char *getXml(void);

	bool LoadEncryptedPem(const char *pemStr, const char *password);

	bool LoadEncryptedPemFile(const char *filename, const char *password);

	bool LoadPkcs8Encrypted(CkByteData &data, const char *password);

	bool LoadPkcs8EncryptedFile(const char *filename, const char *password);

	bool GetPkcs8Encrypted(const char *password, CkByteData &outBytes);

	bool GetPkcs8EncryptedPem(const char *password, CkString &outStr);
	const char *getPkcs8EncryptedPem(const char *password);

	bool SavePkcs8EncryptedFile(const char *password, const char *filename);

	bool SavePkcs8EncryptedPemFile(const char *password, const char *filename);

};
#pragma pack (pop)

#endif
