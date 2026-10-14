#include "stdafx.h"
#include "ItemTRSData.h"
#include "Util.h"
#include "CustomGloves.h"
#include "CustomBow.h"

cCustomPosition ItemTRSData;

cCustomPosition::cCustomPosition()
{
	this->m_ItemTRSData.clear();
}

cCustomPosition::~cCustomPosition()
{

}

void cCustomPosition::Init()
{

}

void cCustomPosition::Load(CUSTOM_POSITION_INFO* info)
{
	for (int n = 0; n < MAX_POSITION_ITEM; n++)
	{
		if (info[n].Index < 0 || info[n].Index >= MAX_POSITION_ITEM)
		{
			return;
		}

		this->m_ItemTRSData.insert(std::pair<int, CUSTOM_POSITION_INFO>(info[n].ItemIndex,info[n]));
	}
}

bool cCustomPosition::GetItemTRSData(int ItemIndex)
{
	std::map<int, CUSTOM_POSITION_INFO>::iterator it = this->m_ItemTRSData.find(ItemIndex);

	if(it != this->m_ItemTRSData.end())
	{
		return true;
	}

	return false;
}

float cCustomPosition::GetItemTRSDataTransLationX(int ItemIndex)
{
	std::map<int, CUSTOM_POSITION_INFO>::iterator it = this->m_ItemTRSData.find(ItemIndex);

	if(it != this->m_ItemTRSData.end())
	{
		return it->second.translationX;
	}
	return 0;
}

float cCustomPosition::GetItemTRSDataTransLationY(int ItemIndex)
{
	std::map<int, CUSTOM_POSITION_INFO>::iterator it = this->m_ItemTRSData.find(ItemIndex);

	if(it != this->m_ItemTRSData.end())
	{
		return it->second.translationY;
	}
	return 0;
}

float cCustomPosition::GetItemTRSDataRotationX(int ItemIndex)
{
	std::map<int, CUSTOM_POSITION_INFO>::iterator it = this->m_ItemTRSData.find(ItemIndex);

	if(it != this->m_ItemTRSData.end())
	{
		return it->second.RotationX;
	}
	return 0;
}

float cCustomPosition::GetItemTRSDataRotationY(int ItemIndex)
{
	std::map<int, CUSTOM_POSITION_INFO>::iterator it = this->m_ItemTRSData.find(ItemIndex);

	if(it != this->m_ItemTRSData.end())
	{
		return it->second.RotationY;
	}
	return 0;
}

float cCustomPosition::GetItemTRSDataRotationZ(int ItemIndex)
{
	std::map<int, CUSTOM_POSITION_INFO>::iterator it = this->m_ItemTRSData.find(ItemIndex);

	if(it != this->m_ItemTRSData.end())
	{
		return it->second.RotationZ;
	}
	return 0;
}

float cCustomPosition::GetItemTRSDataScale(int ItemIndex)
{
	std::map<int, CUSTOM_POSITION_INFO>::iterator it = this->m_ItemTRSData.find(ItemIndex);

	if(it != this->m_ItemTRSData.end())
	{
		return it->second.Scale;
	}
	return 0;
}
