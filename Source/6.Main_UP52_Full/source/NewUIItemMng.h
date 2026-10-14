
#if !defined(AFX_NEWUIITEMMNG_H__FAF3ACC1_45A1_4912_9EB6_E3635B4130B0__INCLUDED_)
#define AFX_NEWUIITEMMNG_H__FAF3ACC1_45A1_4912_9EB6_E3635B4130B0__INCLUDED_

#pragma once

#pragma warning(disable : 4786)
#include <list>

#include "./Time/Timer.h"

namespace SEASON3B
{
	class CNewUIItemMng
	{
		typedef std::list<ITEM*>	type_list_item;

		type_list_item	m_listItem;
		DWORD	m_dwAlternate, m_dwAvailableKeyStream;
		CTimer2 m_UpdateTimer;

	public:
		CNewUIItemMng();
		virtual ~CNewUIItemMng();

		ITEM* CreateItem(BYTE* pbyItemPacket);
		ITEM* CreateItem(WORD byType, WORD bySubType, BYTE byLevel = 0, BYTE byDurability = 255, BYTE byOption1 = 0, BYTE byOptionEx = 0, BYTE byOption380 = 0, BYTE byOptionHarmony = 0	, BYTE * pbySocketOptions = NULL);
		ITEM* CreateItem(ITEM* pItem);
		ITEM* DuplicateItem(ITEM* pItem);
		void DeleteItem(ITEM* pItem);
		void DeleteDuplicatedItem(ITEM* pItem);
		void DeleteAllItems();

		bool IsEmpty();

		void Update();

	protected:
		DWORD GenerateItemKey();
		DWORD FindAvailableKeyIndex(DWORD dwSeed);

		WORD ExtractItemType(BYTE* pbyItemPacket);
		void SetItemAttr(ITEM* pItem, BYTE byLevel, BYTE byOption1, BYTE byOptionEx);
	};
}

#endif
