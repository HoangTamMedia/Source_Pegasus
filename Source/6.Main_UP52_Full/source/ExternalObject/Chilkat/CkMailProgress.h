
#ifndef _CkMailProgress_H
#define _CkMailProgress_H

#pragma once

#pragma pack (push, 8)

class CkMailProgress
{
    public:
	CkMailProgress();
	virtual ~CkMailProgress();

	virtual void SendPercentDone(long pctDone, bool *abort) { }
	virtual void ReadPercentDone(long pctDone, bool *abort) { }

	virtual void AbortCheck(bool *abort) { }

	virtual void EmailReceived(const char *subject,
			    const char *fromAddr, const char *fromName,
			    const char *returnPath,
			    const char *date,
			    const char *uidl,
			    int sizeInBytes) { }

};
#pragma pack (pop)

#endif
