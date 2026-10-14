#include "stdafx.h"
#include "CCRC32.H"
#include "CustomItem.h"
#include "CustomWing.h"
#include "CustomWIngEffect.h"
#include "DynamicEffect.h"
#include "ThemidaSDK.h"
#include "CustomMessage.h"
#include "CustomJewel.h"
#include "CustomCloak.h"
#include "CustomGloves.h"
#include "CustomCrossBow.h"
#include "CustomItemPosition.h"
#include "CustomMonster.h"
#include "CustomPet.h"
#include "UIMapName.h"
#include "TooltipBuff.h"
#include "CustomNpcName.h"
#include "ItemToolTip.h"
#include "CustomBuyVip.h"
#include "CustomPetEffect.h"
#include "CustomMonsterGlow.h"
#include "MonsterEffect.h"
#include "ItemStack.h"
#include "CustomRingPen.h"
#include "CustomLucChien.h"

// Função para escrever log em arquivo e console
void WriteLog(const char* format, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsprintf_s(buffer, sizeof(buffer), format, args);
	va_end(args);

	// Escrever no console
	printf("%s\n", buffer);

	// Escrever no arquivo de log
	FILE* logFile = NULL;
	fopen_s(&logFile, "GetMainInfo.log", "a");
	if (logFile)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		fprintf(logFile, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n", 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, buffer);
		fclose(logFile);
	}
}
struct MAIN_FILE_INFO
{
	BYTE LauncherType;
	char CustomerName[32];
	char IpAddress[32];
	WORD IpAddressPort;
	char ClientVersion[8];
	char ClientSerial[17];
	char WindowName[32];
	char ScreenShotPath[50];
	char ClientName[32];
	DWORD TIME_GMT;
	char PluginName[32];
	char CameraName[32];
	char PathItem[32];
	char PathSet[32];
	char PathWing[32];
	char PathPet[32];
	char PathGloves[32];
	char PathBow[32];
	DWORD ClientCRC32;
	DWORD PluginCRC32;
	DWORD CameraCRC32;
	DWORD HelperActiveAlert;
	DWORD HelperActiveLevel;
	DWORD DWMaxAttackSpeed;
	DWORD DKMaxAttackSpeed;
	DWORD FEMaxAttackSpeed;
	DWORD MGMaxAttackSpeed;
	DWORD DLMaxAttackSpeed;
	DWORD SUMaxAttackSpeed;
	DWORD RFMaxAttackSpeed;
	DWORD ReconnectTime;

	DWORD EnableCsSkillsAllMaps;
	DWORD PlayerShowName;
	DWORD PlayerShowGuildLogo;


	DWORD Menu[15];

	//===Nap Bank
	DWORD EnableNapBankButton;
	char Bank_NameBank[126];
	char Bank_STK[126];
	char Bank_ChuTK[126];
	char Bank_ChiNhanh[126];
	//===
	BOOL EnableButtonFB;
	char LinkFacbook[128];
	int ViTriFB[2];
	BOOL EnableButtonZL;
	char LinkZalo[128];
	int ViTriZL[2];
	BOOL EnableButtonRS;
	int ViTriRS[2];
	BOOL NewText;
	int EnableXShop;
	int HienPetTrongThanh;
	int ChoPetChayTrongThanh;
	BOOL EnableButtonPK;
	int ViTriPK[2];
	
	
	CUSTOM_MESSAGE_INFO EngCustomMessageInfo[MAX_CUSTOM_MESSAGE];
	CUSTOM_MESSAGE_INFO VtmCustomMessageInfo[MAX_CUSTOM_MESSAGE];
	CUSTOM_JEWEL_INFO CustomJewelInfo[MAX_CUSTOM_JEWEL];

	CUSTOM_WING_INFO CustomWingInfo[MAX_CUSTOM_WING];
	CUSTOMCLOAK_DATA m_CustomCloak[MAX_CUSTOMCLOAK];
	CEFFECT_DATA m_CustomCEffect[MAX_CUSTOMCEFFECT];
	CUSTOM_ITEM_INFO CustomItemInfo[MAX_CUSTOM_ITEM];
	CUSTOM_WING_EFFECT_INFO CustomWingEffectInfo[MAX_CUSTOM_WING_EFFECT];
	DYNAMIC_WING_EFFECT_INFO DynamicWingEffectInfo[MAX_DYNAMIC_WING_EFFECT];
	CUSTOM_BOW_INFO CustomBowInfo[MAX_CUSTOM_BOW];
	CUSTOM_RF_GLOVES CustomGloves[MAX_CUSTOM_GLOVES];
	CUSTOM_POSITION_INFO CustomPosition[MAX_POSITION_ITEM];
	DWORD IsVersion;
	BOOL  ShowInfoItemDrop;

