
#ifndef _CKSshPROGRESS_H
#define _CKSshPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkSshProgress
{
    public:
	CkSshProgress() { }
	virtual ~CkSshProgress() { }

	virtual void AbortCheck(bool *abort) { }
	virtual void PercentDone(int pctDone, bool *abort) { }

};
#pragma pack (pop)

#endif
