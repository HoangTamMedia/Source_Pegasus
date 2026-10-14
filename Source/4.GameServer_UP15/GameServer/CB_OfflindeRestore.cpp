#include "StdAfx.h"
#include "CB_OfflindeRestore.h"
#include "Util.h"
#include "MemScript.h"
#include "CustomAttack.h"
#include "CommandManager.h"
#include "EffectManager.h"
#include "GameMain.h"
#include "ItemManager.h"
#include "Util.h"
#include "Map.h"
#include "MapManager.h"
#include "MasterSkillTree.h"
#include "Message.h"
#include "Notice.h"
#include "Party.h"
#include "SkillManager.h"
#include "SocketManager.h"
#include "Viewport.h"
#include "OfflineMode.h"
#include "JSProtocol.h"
#include "Helper.h"
#include <sstream>
#include <iomanip>
#include "ObjectManager.h"
#if(Pegasus_AutoOff)
char* PatchFileCache = ".\\Data\\Dialog.db";
CBOfflindeRestore gCBOfflindeRestore;

CBOfflindeRestore::CBOfflindeRestore()
{
	this->m_Data.clear();
	this->AccountsRestored = 0;
}


CBOfflindeRestore::~CBOfflindeRestore()
{
}
std::string HexToString(const std::string& hexString)
{
	std::string byteString;
	byteString.reserve(hexString.length() / 2);
	for (size_t i = 0; i < hexString.length(); i += 2)
	{
		std::string byteStr = hexString.substr(i, 2);
		byteString.push_back(static_cast<char>(std::stoi(byteStr, nullptr, 16)));
	}
	return byteString;
}
std::string SerializeDataToHex(const OfflineMode_Data& data)
{
	const char* byteData = reinterpret_cast<const char*>(&data);
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (size_t i = 0; i < sizeof(data); i++)
	{
		ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(byteData[i]));
	}
	return ss.str();
}

void DeserializeDataFromHex(const std::string& hexString, OfflineMode_Data& data)
{
	const std::string& byteString = HexToString(hexString);
	char* byteData = reinterpret_cast<char*>(&data);
	std::copy(byteString.begin(), byteString.end(), byteData);
}


//void CBOfflindeRestore::LoadData()
//{
//	std::ifstream fin(PatchFileCache);
//	if (!fin)
//	{
//		ErrorMessageBox(MEM_SCRIPT_ERROR_CODE0, PatchFileCache);
//		return;
//	}
//	std::string line;
//	while (std::getline(fin, line))
//	{
//		if (line == "end")
//		{
//			break;
//		}
//		std::istringstream iss(line);
//		OFFATTACK_DATA info;
//		std::string offlineModeData;
//		if (!(iss >> info.Account >> info.Password >> info.Name >> info.IP >> offlineModeData))
//		{
//			ErrorMessageBox(MEM_SCRIPT_ERROR_CODE2, PatchFileCache);
//			break;
//		}
//		DeserializeDataFromHex(offlineModeData, info.OfflineModeData);
//		this->m_Data.insert(std::pair<std::string, OFFATTACK_DATA>(info.Account, info));
//		LogAdd(LOG_BLUE, "[OfflindeRestore] INput Data [%s][%s] Skill [%d] !", info.Account, info.Name, info.OfflineModeData.SkillBasicID);
//	}
//	fin.close();
//
//	LogAdd(LOG_BLUE, "[OfflindeRestore] Load OK !");
//
//}

void CBOfflindeRestore::LoadData()
{
	this->m_Data.clear();
	this->AccountsRestored = 0;
	char* path = PatchFileCache;
	std::ifstream fin(PatchFileCache);
	if (!fin)
	{
		ErrorMessageBox(MEM_SCRIPT_ERROR_CODE0, PatchFileCache);
		return;
	}


	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			OFFATTACK_DATA info;

			strcpy_s(info.Account, lpMemScript->GetString());
			strcpy_s(info.Password, lpMemScript->GetAsString());
			strcpy_s(info.Name, lpMemScript->GetAsString());
			strcpy_s(info.IP, lpMemScript->GetAsString());
			char GetDataOfflineMode[255];
			strcpy_s(GetDataOfflineMode, lpMemScript->GetAsString());
			DeserializeDataFromHex(GetDataOfflineMode, info.OfflineModeData);
			this->m_Data.insert(std::pair<std::string, OFFATTACK_DATA>(info.Account, info));

		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}