	CUSTOMMONSTER_DATA CustomMonsters[MAX_CUSTOMMONSTER];
	BOSSCLASS_DATA	   CustomBossClass[MAX_BOSSCLASS]; //Boss Class
	NPCNAME_DATA CustomNPCName[MAX_CUSTOM_NPCNAME];

	CUSTOM_PET_INFO CustomPetInfo[MAX_PET_ITEM];
	PET_EFFECT_CUSTOM m_PetCEffectBMD[MAX_CUSTOPETEFFECT];
	RENDER_MESH RenderMeshPet[MAX_RENDER_MESH];

	PathMap m_MapInfo[MAX_MAP];

	CUSTOM_BUYVIP_INFO CustomBuyVipInfo[MAX_CUSTOM_BUYVIP];
	CUSTOM_MONSTERGLOW_INFO m_CustomMonsterGlow[MAX_CUSTOM_MONSTERGLOW];
	CUSTOM_GLOW_INFO m_CustomMonsterbrightness[MAX_CUSTOM_MONSTERGLOW];
	xMonsterEffect m_CustomMonsterEffect[MAX_EFFECT_MONSTER];
	ITEM_STACK_INFO ItemStackInfo[MAX_ITEM_STACK_INFO];
	CUSTOM_RING_INFO CustomRingPenInfo[MAX_CUSTOM_RP];
	ITEMDATA_LUCCHIEN m_LucChienDataItem[MAX_CUSTOM_LUCCHIEN];
};
struct TEXT_FILE_INFO
{
	RenderTooltipBuff m_TooltipTRSDataEng[256];
	RenderTooltipBuff m_TooltipTRSDataVTM[256];

	nInformation m_TRSTooltipData[MaxLineItemToolTip];
	nText m_TRSTooltipText[MaxLineItemToolTip];
};

