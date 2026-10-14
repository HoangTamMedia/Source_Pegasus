#pragma once
#include <ItemManager.h>

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
	void Load(ITEMDATA_LUCCHIEN* info);
	void SetInfo(ITEMDATA_LUCCHIEN info);
	ITEMDATA_LUCCHIEN mLucChienDataItem[MAX_CUSTOM_LUCCHIEN];
	ITEMDATA_LUCCHIEN* CustomLucChien::GetInfoLucChien(int IndexItem);
	bool CustomLucChien::CheckItemIsSkill(ITEM* ItemSearch);
	bool CustomLucChien::CheckItemIsLuck(ITEM* ItemSearch);
	int CustomLucChien::CheckItemIsOptLife(ITEM* ItemSearch);
	BYTE CustomLucChien::GetNewOptionCount(BYTE NewOption);
	BYTE CustomLucChien::GetSocketOptionCount(BYTE SocketOption[5]);
	int CustomLucChien::TinhToanLucChien(ITEM* ItemSearch);
};

extern CustomLucChien gCustomLucChien;
