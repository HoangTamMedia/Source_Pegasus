
#ifndef _CKCERT_H
#define _CKCERT_H

#pragma once

class CkString;

#include "CkObject.h"
#include "CkString.h"
#include "CkByteData.h"

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

class CkPublicKey;
class CkPrivateKey;

#pragma pack (push, 8)

class CkCert : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkCert(const CkCert &) { }
	CkCert &operator=(const CkCert &) { return *this; }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }
	void inject(void *impl);

	CkCert(void *impl);
	CkCert();
	virtual ~CkCert();

	bool LoadFromBinary(const char *buf, unsigned long bufLen);

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool LoadPfxFile(const char *filename, const char *password);
	bool LoadPfxData(CkByteData &pfxData, const char *password);
	bool LoadPfxData2(const unsigned char *buf, unsigned long bufLen, const char *password);

	bool LoadFromFile(const char *filename);
	bool LoadFromBase64(const char *encodedCert);
	bool LoadFromBinary(const unsigned char *buf, unsigned long bufLen);
	bool SetFromEncoded(const char *encodedCert);
	bool GetEncoded(CkString &str);
	bool get_IsRoot(void);
	void get_IssuerE(CkString &str);
	void get_IssuerC(CkString &str);
	void get_IssuerS(CkString &str);
	void get_IssuerL(CkString &str);
	void get_IssuerO(CkString &str);
	void get_IssuerOU(CkString &str);
	void get_IssuerCN(CkString &str);
	void get_IssuerDN(CkString &str);
	void get_SubjectE(CkString &str);
	void get_SubjectC(CkString &str);
	void get_SubjectS(CkString &str);
	void get_SubjectL(CkString &str);
	void get_SubjectO(CkString &str);
	void get_SubjectOU(CkString &str);
	void get_SubjectCN(CkString &str);
	void get_SubjectDN(CkString &str);
	void get_Sha1Thumbprint(CkString &str);

	unsigned long get_IntendedKeyUsage(void);

	bool get_SignatureVerified(void);
	bool get_TrustedRoot(void);
	bool get_Revoked(void);
	bool get_Expired(void);

	bool HasPrivateKey(void);

	bool SaveToFile(const char *filename);

	void get_Rfc822Name(CkString &str);

	void get_ValidTo(SYSTEMTIME &sysTime);
	void get_ValidFrom(SYSTEMTIME &sysTime);
	void get_SerialNumber(CkString &str);
	bool get_ForTimeStamping(void);
	bool get_ForCodeSigning(void);
	bool get_ForClientAuthentication(void);
	bool get_ForServerAuthentication(void);
	bool get_ForSecureEmail(void);
	void get_Version(CkString &str);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	const char *getEncoded(void);
	const char *issuerE(void);
	const char *issuerC(void);
	const char *issuerS(void);
	const char *issuerL(void);
	const char *issuerO(void);
	const char *issuerOU(void);
	const char *issuerCN(void);
	const char *issuerDN(void);
	const char *subjectE(void);
	const char *subjectC(void);
	const char *subjectS(void);
	const char *subjectL(void);
	const char *subjectO(void);
	const char *subjectOU(void);
	const char *subjectCN(void);
	const char *subjectDN(void);
	const char *sha1Thumbprint(void);
	const char *rfc822Name(void);
	const char *serialNumber(void);
	const char *version(void);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);

	bool get_HasKeyContainer(void);

	void get_KeyContainerName(CkString &str);
	const char *keyContainerName(void);
	void get_CspName(CkString &str);
	const char *cspName(void);
	bool get_MachineKeyset(void);
	bool get_Silent(void);

	bool LoadByEmailAddress(const char *emailAddress);
	bool LoadByCommonName(const char *cn);

#ifdef WIN32
	bool ExportToPfxFile(const char *pfxFilename, const char *password, bool bIncludeChain);
#endif

	bool ExportCertPem(CkString &str);
	bool ExportCertPemFile(const char *filename);
	bool ExportCertDer(CkByteData &data);
	bool ExportCertDerFile(const char *filename);

	bool PemFileToDerFile(const char *inFilename, const char *outFilename);

	CkPublicKey *ExportPublicKey(void);
	CkPrivateKey *ExportPrivateKey(void);

#ifdef WIN32
	bool LinkPrivateKey(const char *keyContainerName, bool bMachineKeyset, bool bForSigning);
#endif
	const char *exportCertPem(void);

	int CheckRevoked(void);

	bool ExportCertXml(CkString &outStr);
	const char *exportCertXml(void);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

	int get_CertVersion(void);

	bool SetPrivateKey(CkPrivateKey &privKey);

	void get_OcspUrl(CkString &str);
	const char *ocspUrl(void);

	CkCert *FindIssuer(void);

	bool LoadByIssuerAndSerialNumber(const char *issuerCN, const char *serialNum);

	bool get_SelfSigned(void);

	bool SetPrivateKeyPem(const char *privKeyPem);

	bool GetPrivateKeyPem(CkString &outStr);
	const char *getPrivateKeyPem(void);

};
#pragma pack (pop)

#endif
