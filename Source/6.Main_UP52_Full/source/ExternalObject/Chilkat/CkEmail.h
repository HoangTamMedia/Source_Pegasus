
#ifndef _CKEMAIL_H
#define _CKEMAIL_H

#pragma once

#include "CkString.h"
#ifdef WIN32
class CkCSP;
#endif
class CkCert;
class CkByteData;
class CkMime;
class CkStringArray;
class CkSpamDefs;
class CkPrivateKey;

#include "CkObject.h"

#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"
#endif

#pragma pack (push, 8)

class CkEmail : public CkObject
{
    public:

	CkEmail();
	virtual ~CkEmail();

	void AddRelatedData2(const char *data, unsigned long dataLen, const char *fileNameInHtml);
	bool AddRelatedData(const char *nameInHtml, const char *data, unsigned long dataLen, CkString *strContentId);
	bool AddDataAttachment(const char *fileName, const char *data, unsigned long dataLen);
    	bool SetMbPlainTextBody(const char *charset, const char *data, unsigned long dataLen);
    	bool SetMbHtmlBody(const char *charset, const char *data, unsigned long dataLen);
	bool QEncodeBytes(const char *data, unsigned long dataLen, const char *charset, CkString &encodedStr);
	bool BEncodeBytes(const char *data, unsigned long dataLen, const char *charset, CkString &encodedStr);
	const char *qEncodeBytes(const char *data, unsigned long dataLen, const char *charset);
	const char *bEncodeBytes(const char *data, unsigned long dataLen, const char *charset);
	bool UidlEquals(const CkEmail *e);

	bool SetFromMimeText(const char *mimeText);
	bool SetFromMimeText2(const char *mimeText, int numBytes);
	bool LoadEml(const char *mimeFilename);
	bool LoadXml(const char *xmlFilename);
	bool SetFromXmlText(const char *xmlStr);

	bool ZipAttachments(const char *zipFilename);

	bool UnzipAttachments();

	bool AesEncrypt(const char *password);

	bool AesDecrypt(const char *password);

	CkEmail *Clone(void);

	CkEmail *CreateForward(void);

	CkEmail *CreateReply(void);

#ifdef WIN32
#ifndef NO_MS_CRYPTO
	bool SetCSP(const CkCSP *csp);
	bool SetCSP(const CkCSP &csp) { return SetCSP(&csp); }
#endif
#endif

    	CkCert *GetSignedByCert(void);

	CkCert *GetEncryptedByCert(void);

	CkCert *GetEncryptCert(void);

	CkCert *GetSigningCert(void);

	bool SetEncryptCert(const CkCert *cert);
	bool SetEncryptCert(const CkCert &cert) { return SetEncryptCert(&cert); }

	bool SetSigningCert(const CkCert *cert);
	bool SetSigningCert(const CkCert &cert) { return SetSigningCert(&cert); }
	bool SetSigningCert2(const CkCert &cert, CkPrivateKey &key);

	bool GetFileContent(const char *filename, CkByteData &bData);
	CkMime **Decompose(int &numParts);

	bool GetRelatedStringCrLf(long index, const char *charset, CkString &str);

	bool GetRelatedContentID(long index, CkString &strContentID);

	bool GetRelatedContentType(long index, CkString &strContentType);

	bool GetRelatedFilename(long index, CkString &strFilename);

	bool GetRelatedString(long index, const char *charset, CkString &strData);

	bool GetRelatedData(long index, CkByteData &buffer);

	void AddHeaderField(const char *fieldName, const char *fieldValue);

	void RemoveHeaderField(const char *fieldName);

	void SetHtmlBody(const char *html);

	bool SaveRelatedItem(long index, const char *directory);

	void DropRelatedItem(long index);
	void DropRelatedItems(void);

    	bool AddRelatedFile(const char *fileName, CkString *strContentID);
	bool AddRelatedFile(const char *fileName, CkString &strContentID) { return AddRelatedFile(fileName,&strContentID); }

    	bool AddRelatedFile2(const char *fileNameOnDisk, const char *filenameInHtml);
	void AddRelatedData2(const unsigned char *data, unsigned long dataLen, const char *fileNameInHtml);
	void AddRelatedString2(const char *str, const char *charset, const char *filenameInHtml);

	bool AddRelatedData(const char *nameInHtml, const unsigned char *data, unsigned long dataLen, CkString *strContentId);
	bool AddRelatedData(const char *nameInHtml, const unsigned char *data, unsigned long dataLen, CkString &strContentId){return AddRelatedData(nameInHtml,data,dataLen,&strContentId);}

