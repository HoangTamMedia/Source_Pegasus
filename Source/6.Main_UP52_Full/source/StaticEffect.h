
#pragma once

struct CUSTOM_WING_EFFECT_INFO
{
	int Index;
	int ItemIndex;
	int EffectIndex;
	int EffectValue;
	int EffectLevel;
	float ColorR;
	float ColorG;
	float ColorB;
	float ColorSize;
	float ColorMain;
	float ColorSide;
};

class CCustomWingEffect
{
public:
	CCustomWingEffect();

	virtual ~CCustomWingEffect();
	void Load(CUSTOM_WING_EFFECT_INFO* info);
	BOOL SetWingEffect(int ItemIndex,DWORD a,DWORD b,DWORD c,DWORD d);

public:
	std::map<int,CUSTOM_WING_EFFECT_INFO> m_CustomWingEffectInfo;
};

extern CCustomWingEffect gCustomWingEffect;
