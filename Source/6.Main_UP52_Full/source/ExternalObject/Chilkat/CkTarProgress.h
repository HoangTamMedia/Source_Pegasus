
#ifndef _CKTARROGRESS_H
#define _CKTARPROGRESS_H

#pragma once

#ifndef WIN32
#include "int64.h"
#endif

#pragma pack (push, 8)

class CkTarProgress
{
    public:
	CkTarProgress() { }
	virtual ~CkTarProgress() { }

	virtual void AbortCheck(bool *abort) { }

	virtual void NextTarFile(const char *fileName,
	    __int64 fileSize,
	    bool bIsDirectory,
	    bool *skip) { }

};
#pragma pack (pop)

#endif
