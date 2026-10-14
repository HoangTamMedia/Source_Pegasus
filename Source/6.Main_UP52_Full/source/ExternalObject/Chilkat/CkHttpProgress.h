
#ifndef _CKHTTPPROGRESS_H
#define _CKHTTPPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkHttpProgress
{
    public:
	CkHttpProgress() { }
	virtual ~CkHttpProgress() { }

	virtual void AbortCheck(bool *abort) { }
	virtual void PercentDone(int pctDone, bool *abort) { }

	virtual void ProgressInfo(const char *name, const char *value) { }

	virtual void HttpRedirect(const char *originalUrl, const char *redirectUrl, bool *abort) { }

	virtual void HttpChunked() { }

	virtual void HttpBeginReceive(void) { }
	virtual void HttpEndReceive(bool success) { }
	virtual void HttpBeginSend(void) { }
	virtual void HttpEndSend(bool success) { }

	virtual void ReceiveRate(unsigned long byteCount, unsigned long bytesPerSec) { }

};
#pragma pack (pop)

#endif
