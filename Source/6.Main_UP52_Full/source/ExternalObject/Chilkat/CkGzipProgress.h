
#ifndef _CKGZIPPROGRESS_H
#define _CKGZIPPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkGzipProgress
{
    public:
	CkGzipProgress() { }
	virtual ~CkGzipProgress() { }

	virtual void AbortCheck(bool *abort) { }
	virtual void PercentDone(int pctDone, bool *abort) { }

};
#pragma pack (pop)

#endif
