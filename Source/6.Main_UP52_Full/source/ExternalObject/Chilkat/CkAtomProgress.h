
#ifndef _CKAtomPROGRESS_H
#define _CKAtomPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkAtomProgress
{
    public:
	CkAtomProgress() { }
	virtual ~CkAtomProgress() { }

	virtual void AbortCheck(bool *abort) { }

};
#pragma pack (pop)

#endif
