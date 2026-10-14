
#ifndef _CKRssPROGRESS_H
#define _CKRssPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkRssProgress
{
    public:
	CkRssProgress() { }
	virtual ~CkRssProgress() { }

	virtual void AbortCheck(bool *abort) { }

};
#pragma pack (pop)

#endif
