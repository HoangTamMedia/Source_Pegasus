
#if !defined(AFX_CKCERTCOLLECTION_H__C76FB184_136E_442A_ACFF_773BD284D60E__INCLUDED_)
#define AFX_CKCERTCOLLECTION_H__C76FB184_136E_442A_ACFF_773BD284D60E__INCLUDED_

#pragma once

class CkCert;
class CkString;
#include "CkObject.h"
#include "CkString.h"

#pragma pack (push, 8)

class CkCertCollection  : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkCertCollection(const CkCertCollection &) { }
	CkCertCollection &operator=(const CkCertCollection &) { return *this; }
	CkCertCollection(void *impl) : m_impl(impl) { }

    public:
	CkCertCollection();
	virtual ~CkCertCollection();

	void *getImpl(void) const { return m_impl; }

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool LoadPkcs7File(const char *filename);
	bool LoadPkcs7Data(const unsigned char *data, unsigned long dataLen);

	CkCert *GetCert(int index);

	int get_NumCerts(void) const;

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

};
#pragma pack (pop)

#endif
