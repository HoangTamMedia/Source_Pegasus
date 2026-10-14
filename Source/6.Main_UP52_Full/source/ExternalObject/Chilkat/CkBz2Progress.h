
#ifndef _CkBz2PROGRESS_H
#define _CkBz2PROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkBz2Progress
{
    public:
	CkBz2Progress() { }
	virtual ~CkBz2Progress() { }

	virtual void AbortCheck(bool *abort) { }

};
#pragma pack (pop)

#endif
