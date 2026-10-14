#pragma once
#include "User.h"
#include <fstream>
#if(Pegasus_AutoOff)

struct OfflineMode_Data
{
	DWORD	HuntingRange;
	BYTE	RecoveryPotionOn;
	BYTE	RecoveryPotionPercent;
	BYTE	RecoveryHealOn;			//-> CLASS_ELF
	BYTE	RecoveryHealPercent;	//-> CLASS_ELF
	BYTE	RecoveryDrainOn;		//-> CLASS_SUMMONER
	BYTE	RecoveryDrainPercent;	//-> CLASS_SUMMONER
	BYTE	DistanceLongOn;
	BYTE	DistanceReturnOn;
	DWORD	DistanceMin;
	DWORD	SkillBasicID;
	BYTE	ComboOn;				//-> CLASS_KNIGHT
	BYTE	PartyModeOn;
	BYTE	PartyModeHealOn;		//-> CLASS_ELF
	BYTE	PartyModeHealPercent;	//-> CLASS_ELF
	BYTE	PartyModeBuffOn;
	BYTE	BuffOn;
	DWORD	BuffSkill[3];
	DWORD	ObtainRange;
	BYTE	ObtainRepairOn;
	BYTE	ObtainPickNear;
	BYTE	ObtainPickSelected;
	BYTE	ObtainPickJewels;
	BYTE	ObtainPickAncient;
	BYTE	ObtainPickMoney;
	BYTE	ObtainPickExcellent;
};
struct OFFATTACK_DATA
{
	char Account[11];
	char Password[11];
	char Name[11];
	char IP[16];
	OfflineMode_Data  OfflineModeData;
};
class CBOfflindeRestore
{
public:
	CBOfflindeRestore();
	virtual ~CBOfflindeRestore();
	void CBOfflindeRestore::LoadData();
	void RestoreOffline();
	OFFATTACK_DATA* GetOffAttackInfo(LPOBJ lpObj);
	OFFATTACK_DATA* GetOffAttackInfoByAccount(LPOBJ lpObj);
	void SetOfflineModeData(LPOBJ lpObj);
	void AddToList(LPOBJ lpObj);
	void DelFromList(char* account);
	void CBOfflindeRestore::WriteToFile();
	std::map<std::string, OFFATTACK_DATA> m_Data;
	int AccountsRestored;

};

extern CBOfflindeRestore gCBOfflindeRestore;
#endif