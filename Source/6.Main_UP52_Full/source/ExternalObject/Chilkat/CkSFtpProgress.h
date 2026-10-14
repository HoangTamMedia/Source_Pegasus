
#ifndef _CKSFTPPROGRESS_H
#define _CKSFTPPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkSFtpProgress
{
    public:
	CkSFtpProgress() { }
	virtual ~CkSFtpProgress() { }

	virtual void AbortCheck(bool *abort) { }
	virtual void PercentDone(int pctDone, bool *abort) { }

	virtual void UploadRate(unsigned long byteCount, unsigned long bytesPerSec) { }
	virtual void DownloadRate(unsigned long byteCount, unsigned long bytesPerSec) { }

};
#pragma pack (pop)

#endif
