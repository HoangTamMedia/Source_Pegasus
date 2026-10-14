
#ifndef _CKDKIMPROGRESS_H
#define _CKDKIMPROGRESS_H

#pragma once

#pragma pack (push, 8)

class CkDkimProgress
{
    public:
	CkDkimProgress() { }
	virtual ~CkDkimProgress() { }

	virtual void AbortCheck(bool *abort) { }

};
#pragma pack (pop)

#endif
