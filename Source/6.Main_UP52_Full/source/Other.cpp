#include "stdafx.h"
#include "Other.h"
#include "Util.h"
Other	gOther;

extern std::string g_strSelectedML;

Other::Other()
{
	Other::Load();
}

Other::~Other()
{
}

void Other::Load()
{

	char TEXTVN_FILE[55] = { 0 };
	wsprintf(TEXTVN_FILE, "./Data/Custom/Text_%s.ini", g_strSelectedML.c_str());

	if (FileExist(TEXTVN_FILE) == false)
	{
		wsprintf(TEXTVN_FILE, "./Data/Custom/Text_Eng.ini", (char*)g_strSelectedML.c_str());
	}

	char GetFotText[55] = { 0 };
	for (int st = 0; st < 50; st++)
	{
		wsprintf(GetFotText, "Text%d", st);
		GetPrivateProfileStringA("Text_VIPCHAR", GetFotText, "Null", Text_VIPCHAR[st], sizeof(Text_VIPCHAR[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("TextVN_NAPGAME", GetFotText, "Null", TextVN_NAPGAME[st], sizeof(TextVN_NAPGAME[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("TextVN_CHOTROI", GetFotText, "Null", TextVN_CHOTROI[st], sizeof(TextVN_CHOTROI[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_NewBank", GetFotText, "Null", Text_NewBank[st], sizeof(Text_NewBank[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_Button", GetFotText, "Null", Text_Button[st], sizeof(Text_Button[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_Event", GetFotText, "Null", Text_Event[st], sizeof(Text_Event[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_InfoDW", GetFotText, "Null", Text_InfoDW[st], sizeof(Text_InfoDW[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_InfoDK", GetFotText, "Null", Text_InfoDK[st], sizeof(Text_InfoDK[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_InfoELF", GetFotText, "Null", Text_InfoELF[st], sizeof(Text_InfoELF[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_InfoMG", GetFotText, "Null", Text_InfoMG[st], sizeof(Text_InfoMG[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_InfoDL", GetFotText, "Null", Text_InfoDL[st], sizeof(Text_InfoDL[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_InfoSUM", GetFotText, "Null", Text_InfoSUM[st], sizeof(Text_InfoSUM[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_InfoRF", GetFotText, "Null", Text_InfoRF[st], sizeof(Text_InfoRF[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_HuyDong", GetFotText, "Null", Text_HuyDong[st], sizeof(Text_HuyDong[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_NapGame", GetFotText, "Null", Text_NapGame[st], sizeof(Text_NapGame[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_Reset", GetFotText, "Null", Text_Rs[st], sizeof(Text_Rs[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_MENU", GetFotText, "Null", Text_MENU[st], sizeof(Text_MENU[st]), TEXTVN_FILE);
		GetPrivateProfileStringA("Text_ChangeItem", GetFotText, "Null", Text_ChangeItem[st], sizeof(Text_ChangeItem[st]), TEXTVN_FILE);
	}

	char GetFotText2[55] = { 0 };
	for (int st2 = 0; st2 < 150; st2++)
	{
		wsprintf(GetFotText2, "Text%d", st2);
		GetPrivateProfileStringA("Text_Chung", GetFotText2, "Null", Text_Chung[st2], sizeof(Text_Chung[st2]), TEXTVN_FILE);
	}
	char GetFotText1[55] = { 0 };
	for (int st = 0; st < 500; st++)
	{
		wsprintf(GetFotText, "Text%d", st);
		GetPrivateProfileStringA("Text_GAME", GetFotText, "Null", Text_GAME[st], sizeof(Text_GAME[st]), TEXTVN_FILE);
	}

	char GetFotText3[55] = { 0 };
	for (int st = 0; st < 500; st++)
	{
		wsprintf(GetFotText, "Text%d", st);
		GetPrivateProfileStringA("Text_Option", GetFotText, "Null", Text_Option[st], sizeof(Text_Option[st]), TEXTVN_FILE);
	}

}