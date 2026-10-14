#include "stdafx.h"
#include "CustomRingPen.h"
#include "Util.h"

CCustomRingPen gCustomRingPen;

CCustomRingPen::CCustomRingPen()
{
	this->m_CustomRingPenInfo.clear();
}

CCustomRingPen::~CCustomRingPen()
{

}

void CCustomRingPen::Init()
{

}

void CCustomRingPen::Load(CUSTOM_RING_INFO* info)
{
	for(int n=0;n < MAX_CUSTOM_RP;n++)
	{
		if(info[n].Index < 0 || info[n].Index >= MAX_CUSTOM_RP)
		{
			return;
		}
		this->m_CustomRingPenInfo.insert(std::pair<int,CUSTOM_RING_INFO>(info[n].ItemIndex,info[n]));
	}
}

bool CCustomRingPen::CheckCustomRing(int ItemIndex)
{

	std::map<int,CUSTOM_RING_INFO>::iterator it = this->m_CustomRingPenInfo.find(ItemIndex);

	if(it != this->m_CustomRingPenInfo.end() && it->second.ItemType == 1)
	{
		return true;
	}

	return false;
}

bool CCustomRingPen::CheckCustomPen(int ItemIndex)
{
	std::map<int,CUSTOM_RING_INFO>::iterator it = this->m_CustomRingPenInfo.find(ItemIndex);

	if(it != this->m_CustomRingPenInfo.end() && it->second.ItemType >= 2)
	{
		return true;
	}

	return false;
}

int CCustomRingPen::CheckCustomRingPen(int ItemIndex)
{
	std::map<int, CUSTOM_RING_INFO>::iterator it = this->m_CustomRingPenInfo.find(ItemIndex);

	if (it != this->m_CustomRingPenInfo.end())
	{
		return it->second.ItemType;
	}

	return false;
}
