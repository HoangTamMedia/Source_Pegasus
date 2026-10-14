
#ifndef _CKCRYPT2PROGRESS_H
#define _CKCRYPT2PROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkCrypt2Progress
{
    public:
	CkCrypt2Progress() { }
	virtual ~CkCrypt2Progress() { }

	virtual void AbortCheck(bool *abort) { }

	virtual void PercentDone(int pctDone, bool *abort) { }

};
#pragma pack (pop)

#endif
