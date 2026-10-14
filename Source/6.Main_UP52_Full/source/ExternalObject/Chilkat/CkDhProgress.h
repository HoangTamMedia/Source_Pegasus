
#ifndef _CKDHPROGRESS_H
#define _CKDHPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkDhProgress
{
    public:
	CkDhProgress() { }
	virtual ~CkDhProgress() { }

	virtual void AbortCheck(bool *abort) { }

};
#pragma pack (pop)

#endif
