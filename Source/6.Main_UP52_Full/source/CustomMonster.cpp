#include "stdafx.h"
#include "CustomMonster.h"

CCustomMonster gCustomMonster;

CCustomMonster::CCustomMonster()
{
	this->Init();
}

CCustomMonster::~CCustomMonster()
{

}

void CCustomMonster::Init()
{
	for (int i = 0; i < MAX_CUSTOMMONSTER; i++)
	{
		this->m_CustomMonster[i].Index = -1;
	}

	for (int i = 0; i < MAX_BOSSCLASS; i++)
	{
		this->m_CustomBossClass[i].Index = -1;
	}

}

void CCustomMonster::LoadBossClass(BOSSCLASS_DATA* info)
{
	for (int i = 0; i < MAX_BOSSCLASS; i++)
	{
		this->SetInfoBossClass(info[i]);
	}
}

void CCustomMonster::SetInfoBossClass(BOSSCLASS_DATA info)
{
	if (info.Index < 0 || info.Index >= MAX_BOSSCLASS)
	{
		return;
	}

	this->m_CustomBossClass[info.Index] = info;
}

BOSSCLASS_DATA* CCustomMonster::GetInfoByIDBoss(int MonsterID)
{
	for (int n = 0; n < MAX_BOSSCLASS; n++)
	{
		BOSSCLASS_DATA* lpInfo = this->GetInfoBossByIndex(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (MonsterID == lpInfo->ID)
		{
			return lpInfo;
		}
	}

	return 0;
}
BOSSCLASS_DATA* CCustomMonster::GetInfoBossByIndex(int index)
{
	if (index < 0 || index >= MAX_BOSSCLASS)
	{
		return 0;
	}

	if (this->m_CustomBossClass[index].Index != index)
	{
		return 0;
	}

	return &this->m_CustomBossClass[index];
}

void CCustomMonster::Load(CUSTOMMONSTER_DATA* info)
{
	for (int i = 0; i < MAX_CUSTOMMONSTER; i++)
	{
		this->SetInfo(info[i]);
	}
}

void CCustomMonster::SetInfo(CUSTOMMONSTER_DATA info)
{
	if (info.Index < 0 || info.Index >= MAX_CUSTOMMONSTER)
	{
		return;
	}

	this->m_CustomMonster[info.Index] = info;
}

CUSTOMMONSTER_DATA* CCustomMonster::GetInfoByMonsterID(int MonsterID)
{
	for (int n = 0; n < MAX_CUSTOMMONSTER; n++)
	{
		CUSTOMMONSTER_DATA* lpInfo = this->GetInfoByIndex(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (MonsterID == lpInfo->ID)
		{
			return lpInfo;
		}
	}

	return 0;
}

int CCustomMonster::CheckCustomMonster(int MonsterID)
{
	if (MonsterID < 580)
	{
		return 0;
	}

	for (int n = 0; n < MAX_CUSTOMMONSTER; n++)
	{
		CUSTOMMONSTER_DATA* lpInfo = this->GetInfoByIndex(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (MonsterID == lpInfo->ID && lpInfo->Type == 1)
		{
			return 1;
		}
	}

	return 0;
}

CUSTOMMONSTER_DATA* CCustomMonster::GetInfoByIndex(int index)
{
	if (index < 0 || index >= MAX_CUSTOMMONSTER)
	{
		return 0;
	}

	if (this->m_CustomMonster[index].Index != index)
	{
		return 0;
	}

	return &this->m_CustomMonster[index];
}
#include "ZzzInfomation.h"

char* CCustomMonster::GetNameMonter(int Class)
{
	BOSSCLASS_DATA* BossClass = gCustomMonster.GetInfoByIDBoss(Class);
	if (BossClass != 0)
	{
		return BossClass->Name;
	}

	CUSTOMMONSTER_DATA* lpInfo = gCustomMonster.GetInfoByMonsterID(Class);

	if (lpInfo != 0)
	{
		return lpInfo->Name;
	}

	return getMonsterName(Class);
}