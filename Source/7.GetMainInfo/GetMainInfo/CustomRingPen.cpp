#include "stdafx.h"
#include "CustomRingPen.h"
#include "MemScript.h"

cCustomRingPen gCustomRingPen;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cCustomRingPen::cCustomRingPen() // OK
{
	this->Init();
}

cCustomRingPen::~cCustomRingPen() // OK
{

}

void cCustomRingPen::Init() // OK
{
	for(int n=0;n < MAX_CUSTOM_BOW;n++)
	{
		this->m_CustomRingPenInfo[n].Index = -1;
	}
}

bool cCustomRingPen::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR,path);
		return 0;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		printf(lpMemScript->GetLastError());
		delete lpMemScript;
		return 0;
	}

	this->Init();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			CUSTOM_RING_INFO info;

			memset(&info,0,sizeof(info));

			static int CustomItemIndexCount = 0;

			info.Index = CustomItemIndexCount++;

			info.ItemIndex = lpMemScript->GetNumber();

			strcpy_s(info.ModelName,lpMemScript->GetAsString());

			info.ItemType = lpMemScript->GetAsNumber();

			info.ColorR = lpMemScript->GetAsNumber();

			info.ColorG = lpMemScript->GetAsNumber();

			info.ColorB = lpMemScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch(...)
	{
		printf(lpMemScript->GetLastError());
		return 0;
	}
	delete lpMemScript;
	return 1;
}

void cCustomRingPen::SetInfo(CUSTOM_RING_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_RP)
	{
		return;
	}
	this->m_CustomRingPenInfo[info.Index] = info;
}