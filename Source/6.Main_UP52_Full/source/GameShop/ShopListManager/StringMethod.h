
#pragma once

#include "include.h"
#include <time.h>

class CStringMethod
{
public:
	CStringMethod();
	virtual ~CStringMethod();

	static void ConvertStringToDateTime(tm& datetime, std::string strdata);
};