	bool AddHtmlAlternativeBody(const char *body);

	bool AddPlainTextAlternativeBody(const char *body);

	bool GetAlternativeBody(long index, CkString &strBody);

	bool GetAlternativeContentType( long index,  CkString &strContentType);

	bool GetHtmlBody(CkString &strBody);

	bool GetPlainTextBody(CkString &strBody);

	void SetTextBody(const char *bodyText, const char *contentType);

	long get_NumHeaderFields(void);
	bool GetHeaderFieldName(long index, CkString &strFieldName);
	bool GetHeaderFieldValue(long index, CkString &strFieldValue);

	bool GetAttachmentStringCrLf(long index, const char *charset, CkString &strData);

	bool GetAttachmentContentID(long index, CkString &strContentID);

	bool GetAttachmentContentType(long index, CkString &strContentType);

	bool GetAttachmentHeader(long index, const char *fieldName, CkString &fieldValue);

	long GetAttachmentSize(long index);

	bool GetAttachmentString(long index, const char *charset, CkString &str);

	bool SaveAllAttachments(const char *directory);

	bool SaveAttachedFile(long index, const char *directory);

	bool GetAttachmentData(long index, CkByteData &buffer);

	bool DropSingleAttachment(long index);

	bool SetAttachmentFilename(long index, const char *filename);

	bool GetAttachmentFilename(long index, CkString &strFilename);

	void AddAttachmentHeader(int index, const char *fieldName, const char *fieldValue);

	void DropAttachments(void);

	bool AddFileAttachment(const char *fileName, CkString *strContentType);
	bool AddFileAttachment(const char *fileName, CkString &strContentType){return AddFileAttachment(fileName,&strContentType);}

	bool AddFileAttachment2(const char *fileName, const char *contentType);

	bool AddDataAttachment(const char *fileName, const unsigned char *data, unsigned long dataLen);

	bool AddStringAttachment(const char *fileName, const char *str);
	bool AddStringAttachment2(const char *fileName, const char *str, const char *charset);
	bool AddRelatedString(const char *nameInHtml, const char *str, const char *charset, CkString *cid);
	bool AddRelatedString(const char *nameInHtml, const char *str, const char *charset, CkString &cid){return AddRelatedString(nameInHtml,str,charset,&cid);}

	long get_NumReplacePatterns(void);

	bool GetReplaceString2(const char *pattern, CkString &str);

	bool GetReplaceString(long index, CkString &str);

	bool GetReplacePattern(long index, CkString &strPattern);

	bool SetReplacePattern(const char *pattern, const char *replaceString);

	void GenerateFilename(CkString &strFilename);

	bool AddMultipleBcc(const char *commaSeparatedAddresses);
	bool AddMultipleCC(const char *commaSeparatedAddresses);
	bool AddMultipleTo(const char *commaSeparatedAddresses);

	bool GetHeaderField(const char *fieldName, CkString &strFieldData);

	bool GetBcc(long index, CkString &str);
	bool GetCC(long index, CkString &str);
	bool GetTo(long index, CkString &str);

	bool GetBccAddr(long index, CkString &str);
	bool GetCcAddr(long index, CkString &str);
	bool GetToAddr(long index, CkString &str);

	bool GetBccName(long index, CkString &str);
	bool GetCcName(long index, CkString &str);
	bool GetToName(long index, CkString &str);

	void ClearBcc(void);
	void ClearCC(void);
	void ClearTo(void);

	bool AddBcc(const char *friendlyName, const char *emailAddress);

	bool AddCC(const char *friendlyName, const char *emailAddress);

	bool AddTo(const char *friendlyName, const char *emailAddress);

	void GetMime(CkString &strMime);

	void GetXml(CkString &strXml);

	bool SaveXml(const char *filename);

	bool SaveEml(const char *filename);

	void get_Uidl(CkString &str);

	bool get_ReturnReceipt(void);
	void put_ReturnReceipt(bool newVal);

	long get_Size(void);

	long get_NumAlternatives(void);

	long get_NumRelatedItems(void);

	bool get_SendEncrypted(void);
	void put_SendEncrypted(bool newVal);

	void get_FileDistList(CkString &str);
	void put_FileDistList(const char *str);

	void get_Charset(CkString &str);
	void put_Charset(const char *str);

	bool get_OverwriteExisting(void);
	void put_OverwriteExisting(bool newVal);

