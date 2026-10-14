
#include "stdafx.h"
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
#include "StringMethod.h"

CStringMethod::CStringMethod()
{

}

CStringMethod::~CStringMethod()
{

}

void CStringMethod::ConvertStringToDateTime(tm& datetime,std::string strdata)
{
	if(strdata.length()>=4)
	{
		datetime.tm_year = atoi(strdata.substr(0,4).c_str())-1900;
	}

	if(strdata.length()>=6)
	{
		datetime.tm_mon = atoi(strdata.substr(4,2).c_str())-1;
	}

	if(strdata.length()>=8)
	{
		datetime.tm_mday = atoi(strdata.substr(6,2).c_str());
	}

	if(strdata.length()>=10)
	{
		datetime.tm_hour = atoi(strdata.substr(8,2).c_str());
	}

	if(strdata.length()>=12)
	{
		datetime.tm_min = atoi(strdata.substr(10,2).c_str());
	}

	if(strdata.length()>=14)
	{
		datetime.tm_sec = atoi(strdata.substr(12,2).c_str());
	}

	mktime(&datetime);
}
#endif