void CBOfflindeRestore::RestoreOffline()
{
	for (std::map<std::string, OFFATTACK_DATA>::iterator it = this->m_Data.begin(); it != this->m_Data.end(); it++)
	{
		if (gObjFindByAcc(it->second.Account) != 0)
		{
			continue;
		}

		int aIndex = gObjAddSearch(0, it->second.IP);

		//LogAdd(LOG_TEST,"aIndex = %d",aIndex);
		if (aIndex >= 0)
		{
			gObjAdd(0, it->second.IP, aIndex);
			gObj[aIndex].LoginMessageSend++;
			gObj[aIndex].LoginMessageSend++;
			gObj[aIndex].LoginMessageCount++;
			gObj[aIndex].ConnectTickCount = GetTickCount();
			gObj[aIndex].ClientTickCount = GetTickCount();
			gObj[aIndex].ServerTickCount = GetTickCount();

			gObj[aIndex].MapServerMoveRequest = 0;
			gObj[aIndex].LastServerCode = -1;
			gObj[aIndex].DestMap = -1;
			gObj[aIndex].DestX = 0;
			gObj[aIndex].DestY = 0;
			GJConnectAccountSend(aIndex, it->second.Account, it->second.Password, it->second.IP);
			gObj[aIndex].Socket = INVALID_SOCKET;
			LogAdd(LOG_BLUE, "[ OfflindeRestore ]  [%s|%s] restored", it->second.Account, it->second.Name);

		}
	}
}

OFFATTACK_DATA* CBOfflindeRestore::GetOffAttackInfo(LPOBJ lpObj)
{
	std::map<std::string, OFFATTACK_DATA>::iterator it = this->m_Data.find(lpObj->Account);

	if (it != this->m_Data.end())
	{
		if (strcmp(lpObj->Name, it->second.Name) == 0)
		{
			return &it->second;
		}
	}

	return 0;
}

