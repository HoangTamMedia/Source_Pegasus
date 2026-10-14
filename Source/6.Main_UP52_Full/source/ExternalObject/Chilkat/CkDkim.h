
#ifndef _CkDkim_H
#define _CkDkim_H

#pragma once

#include "CkString.h"
class CkByteData;
class CkDkimProgress;
#include "CkObject.h"

#pragma pack (push, 8)

class CkDkim  : public CkObject
{
    private:
	CkDkimProgress *m_callback;

	void *m_impl;
	bool m_utf8;

	CkDkim(const CkDkim &);
	CkDkim &operator=(const CkDkim &);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

	CkDkim(void *impl);

	CkDkim();
	virtual ~CkDkim();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
    void LastErrorXml(CkString &str);
    void LastErrorHtml(CkString &str);
    void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	CkDkimProgress *get_EventCallbackObject(void) const;
	void put_EventCallbackObject(CkDkimProgress *progress);

	bool UnlockComponent(const char *unlockCode);

	void get_DkimCanon(CkString &str);
	const char *dkimCanon(void);
	void put_DkimCanon(const char *newVal);

	void get_DomainKeyCanon(CkString &str);
	const char *domainKeyCanon(void);
	void put_DomainKeyCanon(const char *newVal);

	void get_DkimAlg(CkString &str);
	const char *dkimAlg(void);
	void put_DkimAlg(const char *newVal);

	void get_DomainKeyAlg(CkString &str);
	const char *domainKeyAlg(void);
	void put_DomainKeyAlg(const char *newVal);

	void get_DkimHeaders(CkString &str);
	const char *dkimHeaders(void);
	void put_DkimHeaders(const char *newVal);

	void get_DomainKeyHeaders(CkString &str);
	const char *domainKeyHeaders(void);
	void put_DomainKeyHeaders(const char *newVal);

	void get_DkimDomain(CkString &str);
	const char *dkimDomain(void);
	void put_DkimDomain(const char *newVal);

	void get_DomainKeyDomain(CkString &str);
	const char *domainKeyDomain(void);
	void put_DomainKeyDomain(const char *newVal);

	void get_DkimSelector(CkString &str);
	const char *dkimSelector(void);
	void put_DkimSelector(const char *newVal);

	void get_DomainKeySelector(CkString &str);
	const char *domainKeySelector(void);
	void put_DomainKeySelector(const char *newVal);

	int get_DkimBodyLengthCount(void);
	void put_DkimBodyLengthCount(int newVal);

	bool LoadDkimPk(const char *privateKey, const char *optionalPassword);

	bool LoadDomainKeyPk(const char *privateKey, const char *optionalPassword);

	bool LoadDkimPkFile(const char *privateKeyFilepath, const char *optionalPassword);

	bool LoadDomainKeyPkFile(const char *privateKeyFilepath, const char *optionalPassword);

	bool LoadPublicKey(const char *selector, const char *domain, const char *publicKey);

	bool LoadPublicKeyFile(const char *selector, const char *domain, const char *publicKeyFilepath);

	bool PrefetchPublicKey(const char *selector, const char *domain);

	bool VerifyDkimSignature(int sigIdx, CkByteData &mimeData);

	bool VerifyDomainKeySignature(int sigIdx, CkByteData &mimeData);

	int NumDkimSignatures(CkByteData &mimeData);

	int NumDomainKeySignatures(CkByteData &mimeData);

	bool AddDkimSignature(CkByteData &mimeIn, CkByteData &outBytes);

	bool AddDomainKeySignature(CkByteData &mimeIn, CkByteData &outBytes);

	bool LoadDkimPkBytes(CkByteData &privateKeyDer, const char *optionalPassword);

	bool LoadDomainKeyPkBytes(CkByteData &privateKeyDer, const char *optionalPassword);

};
#pragma pack (pop)

#endif
