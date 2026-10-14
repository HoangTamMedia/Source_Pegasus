
#ifndef _CKSPIDERPROGRESS_H
#define _CKSPIDERPROGRESS_H

#pragma once

class CkSpiderProgress
{
    public:
	CkSpiderProgress();
	virtual ~CkSpiderProgress();

	virtual void AbortCheck(bool *abort) { }

};

#endif