OFFATTACK_DATA* CBOfflindeRestore::GetOffAttackInfoByAccount(LPOBJ lpObj)
{
	std::map<std::string, OFFATTACK_DATA>::iterator it = this->m_Data.find(lpObj->Account);

	if (it != this->m_Data.end())
	{
		return &it->second;
	}

	return 0;
}
void CBOfflindeRestore::SetOfflineModeData(LPOBJ lpObj)
{
	OFFATTACK_DATA* info = gCBOfflindeRestore.GetOffAttackInfo(lpObj);//

	if (info != 0 && lpObj->Socket == INVALID_SOCKET)
	{
		lpObj->HuntingRange = info->OfflineModeData.HuntingRange;
		//--
		lpObj->RecoveryPotionOn = info->OfflineModeData.RecoveryPotionOn;
		lpObj->RecoveryPotionPercent = info->OfflineModeData.RecoveryPotionPercent;
		lpObj->RecoveryHealOn = (lpObj->Class == CLASS_FE) ? info->OfflineModeData.RecoveryHealOn : 0; //-> CLASS_ELF
		lpObj->RecoveryHealPercent = (lpObj->Class == CLASS_FE) ? info->OfflineModeData.RecoveryHealPercent : 0; //-> CLASS_ELF
		lpObj->RecoveryDrainOn = (lpObj->Class == CLASS_SU) ? info->OfflineModeData.RecoveryDrainOn : 0; //-> CLASS_SUMMONER
		lpObj->RecoveryDrainPercent = (lpObj->Class == CLASS_SU) ? info->OfflineModeData.RecoveryDrainPercent : 0; //-> CLASS_SUMMONER
		//--
		lpObj->DistanceLongOn = info->OfflineModeData.DistanceLongOn;
		lpObj->DistanceReturnOn = info->OfflineModeData.DistanceReturnOn;
		lpObj->DistanceMin = info->OfflineModeData.DistanceMin;
		lpObj->SkillBasicID = info->OfflineModeData.SkillBasicID;
		lpObj->ComboOn = (lpObj->Class == CLASS_DK) ? info->OfflineModeData.ComboOn : 0;				//-> CLASS_KNIGHT
		//--
		lpObj->PartyModeOn = info->OfflineModeData.PartyModeOn;
		lpObj->PartyModeHealOn = (lpObj->Class == CLASS_FE) ? info->OfflineModeData.PartyModeHealOn : 0;		//-> CLASS_ELF
		lpObj->PartyModeHealPercent = (lpObj->Class == CLASS_FE) ? info->OfflineModeData.PartyModeHealPercent : 0;	//-> CLASS_ELF
		lpObj->PartyModeBuffOn = info->OfflineModeData.PartyModeBuffOn;
		//--
		lpObj->BuffOn = info->OfflineModeData.BuffOn;
		lpObj->BuffSkill[0] = info->OfflineModeData.BuffSkill[0];
		lpObj->BuffSkill[1] = info->OfflineModeData.BuffSkill[1];
		lpObj->BuffSkill[2] = info->OfflineModeData.BuffSkill[2];
		lpObj->ObtainRange = (DWORD)info->OfflineModeData.ObtainRange;
		lpObj->ObtainRepairOn = info->OfflineModeData.ObtainRepairOn;
		lpObj->ObtainPickNear = (info->OfflineModeData.ObtainPickNear != 0) ? 1 : 0;
		lpObj->ObtainPickSelected = info->OfflineModeData.ObtainPickSelected;
		lpObj->ObtainPickJewels = (info->OfflineModeData.ObtainPickJewels != 0) ? 1 : 0;
		lpObj->ObtainPickAncient = (info->OfflineModeData.ObtainPickAncient != 0) ? 1 : 0;
		lpObj->ObtainPickMoney = (info->OfflineModeData.ObtainPickMoney != 0) ? 1 : 0;
		lpObj->ObtainPickExcellent = (info->OfflineModeData.ObtainPickExcellent != 0) ? 1 : 0;
		lpObj->ObtainPickExtra = 0;
		for (int i = 0; i < lpObj->ObtainPickExtraCount; ++i)
		{
			memset(lpObj->ObtainPickItemList[i], 0, sizeof(lpObj->ObtainPickItemList[i]));
		}

		lpObj->m_OfflineCoordX = lpObj->X;
		lpObj->m_OfflineCoordY = lpObj->Y;
		lpObj->m_OfflineTimeResetMove = GetTickCount();
		lpObj->AttackCustomDelay = GetTickCount();
		lpObj->MuOffHelperTime = g_OfflineMode.m_MUOffHelperTimer[lpObj->AccountLevel] * 600;
		lpObj->m_OfflineMode = 1;

		lpObj->orderskill = 0;
		LogAdd(LOG_BLUE, "[OfflindeRestore][Obj][%d] AddCharacterInfo (%s) OK Skill (%d) Range (%d)", lpObj->Index, lpObj->Name, info->OfflineModeData.SkillBasicID, info->OfflineModeData.HuntingRange);
		gObjectManager.SaveLucChien(lpObj->Index);
	}
}

