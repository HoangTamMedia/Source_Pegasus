#pragma once

#define MAX_CUSTOM_LUCCHIEN 1500

struct ITEMDATA_LUCCHIEN
{
	int Index;
	int IndexMin;
	int IndexMax;
	int LCCap;
	int LCGoc;
	int LCLevel;
	int LCOptSkill;
	int LCOptLuck;
	int LCOptLife;
	int LCOptExl;
	int LCOptHarmony;
	int LCOpt380;
	int LCOptAnc;
	int LCOptSocket;
};
class CustomLucChien
{
public:
	CustomLucChien();
	virtual ~CustomLucChien();
	void Init();
	void Load(char* path);
	void SetInfo(ITEMDATA_LUCCHIEN info);
	ITEMDATA_LUCCHIEN mLucChienDataItem[MAX_CUSTOM_LUCCHIEN];
};

extern CustomLucChien gCustomLucChien;
