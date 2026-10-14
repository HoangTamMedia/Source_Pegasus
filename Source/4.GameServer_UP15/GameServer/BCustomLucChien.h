#pragma once
#include "Protocol.h"

struct CHAR_DATA
{
	int LCLevel;
	int LCStr;
	int LCAgi;
	int LCVit;
	int LCEne;
	int LCCmd;
	int LCLvMaster;
	int LCAddSkillMT;
	int LCTuLuyen;
	int LCDanhHieu;
	int LCHonHoan;
	void clear()
	{
		LCLevel = 0;
		LCStr = 0;
		LCAgi = 0;
		LCVit = 0;
		LCEne = 0;
		LCCmd = 0;
		LCLvMaster = 0;
		LCAddSkillMT = 0;
		LCTuLuyen = 0;
		LCDanhHieu = 0;
		LCHonHoan = 0;
	}

};
struct ITEMDATA_LUCCHIEN
{
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

struct DATA_LUCCHIEN
{
	PSBMSG_HEAD header; // C3:F3:03
	DWORD LucChien;
};

class BCustomLucChien
{
public:
	BCustomLucChien();
	virtual ~BCustomLucChien();
	void Init();
	void LoadFileXML(char* FilePath);
	int Enable;
	CHAR_DATA mLucChienDataChar;
	std::vector<ITEMDATA_LUCCHIEN> mLucChienDataItem;
	void BCustomLucChien::CalcLucChien(int aIndex);
	int BCustomLucChien::GetLucChienItem(CItem* Item);
	void CharacterCalc(int aIndex, bool flag);
	
};

extern BCustomLucChien gBCustomLucChien;
