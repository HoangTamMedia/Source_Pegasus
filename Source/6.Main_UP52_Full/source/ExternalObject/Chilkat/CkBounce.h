
#ifndef _CKBOUNCE_H
#define _CKBOUNCE_H

#pragma once

class CkString;
class CkEmail;
#include "CkObject.h"
#include "CkString.h"

#pragma pack (push, 8)

class CkBounce  : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkBounce(const CkBounce &) { }
	CkBounce &operator=(const CkBounce &) { return *this; }
	CkBounce(void *impl) : m_impl(impl) { }

    public:
	void *getImpl(void) const;

	CkBounce();
	virtual ~CkBounce();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool UnlockComponent(const char *unlockCode);
	bool ExamineEmail(const CkEmail &email);
	bool ExamineMime(const CkString &mimeString);
	bool ExamineEml(const char *emlFilename);

	int get_BounceType(void) const;
	void get_BounceAddress(CkString &bouncedEmailAddr);
	void get_BounceData(CkString &mailBodyText);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	CkString m_resultString;
        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);
	const char *bounceAddress(void);
	const char *bounceData(void);

	void get_Version(CkString &str);
	const char *version(void);

};

#pragma pack (pop)

#endif
