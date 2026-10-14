
#ifndef _CKSOCKSPROXYPROGRESS_H
#define _CKSOCKSPROXYPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkSocksProxyProgress
{
    public:
	CkSocksProxyProgress() { }
	virtual ~CkSocksProxyProgress() { }

	virtual void AbortCheck(bool *abort) { }

};
#pragma pack (pop)

#endif
