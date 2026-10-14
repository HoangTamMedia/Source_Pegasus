
#ifndef _CKMHTPROGRESS_H
#define _CKMHTPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkMhtProgress
{
    public:
	CkMhtProgress() { }
	virtual ~CkMhtProgress() { }

	virtual void AbortCheck(bool *abort) { }

	virtual void DownloadingUrl(const char *url) { }

	virtual void ProgressInfo(const char *name, const char *value) { }
};
#pragma pack (pop)

#endif
