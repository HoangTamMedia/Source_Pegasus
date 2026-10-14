
#include "stdafx.h"

#include "StaticEffect.h"
#include "CustomWing.h"
#include "DynamicEffect.h"

#include "NewUISystem.h"

CCustomWingEffect gCustomWingEffect;

CCustomWingEffect::CCustomWingEffect()
{
	this->m_CustomWingEffectInfo.clear();
}

CCustomWingEffect::~CCustomWingEffect()
{

}

void CCustomWingEffect::Load(CUSTOM_WING_EFFECT_INFO* info)
{
	for(int n=0;n < MAX_DYNAMIC_WING_EFFECT;n++)
	{
		if(info[n].Index < 0 || info[n].Index >= MAX_DYNAMIC_WING_EFFECT)
		{
			return;
		}

		this->m_CustomWingEffectInfo.insert(std::pair<int,CUSTOM_WING_EFFECT_INFO>(info[n].Index,info[n]));

	}
}

BOOL CCustomWingEffect::SetWingEffect(int ItemIndex,DWORD a,DWORD b,DWORD c,DWORD d)
{
	float ItemColor1[3];
	float ItemColor2[3];
	int active = 0;

	for(std::map<int, DYNAMIC_WING_EFFECT_INFO>::iterator it =  gDynamicWingEffect.m_DynamicWingEffectInfo.begin(); it !=  gDynamicWingEffect.m_DynamicWingEffectInfo.end(); it ++)
	{
		if(it->second.ItemIndex == ItemIndex)
		{
			ItemColor1[0] = it->second.ColorR;

			ItemColor1[1] = it->second.ColorG;

			ItemColor1[2] = it->second.ColorB;

			active = 1;
		}

	}

	for(std::map<int, CUSTOM_WING_EFFECT_INFO>::iterator it2 =  this->m_CustomWingEffectInfo.begin(); it2 !=  this->m_CustomWingEffectInfo.end(); it2 ++)
	{
		if(it2->second.ItemIndex == ItemIndex)
		{
			ItemColor2[0] = it2->second.ColorR;

			ItemColor2[1] = it2->second.ColorG;

			ItemColor2[2] = it2->second.ColorB;

			active = 1;
		}
	}

	return active;
}