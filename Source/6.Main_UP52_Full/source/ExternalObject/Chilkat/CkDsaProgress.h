
#ifndef _CKDSAPROGRESS_H
#define _CKDSAPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkDsaProgress
{
    public:
	CkDsaProgress() { }
	virtual ~CkDsaProgress() { }

	virtual void AbortCheck(bool *abort) { }

};
#pragma pack (pop)

#endif
