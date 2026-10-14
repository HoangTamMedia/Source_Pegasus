#pragma once
#define TEXTVN_FILEBBB "./Data/Custom/TextVN.dat"

class Other
{
public:
	Other();
	virtual ~Other();
	void Other::Load();
	char Text_Chung[150][120];
	char Text_Rs[50][120];
	char Text_VIPCHAR[50][120];
	char Text_Event[50][120];
	char Text_Button[50][120];
	char TextVN_CHOTROI[50][120];
	char TextVN_NAPGAME[50][120];
	char Text_MENU[50][120];
	char Text_InfoDW[50][120];
	char Text_InfoDK[50][120];
	char Text_InfoELF[50][120];
	char Text_InfoMG[50][120];
	char Text_InfoDL[50][120];
	char Text_InfoSUM[50][120];
	char Text_InfoRF[50][120];
	char Text_GAME[500][500];
	char Text_Option[500][500];
	char Text_HuyDong[500][500];
	char Text_NapGame[50][120];
	char Text_NewBank[50][120];
	char Text_ChangeItem[50][120];
};

extern Other	gOther;