void CBOfflindeRestore::AddToList(LPOBJ lpObj)
{
	if (this->m_Data.find(lpObj->Account) != this->m_Data.end())
	{
		return;
	}

	OFFATTACK_DATA info;

	strcpy_s(info.Account, lpObj->Account);
	strcpy_s(info.Password, lpObj->Password);
	//strcpy_s(info.Password, "123456789");
	strcpy_s(info.Name, lpObj->Name);
	strcpy_s(info.IP, lpObj->IpAddr);
	//===INfo offlinemode
	info.OfflineModeData.HuntingRange = lpObj->HuntingRange;
	//--
	info.OfflineModeData.RecoveryPotionOn = lpObj->RecoveryPotionOn;
	info.OfflineModeData.RecoveryPotionPercent = lpObj->RecoveryPotionPercent;
	info.OfflineModeData.RecoveryHealOn = (lpObj->Class == CLASS_FE) ? lpObj->RecoveryHealOn : 0; //-> CLASS_ELF
	info.OfflineModeData.RecoveryHealPercent = (lpObj->Class == CLASS_FE) ? lpObj->RecoveryHealPercent : 0; //-> CLASS_ELF
	info.OfflineModeData.RecoveryDrainOn = (lpObj->Class == CLASS_SU) ? lpObj->RecoveryDrainOn : 0; //-> CLASS_SUMMONER
	info.OfflineModeData.RecoveryDrainPercent = (lpObj->Class == CLASS_SU) ? lpObj->RecoveryDrainPercent : 0; //-> CLASS_SUMMONER
	//--
	info.OfflineModeData.DistanceLongOn = lpObj->DistanceLongOn;
	info.OfflineModeData.DistanceReturnOn = lpObj->DistanceReturnOn;
	info.OfflineModeData.DistanceMin = lpObj->DistanceMin;
	info.OfflineModeData.SkillBasicID = lpObj->SkillBasicID;
	info.OfflineModeData.ComboOn = (lpObj->Class == CLASS_DK) ? lpObj->ComboOn : 0;				//-> CLASS_KNIGHT
	//--
	info.OfflineModeData.PartyModeOn = lpObj->PartyModeOn;
	info.OfflineModeData.PartyModeHealOn = (lpObj->Class == CLASS_FE) ? lpObj->PartyModeHealOn : 0;		//-> CLASS_ELF
	info.OfflineModeData.PartyModeHealPercent = (lpObj->Class == CLASS_FE) ? lpObj->PartyModeHealPercent : 0;	//-> CLASS_ELF
	info.OfflineModeData.PartyModeBuffOn = lpObj->PartyModeBuffOn;
	//--
	info.OfflineModeData.BuffOn = lpObj->BuffOn;
	info.OfflineModeData.BuffSkill[0] = lpObj->BuffSkill[0];
	info.OfflineModeData.BuffSkill[1] = lpObj->BuffSkill[1];
	info.OfflineModeData.BuffSkill[2] = lpObj->BuffSkill[2];
	info.OfflineModeData.ObtainRange = (DWORD)lpObj->ObtainRange;
	info.OfflineModeData.ObtainRepairOn = lpObj->ObtainRepairOn;
	info.OfflineModeData.ObtainPickNear = (lpObj->ObtainPickNear != 0) ? 1 : 0;
	info.OfflineModeData.ObtainPickSelected = lpObj->ObtainPickSelected;
	info.OfflineModeData.ObtainPickJewels = (lpObj->ObtainPickJewels != 0) ? 1 : 0;
	info.OfflineModeData.ObtainPickAncient = (lpObj->ObtainPickAncient != 0) ? 1 : 0;
	info.OfflineModeData.ObtainPickMoney = (lpObj->ObtainPickMoney != 0) ? 1 : 0;
	info.OfflineModeData.ObtainPickExcellent = (lpObj->ObtainPickExcellent != 0) ? 1 : 0;
	this->m_Data.insert(std::pair<std::string, OFFATTACK_DATA>(info.Account, info));

	this->WriteToFile();
}


void CBOfflindeRestore::DelFromList(char* account)
{
	std::map<std::string, OFFATTACK_DATA>::iterator it = this->m_Data.find(account);

	if (it != this->m_Data.end())
	{
		this->m_Data.erase(it);
		LogAdd(LOG_BLACK, "[ OfflindeRestore ]  [%s|%s] DelFromList", it->second.Account, it->second.Name);
	}

	this->WriteToFile();
}

void CBOfflindeRestore::WriteToFile()
{
	std::ofstream fout(PatchFileCache, std::ios_base::out | std::ios_base::trunc);

	for (std::map<std::string, OFFATTACK_DATA>::iterator it = this->m_Data.begin(); it != this->m_Data.end(); it++)
	{
		fout << "\"" << it->second.Account << "\"" << "\t"
			<< "\"" << it->second.Password << "\"" << "\t"
			<< "\"" << it->second.Name << "\"" << "\t"
			<< "\"" << it->second.IP << "\"" << "\t"
			<< "\"" << SerializeDataToHex(it->second.OfflineModeData) << "\"" << std::endl;

		//fout << it->second.Account << "\t"
		//	<< it->second.Password << "\t"
		//	<< it->second.Name << "\t"
		//	<< it->second.IP << "\t"
		//	<< SerializeDataToHex(it->second.OfflineModeData) << std::endl;
	}

	fout << "end";

	fout.close(); //
}

#endif