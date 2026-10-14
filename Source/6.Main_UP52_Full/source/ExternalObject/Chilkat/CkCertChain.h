
#if !defined(AFX_CKCERTCHAIN_H__CB7CF92E_A9AC_469E_B628_C76FAA866AB7__INCLUDED_)
#define AFX_CKCERTCHAIN_H__CB7CF92E_A9AC_469E_B628_C76FAA866AB7__INCLUDED_

#pragma once

class CkCert;
class CkCertCollection;
#include "CkObject.h"
#include "CkString.h"

#pragma pack (push, 8)

class CkCertChain  : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkCertChain(const CkCertChain &) { }
	CkCertChain &operator=(const CkCertChain &) { return *this; }
	CkCertChain(void *impl) : m_impl(impl) { }

    public:
	CkCertChain();
	virtual ~CkCertChain();

	void *getImpl(void) const { return m_impl; }

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool BuildChain(const CkCert &cert, CkCertCollection *extraCerts);

	CkCert *GetCert(int index);

	int get_NumCerts(void) const;

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

};
#pragma pack (pop)

#endif
