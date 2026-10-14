
#pragma once

#define MAX_ITEM_STACK_INFO 200
struct ITEM_STACK_INFO
{
	int Index;
	int ItemIndex;
	int Level;
	int MaxStack;
	int CreateItemIndex;
};

class CCustomItemStack
{
public:
	CCustomItemStack();
	virtual ~CCustomItemStack();
	void Load(ITEM_STACK_INFO* info);
	bool IsStackItem(DWORD ItemIndex, DWORD Level);
public:
	std::map<int, ITEM_STACK_INFO> m_ItemStack;

};

extern CCustomItemStack gCustomItemStack;
