
#ifndef _CkNtlm_H
#define _CkNtlm_H

#pragma once

#include "CkString.h"
#include "CkObject.h"

#pragma pack (push, 8)

class CkNtlm  : public CkObject
{
    private:

	void *m_impl;
	bool m_utf8;

	CkNtlm(const CkNtlm &);
	CkNtlm &operator=(const CkNtlm &);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

	CkNtlm(void *impl);

	CkNtlm();
	virtual ~CkNtlm();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
    void LastErrorXml(CkString &str);
    void LastErrorHtml(CkString &str);
    void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	bool UnlockComponent(const char *unlockCode);

	void get_TargetName(CkString &str);
	const char *targetName(void);
	void put_TargetName(const char *newVal);

	void get_NetBiosComputerName(CkString &str);
	const char *netBiosComputerName(void);
	void put_NetBiosComputerName(const char *newVal);

	void get_NetBiosDomainName(CkString &str);
	const char *netBiosDomainName(void);
	void put_NetBiosDomainName(const char *newVal);

	void get_DnsDomainName(CkString &str);
	const char *dnsDomainName(void);
	void put_DnsDomainName(const char *newVal);

	void get_DnsComputerName(CkString &str);
	const char *dnsComputerName(void);
	void put_DnsComputerName(const char *newVal);

	void get_UserName(CkString &str);
	const char *userName(void);
	void put_UserName(const char *newVal);

	void get_Password(CkString &str);
	const char *password(void);
	void put_Password(const char *newVal);

	int get_NtlmVersion(void);
	void put_NtlmVersion(int newVal);

	int get_OemCodePage(void);
	void put_OemCodePage(int newVal);

	void get_Flags(CkString &str);
	const char *flags(void);
	void put_Flags(const char *newVal);

	void get_Domain(CkString &str);
	const char *domain(void);
	void put_Domain(const char *newVal);

	void get_Workstation(CkString &str);
	const char *workstation(void);
	void put_Workstation(const char *newVal);

	bool GenType1(CkString &outStr);
	const char *genType1(void);

	bool GenType2(const char *type1Msg, CkString &outStr);
	const char *genType2(const char *type1Msg);

	bool GenType3(const char *type2Msg, CkString &outStr);
	const char *genType3(const char *type2Msg);

	bool ParseType1(const char *type1Msg, CkString &outStr);
	const char *parseType1(const char *type1Msg);

	bool ParseType2(const char *type2Msg, CkString &outStr);
	const char *parseType2(const char *type2Msg);

	bool ParseType3(const char *type3Msg, CkString &outStr);
	const char *parseType3(const char *type3Msg);

	void get_EncodingMode(CkString &str);
	const char *encodingMode(void);
	void put_EncodingMode(const char *newVal);

	bool SetFlag(const char *flagLetter, bool on);

	void get_ClientChallenge(CkString &str);
	const char *clientChallenge(void);
	void put_ClientChallenge(const char *newVal);

	void get_ServerChallenge(CkString &str);
	const char *serverChallenge(void);
	void put_ServerChallenge(const char *newVal);

	bool LoadType3(const char *type3Msg);

};
#pragma pack (pop)

#endif