void GetMainInfo()
{
	WriteLog("=== INICIO GetMainInfo() ===");
	
	MAIN_FILE_INFO info;

	memset(&info, 0, sizeof(info));
	
	WriteLog("Verificando existencia do arquivo MainInfo.ini...");
	DWORD dwAttrib = GetFileAttributesA(".\\MainInfo.ini");
	if (dwAttrib == INVALID_FILE_ATTRIBUTES)
	{
		char currentDir[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, currentDir);
		WriteLog("ERRO: Arquivo MainInfo.ini nao encontrado no diretorio atual!");
		WriteLog("Diretorio atual: %s", currentDir);
		return;
	}
	WriteLog("Arquivo MainInfo.ini encontrado. Iniciando leitura...");

	info.LauncherType = GetPrivateProfileInt("MainInfo", "LauncherType", 0, ".\\MainInfo.ini");

	info.IpAddressPort = GetPrivateProfileInt("MainInfo", "IpAddressPort", 44405, ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "CustomerName", "", info.CustomerName, sizeof(info.CustomerName), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "IpAddress", "", info.IpAddress, sizeof(info.IpAddress), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "ClientVersion", "", info.ClientVersion, sizeof(info.ClientVersion), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "ClientSerial", "", info.ClientSerial, sizeof(info.ClientSerial), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "WindowName", "", info.WindowName, sizeof(info.WindowName), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "ScreenShotPath", "", info.ScreenShotPath, sizeof(info.ScreenShotPath), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "ClientName", "", info.ClientName, sizeof(info.ClientName), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "PluginName", "", info.PluginName, sizeof(info.PluginName), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "CameraName", "", info.CameraName, sizeof(info.CameraName), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "PathItem", "", info.PathItem, sizeof(info.PathItem), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "PathSet", "", info.PathSet, sizeof(info.PathSet), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "PathWing", "", info.PathWing, sizeof(info.PathWing), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "PathPet", "", info.PathPet, sizeof(info.PathPet), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "PathGloves", "", info.PathGloves, sizeof(info.PathGloves), ".\\MainInfo.ini");

	GetPrivateProfileString("MainInfo", "PathBow", "", info.PathBow, sizeof(info.PathBow), ".\\MainInfo.ini");

	info.TIME_GMT = GetPrivateProfileInt("MainInfo", "GTM", 0, ".\\MainInfo.ini");

	info.HelperActiveAlert = GetPrivateProfileInt("HelperInfo", "HelperActiveAlert", 0, ".\\MainInfo.ini");

	info.HelperActiveLevel = GetPrivateProfileInt("HelperInfo", "HelperActiveLevel", 80, ".\\MainInfo.ini");

	info.DWMaxAttackSpeed = GetPrivateProfileInt("CharacterInfo", "DWMaxAttackSpeed", 65535, ".\\MainInfo.ini");

	info.DKMaxAttackSpeed = GetPrivateProfileInt("CharacterInfo", "DKMaxAttackSpeed", 65535, ".\\MainInfo.ini");

	info.FEMaxAttackSpeed = GetPrivateProfileInt("CharacterInfo", "FEMaxAttackSpeed", 65535, ".\\MainInfo.ini");

	info.MGMaxAttackSpeed = GetPrivateProfileInt("CharacterInfo", "MGMaxAttackSpeed", 65535, ".\\MainInfo.ini");

	info.DLMaxAttackSpeed = GetPrivateProfileInt("CharacterInfo", "DLMaxAttackSpeed", 65535, ".\\MainInfo.ini");

	info.SUMaxAttackSpeed = GetPrivateProfileInt("CharacterInfo", "SUMaxAttackSpeed", 65535, ".\\MainInfo.ini");

	info.RFMaxAttackSpeed = GetPrivateProfileInt("CharacterInfo", "RFMaxAttackSpeed", 65535, ".\\MainInfo.ini");

	info.ReconnectTime = GetPrivateProfileInt("ReconnectInfo", "ReconnectTime", 0, ".\\MainInfo.ini");

	info.IsVersion = GetPrivateProfileInt("MainInfo", "IsSeason", 0, ".\\MainInfo.ini");

	info.ShowInfoItemDrop = GetPrivateProfileInt("MainInfo", "ShowInfoItemDrop", 0, ".\\MainInfo.ini");

	info.EnableCsSkillsAllMaps = GetPrivateProfileInt("Custom", "EnableCsSkillsAllMaps", 0, ".\\MainInfo.ini");
	info.PlayerShowGuildLogo = GetPrivateProfileInt("Custom", "PlayerShowGuildLogo", 0, ".\\MainInfo.ini");


	//==Button Menu
	char GetFotText[55] = { 0 };
	for (int st = 0; st < 15; st++)
	{
		wsprintf(GetFotText, "Menu%d", st);
		info.Menu[st] = GetPrivateProfileInt("Custom", GetFotText, 0, ".\\MainInfo.ini");
	}
	//===Nap Bank
	info.EnableNapBankButton = GetPrivateProfileInt("Custom", "EnableNapBankButton", 0, ".\\MainInfo.ini");
	GetPrivateProfileString("Custom", "Bank_NameBank", "Bank_NameBank", info.Bank_NameBank, sizeof(info.Bank_NameBank), ".\\MainInfo.ini");
	GetPrivateProfileString("Custom", "Bank_STK", "Bank_STK", info.Bank_STK, sizeof(info.Bank_STK), ".\\MainInfo.ini");
	GetPrivateProfileString("Custom", "Bank_ChuTK", "Bank_ChuTK", info.Bank_ChuTK, sizeof(info.Bank_ChuTK), ".\\MainInfo.ini");
	GetPrivateProfileString("Custom", "Bank_ChiNhanh", "Bank_ChiNhanh", info.Bank_ChiNhanh, sizeof(info.Bank_ChiNhanh), ".\\MainInfo.ini");
	//=====
	info.EnableButtonFB = GetPrivateProfileInt("Custom", "EnableButtonFB", 0, ".\\MainInfo.ini");
	GetPrivateProfileString("Custom", "LinkFacbook", "LinkFacbook", info.LinkFacbook, sizeof(info.LinkFacbook), ".\\MainInfo.ini");
	info.ViTriFB[0] = GetPrivateProfileInt("Custom", "ViTriFB_X", 0, ".\\MainInfo.ini");
	info.ViTriFB[1] = GetPrivateProfileInt("Custom", "ViTriFB_Y", 0, ".\\MainInfo.ini");

	info.EnableButtonZL = GetPrivateProfileInt("Custom", "EnableButtonZL", 0, ".\\MainInfo.ini");
	GetPrivateProfileString("Custom", "LinkZalo", "LinkZalo", info.LinkZalo, sizeof(info.LinkZalo), ".\\MainInfo.ini");
	info.ViTriZL[0] = GetPrivateProfileInt("Custom", "ViTriZL_X", 0, ".\\MainInfo.ini");
	info.ViTriZL[1] = GetPrivateProfileInt("Custom", "ViTriZL_Y", 0, ".\\MainInfo.ini");

	info.EnableButtonRS = GetPrivateProfileInt("Custom", "EnableButtonRS", 0, ".\\MainInfo.ini");
	info.ViTriRS[0] = GetPrivateProfileInt("Custom", "ViTriRS_X", 0, ".\\MainInfo.ini");
	info.ViTriRS[1] = GetPrivateProfileInt("Custom", "ViTriRS_Y", 0, ".\\MainInfo.ini");
	info.NewText = GetPrivateProfileInt("Custom", "RenderNewText", 0, ".\\MainInfo.ini");
	info.EnableXShop = GetPrivateProfileInt("Custom", "EnableButtonXShop", 0, ".\\MainInfo.ini");

	info.HienPetTrongThanh = GetPrivateProfileInt("Custom", "EnablePetInTower", 0, ".\\MainInfo.ini");
	info.ChoPetChayTrongThanh = GetPrivateProfileInt("Custom", "EnablePetRunInTower", 0, ".\\MainInfo.ini");

	info.EnableButtonPK = GetPrivateProfileInt("Custom", "EnableButtonPK", 0, ".\\MainInfo.ini");
	info.ViTriPK[0] = GetPrivateProfileInt("Custom", "ViTriPK_X", 0, ".\\MainInfo.ini");
	info.ViTriPK[1] = GetPrivateProfileInt("Custom", "ViTriPK_Y", 0, ".\\MainInfo.ini");


	WriteLog("Lendo informacoes do MainInfo.ini concluido.");
	
	//===Effect
	WriteLog("Carregando arquivos de Effect...");
	gCustomWingEffect.Load("EffectManager\\CustomEffectStatic.txt");
	WriteLog("  - CustomEffectStatic.txt carregado");
	gDynamicWingEffect.Load("EffectManager\\CustomEffectDynamic.txt");
	WriteLog("  - CustomEffectDynamic.txt carregado");
	gCloak.LoadCEffect("EffectManager\\CEffect.txt");
	WriteLog("  - CEffect.txt carregado");
	gCustomMonsterGlow.Load("EffectManager\\CustomMonsterGlow.txt");
	WriteLog("  - CustomMonsterGlow.txt carregado");
	memcpy(info.m_CustomMonsterGlow, gCustomMonsterGlow.m_CustomGlowInfo, sizeof(info.m_CustomMonsterGlow));
	memcpy(info.m_CustomMonsterbrightness, gCustomMonsterGlow.m_CustombrightnessInfo, sizeof(info.m_CustomMonsterbrightness));
	JCEffectMonster.Load("EffectManager\\CustomMonsterEffect.txt");
	WriteLog("  - CustomMonsterEffect.txt carregado");
	memcpy(info.m_CustomMonsterEffect, JCEffectMonster.m_MonsterEffect, sizeof(info.m_CustomMonsterEffect));
	
	//===Item
	WriteLog("Carregando arquivos de Item...");
	gCloak.Load("ItemManager\\CustomCape.txt");
	WriteLog("  - CustomCape.txt carregado");
	gCustomGloves.Load("ItemManager\\CustomClaws.txt");
	WriteLog("  - CustomClaws.txt carregado");
	gCustomCrossBow.Load("ItemManager\\CustomCrossbow.txt");
	WriteLog("  - CustomCrossbow.txt carregado");
	gCustomItem.Load("ItemManager\\CustomItem.txt");
	WriteLog("  - CustomItem.txt carregado");
	gCustomPosition.Load("ItemManager\\CustomItemPosition.txt");
	WriteLog("  - CustomItemPosition.txt carregado");
	gCustomJewel.Load("ItemManager\\CustomJewel.txt");
	WriteLog("  - CustomJewel.txt carregado");
	gCustomWing.Load("ItemManager\\CustomWing.txt");
	WriteLog("  - CustomWing.txt carregado");

	gCustomLucChien.Load("ItemManager\\CustomLucChien.xml");
	WriteLog("  - CustomLucChien.xml carregado");

	gCustomMessage.Load("CommonManager\\CustomMessage.txt");
	WriteLog("  - CustomMessage.txt carregado");

	WriteLog("Carregando ItemStack.txt...");
	if (gItemStack.Load("ItemManager\\ItemStack.txt") == 0) 
	{ 
		WriteLog("ERRO: Falha ao carregar ItemStack.txt!");
		system("pause"); 
		return; 
	}
	WriteLog("  - ItemStack.txt carregado com sucesso");
	
	WriteLog("Carregando CustomRingPen.txt...");
	if (gCustomRingPen.Load("ItemManager\\CustomRingPen.txt") == 0) 
	{ 
		WriteLog("ERRO: Falha ao carregar CustomRingPen.txt!");
		system("pause"); 
		return; 
	}
	WriteLog("  - CustomRingPen.txt carregado com sucesso");
	
	memcpy(info.ItemStackInfo, gItemStack.m_ItemStackInfo, sizeof(info.ItemStackInfo));

	WriteLog("Carregando CustomMonster.txt...");
	gMonsters.Load("CustomManager\\CustomMonster.txt");
	WriteLog("  - CustomMonster.txt carregado");
	memcpy(info.CustomMonsters, gMonsters.m_Monsters, sizeof(info.CustomMonsters));
	memcpy(info.CustomBossClass, gMonsters.m_BossClass, sizeof(info.CustomBossClass)); //Custom Boss Class
	
	WriteLog("Carregando CustomNPCName.txt...");
	gNPCName.Load("CustomManager\\CustomNPCName.txt");
	WriteLog("  - CustomNPCName.txt carregado");
	memcpy(info.CustomNPCName, gNPCName.m_CustomNpcName, sizeof(info.CustomNPCName));

	//==Pet
	WriteLog("Carregando arquivos de Pet...");
	gCustomPet2.Load("CustomManager\\CustomPet.txt"); //CustomPets
	WriteLog("  - CustomPet.txt carregado");
	memcpy(info.CustomPetInfo, gCustomPet2.m_CustomPetInfo, sizeof(info.CustomPetInfo)); //Custom Pets
	gCustomCEffectPet.Load("EffectManager\\CustomPetEffect.txt");
	WriteLog("  - CustomPetEffect.txt carregado");
	memcpy(info.m_PetCEffectBMD, gCustomCEffectPet.m_PetCEffect, sizeof(info.m_PetCEffectBMD));
	gCustomCEffectPet.LoadGlow("EffectManager\\CustomPetGlow.txt");
	WriteLog("  - CustomPetGlow.txt carregado");
	memcpy(info.RenderMeshPet, gCustomCEffectPet.m_RenderMeshPet, sizeof(info.RenderMeshPet));

	WriteLog("Carregando CustomMapList.txt...");
	g_pUIMapName.OpenScritp("CustomManager\\CustomMapList.txt");
	WriteLog("  - CustomMapList.txt carregado");
	memcpy(info.m_MapInfo, g_pUIMapName.m_MapInfo, sizeof(info.m_MapInfo));

	WriteLog("Carregando CustomBuyVip.txt...");
	gCustomBuyVip.Load("CommonManager\\CustomBuyVip.txt");
	WriteLog("  - CustomBuyVip.txt carregado");
	memcpy(info.CustomBuyVipInfo, gCustomBuyVip.m_CustomBuyVipInfo, sizeof(info.CustomBuyVipInfo));

	memcpy(info.EngCustomMessageInfo, gCustomMessage.m_EngCustomMessageInfo, sizeof(info.EngCustomMessageInfo));

	memcpy(info.VtmCustomMessageInfo, gCustomMessage.m_VtmCustomMessageInfo, sizeof(info.VtmCustomMessageInfo));
	//
	memcpy(info.CustomJewelInfo, gCustomJewel.m_CustomJewelInfo, sizeof(info.CustomJewelInfo));
	//
	memcpy(info.m_LucChienDataItem, gCustomLucChien.mLucChienDataItem, sizeof(info.m_LucChienDataItem));
	//
	memcpy(info.CustomWingInfo, gCustomWing.m_CustomWingInfo, sizeof(info.CustomWingInfo));
	//
	memcpy(info.m_CustomCloak, gCloak.m_Cloak, sizeof(info.m_CustomCloak));

	//
	memcpy(info.CustomRingPenInfo, gCustomRingPen.m_CustomRingPenInfo, sizeof(info.CustomRingPenInfo));
	//
	memcpy(info.m_CustomCEffect, gCloak.m_CEffect, sizeof(info.m_CustomCEffect));
	//
	memcpy(info.CustomItemInfo, gCustomItem.m_CustomItemInfo, sizeof(info.CustomItemInfo));
	//
	memcpy(info.CustomWingEffectInfo, gCustomWingEffect.m_CustomWingEffectInfo, sizeof(info.CustomWingEffectInfo));
	//
	memcpy(info.DynamicWingEffectInfo, gDynamicWingEffect.m_DynamicWingEffectInfo, sizeof(info.DynamicWingEffectInfo));
	//
	memcpy(info.CustomGloves, gCustomGloves.m_CustomGlovesInfo, sizeof(info.CustomGloves));
	//
	memcpy(info.CustomBowInfo, gCustomCrossBow.m_CustomBowInfo, sizeof(info.CustomBowInfo));
	//
	memcpy(info.CustomPosition, gCustomPosition.m_CustomPositionInfo, sizeof(info.CustomPosition));

	WriteLog("Calculando CRC32 dos arquivos...");
	CCRC32 CRC32;

	if (CRC32.FileCRC(info.ClientName, &info.ClientCRC32, 1024) == 0)
	{
		WriteLog("  - AVISO: Nao foi possivel calcular CRC32 de %s", info.ClientName);
		info.ClientCRC32 = 0;
	}
	else
	{
		WriteLog("  - CRC32 de %s: 0x%08X", info.ClientName, info.ClientCRC32);
	}

	if (CRC32.FileCRC(info.PluginName, &info.PluginCRC32, 1024) == 0)
	{
		WriteLog("  - AVISO: Nao foi possivel calcular CRC32 de %s", info.PluginName);
		info.PluginCRC32 = 0;
	}
	else
	{
		WriteLog("  - CRC32 de %s: 0x%08X", info.PluginName, info.PluginCRC32);
	}

	if (CRC32.FileCRC(info.CameraName, &info.CameraCRC32, 1024) == 0)
	{
		WriteLog("  - AVISO: Nao foi possivel calcular CRC32 de %s", info.CameraName);
		info.CameraCRC32 = 0;
	}
	else
	{
		WriteLog("  - CRC32 de %s: 0x%08X", info.CameraName, info.CameraCRC32);
	}

	WriteLog("Criptografando dados...");
	for (int n = 0; n <  sizeof(MAIN_FILE_INFO); n++)
	{
		((BYTE*)& info)[n] ^= (BYTE)(0xCA ^ LOBYTE(n));
		((BYTE*)& info)[n] += (BYTE)(0x95 ^ HIBYTE(n));
	}
	WriteLog("Dados criptografados. Tamanho: %d bytes", sizeof(MAIN_FILE_INFO));

	char dirPath[MAX_PATH];
	GetPrivateProfileString("MainInfo", "DuongDan", "", dirPath, sizeof(dirPath), ".\\MainInfo.ini");
	WriteLog("Diretorio de destino lido do INI: [%s]", dirPath);

	if (strlen(dirPath) == 0)
	{
		WriteLog("ERRO: Diretorio de destino (DuongDan) esta vazio no MainInfo.ini!");
		return;
	}

	// Fazer cópia do dirPath porque strtok modifica a string original
	char dirPathCopy[MAX_PATH];
	strcpy_s(dirPathCopy, sizeof(dirPathCopy), dirPath);

	// Tách các phần con của đường dẫn bằng dấu "/"
	char* token = strtok(dirPathCopy, "/");
	char fullDirPath[MAX_PATH] = "";
	
	WriteLog("Criando estrutura de diretorios...");

	while (token != NULL)
	{
		strcat(fullDirPath, token);
		WriteLog("  - Tentando criar diretorio: %s", fullDirPath);

		if (!CreateDirectoryA(fullDirPath, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
		{
			WriteLog("  - ERRO ao criar diretorio %s. Codigo de erro: %d", fullDirPath, GetLastError());
		}
		else
		{
			WriteLog("  - Diretorio %s OK", fullDirPath);
		}

		strcat(fullDirPath, "\\");

		token = strtok(NULL, "/");
	}

	char filePath[MAX_PATH];
	sprintf(filePath, "%sPegasusMain.bin", fullDirPath);
	WriteLog("Caminho completo do arquivo: %s", filePath);

	if (!CreateDirectoryA(dirPath, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
	{
		WriteLog("ERRO: Nao foi possivel criar o diretorio final: %s", dirPath);
	}

	WriteLog("Tentando criar arquivo PegasusMain.bin...");
	HANDLE file = CreateFileA(filePath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, 0);
	if (file != INVALID_HANDLE_VALUE)
	{
		WriteLog("SUCESSO: Arquivo criado: %s", filePath);
	}
	else
	{
		WriteLog("ERRO: Falha ao criar arquivo. Codigo de erro: %d", GetLastError());
		WriteLog("Caminho tentado: %s", filePath);
	}

	if (file == INVALID_HANDLE_VALUE)
	{
		WriteLog("ERRO: Handle invalido. Abortando escrita do arquivo.");
		return;
	}

	DWORD OutSize = 0;
	WriteLog("Escrevendo dados no arquivo...");

	if (WriteFile(file, &info, sizeof(MAIN_FILE_INFO), &OutSize, 0) == 0)
	{
		WriteLog("ERRO: Falha ao escrever no arquivo. Codigo de erro: %d", GetLastError());
		CloseHandle(file);
		return;
	}

	WriteLog("SUCESSO: %d bytes escritos no arquivo", OutSize);
	CloseHandle(file);
	WriteLog("=== FIM GetMainInfo() ===");
}

void GetTextInfo()
{
	WriteLog("=== INICIO GetTextInfo() ===");
	
	//===Text Info
	TEXT_FILE_INFO info;
	memset(&info, 0, sizeof(info));
	WriteLog("Carregando arquivos de Text Info...");
	gCBuffIcon.Load("CommonManager\\CustomBuffEffect_ENG.txt", 0);
	WriteLog("  - CustomBuffEffect_ENG.txt carregado");
	gCBuffIcon.Load("CommonManager\\CustomBuffEffect_VTM.txt", 1);
	WriteLog("  - CustomBuffEffect_VTM.txt carregado");
	memcpy(info.m_TooltipTRSDataEng, gCBuffIcon.m_TooltipENG, sizeof(info.m_TooltipTRSDataEng));
	memcpy(info.m_TooltipTRSDataVTM, gCBuffIcon.m_TooltipVTM, sizeof(info.m_TooltipTRSDataVTM));

	GInfo.Load("ItemManager\\CustomItemToolTip.txt");
	WriteLog("  - CustomItemToolTip.txt carregado");
	GInfo.Load2("ItemManager\\CustomTextTooltip.txt");
	WriteLog("  - CustomTextTooltip.txt carregado");

	memcpy(info.m_TRSTooltipData, GInfo.n_TRSTooltipData, sizeof(info.m_TRSTooltipData));
	memcpy(info.m_TRSTooltipText, GInfo.n_TRSTooltipText, sizeof(info.m_TRSTooltipText));

	WriteLog("Criptografando dados de texto...");
	for (int n = 0; n < sizeof(TEXT_FILE_INFO); n++)
	{
		((BYTE*)& info)[n] ^= (BYTE)(0xCA ^ LOBYTE(n));
		((BYTE*)& info)[n] += (BYTE)(0x95 ^ HIBYTE(n));
	}
	WriteLog("Dados criptografados. Tamanho: %d bytes", sizeof(TEXT_FILE_INFO));

	char dirPath[MAX_PATH];
	GetPrivateProfileString("MainInfo", "DuongDan", "", dirPath, sizeof(dirPath), ".\\MainInfo.ini");
	WriteLog("Diretorio de destino lido do INI: [%s]", dirPath);

	if (strlen(dirPath) == 0)
	{
		WriteLog("ERRO: Diretorio de destino (DuongDan) esta vazio no MainInfo.ini!");
		return;
	}

	// Fazer cópia do dirPath porque strtok modifica a string original
	char dirPathCopy[MAX_PATH];
	strcpy_s(dirPathCopy, sizeof(dirPathCopy), dirPath);

	// Tách các phần con của đường dẫn bằng dấu "/"
	char* token = strtok(dirPathCopy, "/");
	char fullDirPath[MAX_PATH] = "";
	
	WriteLog("Criando estrutura de diretorios...");

	while (token != NULL) 
	{
		strcat(fullDirPath, token);
		WriteLog("  - Tentando criar diretorio: %s", fullDirPath);

		if (!CreateDirectoryA(fullDirPath, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) 
		{
			WriteLog("  - ERRO ao criar diretorio %s. Codigo de erro: %d", fullDirPath, GetLastError());
		}
		else
		{
			WriteLog("  - Diretorio %s OK", fullDirPath);
		}

		strcat(fullDirPath, "\\");

		token = strtok(NULL, "/");
	}

	char filePath[MAX_PATH];
	sprintf(filePath, "%sPegasusText.bin", fullDirPath);
	WriteLog("Caminho completo do arquivo: %s", filePath);

	if (!CreateDirectoryA(dirPath, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
	{
		WriteLog("ERRO: Nao foi possivel criar o diretorio final: %s", dirPath);
	}

	WriteLog("Tentando criar arquivo PegasusText.bin...");
	HANDLE file = CreateFileA(filePath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, 0);
	if (file != INVALID_HANDLE_VALUE)
	{
		WriteLog("SUCESSO: Arquivo criado: %s", filePath);
	}
	else 
	{
		WriteLog("ERRO: Falha ao criar arquivo. Codigo de erro: %d", GetLastError());
		WriteLog("Caminho tentado: %s", filePath);
	}

	if (file == INVALID_HANDLE_VALUE)
	{
		WriteLog("ERRO: Handle invalido. Abortando escrita do arquivo.");
		return;
	}

	DWORD OutSize = 0;
	WriteLog("Escrevendo dados no arquivo...");

	if (WriteFile(file, &info, sizeof(TEXT_FILE_INFO), &OutSize, 0) == 0)
	{
		WriteLog("ERRO: Falha ao escrever no arquivo. Codigo de erro: %d", GetLastError());
		CloseHandle(file);
		return;
	}

	WriteLog("SUCESSO: %d bytes escritos no arquivo", OutSize);
	CloseHandle(file);
	WriteLog("=== FIM GetTextInfo() ===");
}

int _tmain(int argc, _TCHAR* argv[]) // OK
{
	// Limpar arquivo de log anterior
	FILE* logFile = NULL;
	fopen_s(&logFile, "GetMainInfo.log", "w");
	if (logFile)
	{
		fclose(logFile);
	}

	WriteLog("========================================");
	WriteLog("GetMainInfo.exe - Iniciando execucao");
	WriteLog("========================================");
	
	char currentDir[MAX_PATH];
	if (GetCurrentDirectoryA(MAX_PATH, currentDir) != 0)
	{
		WriteLog("Diretorio de trabalho atual: %s", currentDir);
	}
	else
	{
		WriteLog("AVISO: Nao foi possivel obter o diretorio atual");
	}

	CLEAR_START

	ENCODE_START
	
	WriteLog("Chamando GetMainInfo()...");
	GetMainInfo(); 
	
	WriteLog("Chamando GetTextInfo()...");
	GetTextInfo();

	ENCODE_END

	CLEAR_END
	
	WriteLog("========================================");
	WriteLog("Execucao concluida com sucesso!");
	WriteLog("========================================");
	
	printf("Mu Pegasus \n");
	int countdown = 1;
	while (countdown >= 0) {
		printf("Close : %d Second\n", countdown);
		Sleep(1000); // Delay for 1 second (1000 milliseconds)
		countdown--;
	}

	
	return 0;
}
