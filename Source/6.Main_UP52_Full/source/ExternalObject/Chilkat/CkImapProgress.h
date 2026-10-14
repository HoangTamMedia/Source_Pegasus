
#ifndef _CKIMAPPROGRESS_H
#define _CKIMAPPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkImapProgress
{
    public:
	CkImapProgress() { }
	virtual ~CkImapProgress() { }

	virtual void AbortCheck(bool *abort) { }
	virtual void PercentDone(int pctDone, bool *abort) { }

};
#pragma pack (pop)

#endif
