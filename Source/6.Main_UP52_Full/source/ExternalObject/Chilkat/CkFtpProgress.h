
#ifndef _CKFTPPROGRESS_H
#define _CKFTPPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkFtpProgress
{
    public:
	CkFtpProgress();
	virtual ~CkFtpProgress();

	virtual void FtpPercentDone(long pctDone, bool *abort) { }

	virtual void AbortCheck(bool *abort) { }

	virtual void DataPort(int portNumber) { }

	virtual void BeginDownloadFile(const char *pathUtf8, bool *skip) { }
	virtual void EndDownloadFile(const char *pathUtf8, unsigned long numBytes) { }
	virtual void VerifyDownloadDir(const char *pathUtf8, bool *skip) { }

	virtual void BeginUploadFile(const char *pathUtf8, bool *skip) { }
	virtual void EndUploadFile(const char *pathUtf8, unsigned long numBytes) { }
	virtual void VerifyUploadDir(const char *pathUtf8, bool *skip) { }

	virtual void VerifyDeleteDir(const char *pathUtf8, bool *skip) { }
	virtual void VerifyDeleteFile(const char *pathUtf8, bool *skip) { }

	virtual void UploadRate(unsigned long byteCount, unsigned long bytesPerSec) { }
	virtual void DownloadRate(unsigned long byteCount, unsigned long bytesPerSec) { }

	virtual void ProgressInfo(const char *name, const char *value) { }

};
#pragma pack (pop)

#endif