	bool get_SendSigned(void);
	void put_SendSigned(bool newVal);

	void get_EncryptedBy(CkString &str);

	bool get_Decrypted(void);

	bool get_SignaturesValid(void);

	void get_SignedBy(CkString &str);

	bool get_ReceivedSigned(void);

	bool get_ReceivedEncrypted(void);

	long get_NumAttachments(void) const;

	void get_FromAddress(CkString &str);
	void put_FromAddress(const char *str);

	void get_FromName(CkString &str);
	void put_FromName(const char *str);

	void get_LocalDate(SYSTEMTIME &sysTime);

	void get_EmailDate(SYSTEMTIME &sysTime);

	void put_LocalDate(SYSTEMTIME &sysTime);

	void put_EmailDate(SYSTEMTIME &sysTime);

	void get_Mailer(CkString &str);
	void put_Mailer(const char *str);

	void get_Header(CkString &str);

	long get_NumBcc(void);
	long get_NumCC(void);
	long get_NumTo(void);

	void get_From(CkString &str);
	void put_From(const char *str);

	void get_Subject(CkString &str);
	void put_Subject(const char *str);

	void get_ReplyTo(CkString &str);
	void put_ReplyTo(const char *str);

	void get_BounceAddress(CkString &str);
	void put_BounceAddress(const char *str);

	void get_Body(CkString &str);
	void put_Body(const char *str);

	bool UidlEquals(const CkEmail &e) { return UidlEquals(&e); }

	bool CreateTempMht(const char *inFilename, CkString &tempMhtFilenameInOut);
	CkMime *GetMimeObject(void);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

    	bool SetMbPlainTextBody(const char *charset, const unsigned char *data, unsigned long dataLen);

    	bool SetMbHtmlBody(const char *charset, const unsigned char *data, unsigned long dataLen);

	bool GetMbPlainTextBody(const char *charset, CkByteData &data);

	bool GetMbHtmlBody(const char *charset, CkByteData &data);

	bool GetMbHeaderField(const char *fieldName, CkByteData &fieldData);

	bool GetMbHeaderField2(const char *charset, const char *fieldName, CkByteData &fieldData);

	bool QEncodeBytes(const unsigned char *data, unsigned long dataLen, const char *charset, CkString &encodedStr);

	bool BEncodeBytes(const unsigned char *data, unsigned long dataLen, const char *charset, CkString &encodedStr);

	bool QEncodeString(const char *str, const char *charset, CkString &encodedStr);

	bool BEncodeString(const char *str, const char *charset, CkString &encodedStr);

	bool UnlockComponent(const char *unlockCode);

	long get_NumAttachedMessages(void);

	CkEmail *GetAttachedMessage(long index);

	void RemoveAttachedMessages(void);

	void RemoveAttachedMessage(long index);

	void GetLinkedDomains(CkStringArray &array);

	void UnSpamify(void);

	bool IsMultipartReport(void);

	bool GetDeliveryStatusInfo(const char *fieldName, CkString &fieldValue);
	const char *getDeliveryStatusInfo(const char *fieldName);

	bool HasHtmlBody(void);

	bool HasPlainTextBody(void);

	void RemoveAttachmentPaths(void);

