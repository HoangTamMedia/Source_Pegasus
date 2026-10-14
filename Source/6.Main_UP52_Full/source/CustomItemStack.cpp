
#include "stdafx.h"
#include "CustomWing.h"
#include "CustomItemStack.h"
#include "NewUISystem.h"

CCustomItemStack gCustomItemStack;

CCustomItemStack::CCustomItemStack()
{
	this->m_ItemStack.clear();
}

CCustomItemStack::~CCustomItemStack()
{

}

void CCustomItemStack::Load(ITEM_STACK_INFO* info)
{
	for(int n=0;n < MAX_ITEM_STACK_INFO;n++)
	{
		if(info[n].Index < 0 || info[n].Index >= MAX_ITEM_STACK_INFO)
		{

			return;
		}

		this->m_ItemStack.insert(std::pair<int, ITEM_STACK_INFO>(info[n].Index, info[n]));

	}
}

bool CCustomItemStack::IsStackItem(DWORD ItemIndex, DWORD Level)
{

	for (std::map<int, ITEM_STACK_INFO>::iterator it = this->m_ItemStack.begin(); it != this->m_ItemStack.end(); it++)
	{

		if (ItemIndex == it->second.ItemIndex && ItemIndex != 0 && Level == it->second.Level)
		{
			return true;
		}
	}

	return false;

}