	bool UnpackHtml(const char *unpackDir, const char *htmlFilename, const char *partsSubdir);

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);

	const char *getRelatedStringCrLf(long index, const char *charset);
	const char *getRelatedContentID(long index);
	const char *getRelatedContentType(long index);
	const char *getRelatedFilename(long index);
	const char *getRelatedString(long index, const char *charset);
	const char *getAlternativeBody(long index);
	const char *getAlternativeContentType(long index);
	const char *getHtmlBody(void);
	const char *getPlainTextBody(void);
	const char *getHeaderFieldName(long index);
	const char *getHeaderFieldValue(long index);
	const char *getAttachmentStringCrLf(long index, const char *charset);
	const char *getAttachmentContentID(long index);
	const char *getAttachmentContentType(long index);
	const char *getAttachmentHeader(long index, const char *fieldName);
	const char *getAttachmentString(long index, const char *charset);
	const char *getAttachmentFilename(long index);
	const char *getHeaderField(const char *fieldName);
	const char *getBcc(long index);
	const char *getCC(long index);
	const char *getTo(long index);
	const char *getBccAddr(long index);
	const char *getCcAddr(long index);
	const char *getToAddr(long index);
	const char *getBccName(long index);
	const char *getCcName(long index);
	const char *getToName(long index);
	const char *getMime(void);
	const char *getXml(void);
	const char *uidl(void);
	const char *charset(void);
	const char *encryptedBy(void);
	const char *signedBy(void);
	const char *fromAddress(void);
	const char *fromName(void);
	const char *mailer(void);
	const char *header(void);
	const char *ck_from(void);
	const char *subject(void);
	const char *replyTo(void);
	const char *bounceAddress(void);
	const char *body(void);
	const char *qEncodeString(const char *str, const char *charset);
	const char *bEncodeString(const char *str, const char *charset);

	const char *getReplaceString2(const char *pattern);
	const char *getReplaceString(long index);
	const char *getReplacePattern(long index);
	const char *generateFilename(void);
	const char *fileDistList(void);
	const char *createTempMht(const char *inFilename);
	const char *qEncodeBytes(const unsigned char *data, unsigned long dataLen, const char *charset);
	const char *bEncodeBytes(const unsigned char *data, unsigned long dataLen, const char *charset);

	const char *addFileAttachment(const char *fileName);
	const char *addRelatedFile(const char *fileName);
	const char *addRelatedString(const char *nameInHtml, const char *str, const char *charset);

	const char *getAltHeaderField(int index, const char *fieldName);
	const char *getAttachedMessageFilename(int index);

	bool AddDataAttachment2(const char *fileName, CkByteData &content, const char *contentType);

	void AppendToBody(const char *str);

	bool GetAltHeaderField(int index, const char *fieldName, CkString &outStr);

	bool GetAttachedMessageFilename(int index, CkString &outStr);

	bool LoadXmlString(const char *xmlStr);

	bool SetAttachmentCharset(int index, const char *charset);

	bool SetFromMimeObject(CkMime &mime);

	bool AspUnpack(const char *prefix, const char *saveDir, const char *urlPath, bool cleanFiles);
	bool AspUnpack2(const char *prefix, const char *saveDir, const char *urlPath, bool cleanFiles, CkByteData &outHtml);

	int GetImapUid(void);

	bool HasHeaderMatching(const char *fieldName, const char *valuePattern, bool caseInsensitive);

	void get_Language(CkString &str);
	const char *language(void);

	CkStringArray *GetDsnFinalRecipients(void);

	bool GetRelatedContentLocation(int index, CkString &outStr);
	const char *getRelatedContentLocation(int index);

	void GetMimeBinary(CkByteData &outBytes);

	bool AttachMessage(CkByteData &mimeBytes);

	bool ComputeGlobalKey(const char *encoding, bool bFold, CkString &outStr);
	const char *computeGlobalKey(const char *encoding, bool bFold);

	int get_NumDaysOld(void);

	void get_PreferredCharset(CkString &str);
	const char *preferredCharset(void);
	void put_PreferredCharset(const char *newVal);

	bool get_VerboseLogging(void);
	void put_VerboseLogging(bool newVal);

	void AddRelatedHeader(int index, const char *fieldName, const char *fieldValue);

	bool TestFilter(const char *expression);

	void ClearEncryptCerts(void);

	bool AddEncryptCert(CkCert &cert);

	bool SetFromMimeBytes(CkByteData &mimeBytes);

	bool SetAttachmentDisposition(int index, const char *disposition);

	void RemoveHtmlAlternative(void);

	void RemovePlainTextAlternative(void);

	void AddHeaderField2(const char *fieldName, const char *fieldValue);

	bool get_PrependHeaders(void);
	void put_PrependHeaders(bool newVal);

	CkEmail *CreateDsn(const char *explanation, const char *xmlDeliveryStatus, bool bHeaderOnly);

	CkEmail *CreateMdn(const char *explanation, const char *xmlMdnFields, bool bHeaderOnly);

	void get_SigningHashAlg(CkString &str);
	const char *signingHashAlg(void);
	void put_SigningHashAlg(const char *newVal);

	void get_Pkcs7CryptAlg(CkString &str);
	const char *pkcs7CryptAlg(void);
	void put_Pkcs7CryptAlg(const char *newVal);

	int get_Pkcs7KeyLength(void);
	void put_Pkcs7KeyLength(int numBits);

    private:

	bool m_utf8;
	bool m_ownership;
	void *m_impl;

	CkEmail(const CkEmail &) { }
	CkEmail &operator=(const CkEmail &) { return *this; }
	CkEmail(void *impl) : m_impl(impl) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:

	void *getImpl(void) const;

	CkEmail(void *impl, bool ownership);

};

#pragma pack (pop)

#endif
