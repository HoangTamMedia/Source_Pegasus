#include "stdafx.h"
#include "Protocol.h"
#include "WSclient.h"
#include "NewUISystem.h"
#include "BuffIcon.h"
#include "PetProtocol.h"
#include "Reconnect.h"
#include "WSclient.h"
#include "wsclientinline.h"
#include "SkillManager.h"
#include "CharacterManager.h"
#include "CustomEventTime.h"
#include "CustomRankUser.h"
#include "CustomRanking.h"
#include "CBInterface.h"
#include "LoginWin.h"
#include "CBInterfaceVIPChar.h"
#include "CustomLucChien.h"
#include "StatsAdvance.h"
#include "ChangeClass.h"

bool StatusAutoReset = false;
BOOL ProtocolCoreEx(BYTE head, BYTE* lpMsg, int size, int key)
{
	switch (head)
	{
	case 0x11:
		GCDamageRecv((PMSG_DAMAGE_RECV*)lpMsg);
		break;
	case 0x16:
		GCMonsterDieRecv((PMSG_MONSTER_DIE_RECV*)lpMsg);
		break;
	case 0x17:
		GCUserDieRecv((PMSG_USER_DIE_RECV*)lpMsg);
		break;
	case 0x26:
		GCLifeRecv((PMSG_LIFE_RECV*)lpMsg);
		break;
	case 0x27:
		GCManaRecv((PMSG_MANA_RECV*)lpMsg);
		break;
	case 0x2C:
		GCFruitResultRecv((PMSG_FRUIT_RESULT_RECV*)lpMsg);
		break;
	case 0x3F:
		switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
		{
		case 0x10:

			break;
		}
		break;

	case 0x78:
		RecvPostItem(lpMsg);
		break;

	case 0x2D:
		gIconBuff.Recv((PMSG_SEND_BUFF_ICON*)lpMsg);
		break;
	case 0x9C:

		break;
	case 0xA3:

		break;
	case 0xB1:
		switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
		{
		case 0x00:
			gIconBuff.DeleteAllBuff();

			break;
		case 0x01:
			gIconBuff.DeleteAllBuff();

			break;
		}
		break;
	case 0xF1:
		switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
		{
		case 0x00:
			GCConnectClientRecv((PMSG_CONNECT_CLIENT_RECV*)lpMsg);
			break;
		}
		break;

	case 0xAE:
		CGLoadDataMuHelper(lpMsg);
		break;
	case 0xBF:
		switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
		{
		case 0x51:
			RecvStartMuHelper(lpMsg);
			break;
		}
		break;

	case 0xF3:
		switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
		{

		case 0x00:
			gIconBuff.DeleteAllBuff();

			break;
		case 0x03:
			GCCharacterInfoRecv(lpMsg);
			break;
		case 0x04:
			GCCharacterRegenRecv((PMSG_CHARACTER_REGEN_RECV*)lpMsg);
			break;
		case 0x05:
			GCLevelUpRecv((PMSG_LEVEL_UP_RECV*)lpMsg);
			break;
		case 0x06:
			GCLevelUpPointRecv((PMSG_LEVEL_UP_POINT_RECV*)lpMsg);
			break;
		case 0x07:
			GCMonsterDamageRecv((PMSG_MONSTER_DAMAGE_RECV*)lpMsg);
			break;
		case 0x50:
			GCMasterInfoRecv((PMSG_MASTER_INFO_RECV*)lpMsg);
			break;
		case 0x51:
			GCMasterLevelUpRecv((PMSG_MASTER_LEVEL_UP_RECV*)lpMsg);
			break;
		case 0xE0:
		{
			GCNewCharacterInfoRecv(lpMsg);
			return 1;
		}
		break;

		case 0xE1:
		{
			GCNewCharacterCalcRecv(lpMsg);
			return 1;
		}
		break;

		case 0xE2:
			ReceiveNewHealthBar(lpMsg);
			break;
		case 0xF1:
			GCPetCharSetRecv((PMSG_NEW_PET_CHARSET_RECV*)lpMsg);
			break;
		case 0xF2:
			GCPetCharSetSelectCharacterRecv((PMSG_NEW_SELECT_CHARACTER_CHARSET_RECV*)lpMsg);
			break;

		case 0xF4:

			return 1;
		case 0xE5:
			gCustomRankUser.GCReqRankLevelUser(lpMsg);
			return 1;
		case 0xE6:
			gCustomRanking.GCReqRanking((CustomRanking::PMSG_CUSTOM_RANKING_RECV*)lpMsg);
			return 1;
		case 0xE7:
			gCustomRanking.GCReqRankingCount((CustomRanking::PMSG_CUSTOM_RANKING_COUNT_RECV*)lpMsg);
			return 1;
		case 0xE9:

			return 1;
		case 0xEA:
			GCRecvCoin((PMSG_COIN_RECV*)lpMsg);
			return 1;
		case 0xEB:

			return 1;
		case 0xEC:

			return 1;
		case 0xED:

			break;
		case 0xF3:
			g_StatsAdvance.Recv((PMSG_STATS_ADVANCE*)lpMsg);
			return 1;
		case 0x13:
			gIconBuff.GC_BuffInfo((PMSG_SEND_BUFF_ICON_EX*)lpMsg);
			break;
		case 0xF9:

			break;
		}
		break;
	   case 0xFB:
		   switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
		   {
		   case 0x07:
			   gChangeClass.RecvData((PMSG_CHANGECLASS_DATA*)lpMsg);
			   break;
		   }
		   break;
		case 0xD3:
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{

			case 0x22:
				gBInterfaceVIPChar.GCSetInfoVIPChar((BPMSG_CUSTOM_VIPCHAR_SEND*)lpMsg);
				break;
			case 0x24:
				gBInterfaceVIPChar.GCSetInfoVIPCharList((CSEV_COUNTLIST*)lpMsg);
				break;
			}
		break;
		case 0xD7:
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
			case 0x41:
			{
				gSauEventTime.RecvListEventTime(lpMsg);
			}
			break;
			case 0x42:
			{
				gSauEventTime.RecvListItem(lpMsg);
			}
			break;
			}
			break;
		case 0xFA:
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
#if(JEWELBANKVER2)
			case 0x19:
			{

				PMSG_NEW_HEALTH_BAR_RECV* pMsg = (PMSG_NEW_HEALTH_BAR_RECV*)lpMsg;
				gInterface.mCListItemBank.clear();
				for (int n = 0; n < pMsg->count; n++)
				{
					USER_BANK_DATA* lpInfo = (USER_BANK_DATA*)(((BYTE*)lpMsg) + sizeof(PMSG_NEW_HEALTH_BAR_RECV) + (sizeof(USER_BANK_DATA) * n));
					USER_BANK_DATA info;
					info.ItemIndex = lpInfo->ItemIndex;
					info.ItemLevel = lpInfo->ItemLevel;
					info.ItemCount = lpInfo->ItemCount;
					info.AutoPick = lpInfo->AutoPick;
					gInterface.mCListItemBank.push_back(info);

				}
			}
			break;

#endif

			case 0x23:
			{
				DATA_LUCCHIEN* mRecv = (DATA_LUCCHIEN*)lpMsg;
				Hero->LucChienNhanVat = mRecv->LucChien;
				g_StatsAdvance.LucChien = mRecv->LucChien;
			}
			break;

			case 0x16:

				break;
			case 0x17:

				break;

			case 0xD4:

				break;
			}
			break;
	}
	return false;
}
void GCConnectClientRecv(PMSG_CONNECT_CLIENT_RECV* lpMsg)
{
	CharacterAttribute->PrintPlayer.ViewIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);
}

void GCDamageRecv(PMSG_DAMAGE_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (CharacterAttribute->PrintPlayer.ViewIndex == aIndex)
	{
		CharacterAttribute->PrintPlayer.ViewCurHP = lpMsg->ViewCurHP;
		CharacterAttribute->PrintPlayer.ViewCurSD = lpMsg->ViewCurSD;
	}

	CharacterAttribute->PrintPlayer.ViewDamageHP = lpMsg->ViewDamageHP;
	CharacterAttribute->PrintPlayer.ViewDamageSD = lpMsg->ViewDamageSD;

	if ((lpMsg->type & 0x10) != 0)
	{
		if (CharacterAttribute->PrintPlayer.ViewDamageCount < 3)
		{
			CharacterAttribute->PrintPlayer.ViewDamageTable[CharacterAttribute->PrintPlayer.ViewDamageCount++] = lpMsg->ViewDamageHP;
			CharacterAttribute->PrintPlayer.ViewDamageValue = CharacterAttribute->PrintPlayer.ViewDamageCount;
		}
	}

	if ((lpMsg->type & 0x20) != 0)
	{
		if (CharacterAttribute->PrintPlayer.ViewDamageCount < 4)
		{
			CharacterAttribute->PrintPlayer.ViewDamageTable[CharacterAttribute->PrintPlayer.ViewDamageCount++] = lpMsg->ViewDamageHP;
			CharacterAttribute->PrintPlayer.ViewDamageValue = CharacterAttribute->PrintPlayer.ViewDamageCount;
		}
	}
}

void GCMonsterDieRecv(PMSG_MONSTER_DIE_RECV* lpMsg)
{
	CharacterAttribute->PrintPlayer.ViewDamageHP = lpMsg->ViewDamageHP;
}

void GCUserDieRecv(PMSG_USER_DIE_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (CharacterAttribute->PrintPlayer.ViewIndex == aIndex)
	{
		CharacterAttribute->PrintPlayer.ViewCurHP = 0;
	}
}

void GCLifeRecv(PMSG_LIFE_RECV* lpMsg)
{
	if (lpMsg->type == 0xFE)
	{
		CharacterAttribute->PrintPlayer.ViewMaxHP = lpMsg->ViewHP;
		CharacterAttribute->PrintPlayer.ViewMaxSD = lpMsg->ViewSD;
	}

	if (lpMsg->type == 0xFF)
	{
		CharacterAttribute->PrintPlayer.ViewCurHP = ((CharacterAttribute->PrintPlayer.ViewCurHP == 0) ? CharacterAttribute->PrintPlayer.ViewCurHP : lpMsg->ViewHP);
		CharacterAttribute->PrintPlayer.ViewCurSD = lpMsg->ViewSD;
	}
}

void GCManaRecv(PMSG_MANA_RECV * lpMsg)
{
	if (lpMsg->type == 0xFE)
	{
		CharacterAttribute->PrintPlayer.ViewMaxMP = lpMsg->ViewMP;
		CharacterAttribute->PrintPlayer.ViewMaxBP = lpMsg->ViewBP;
	}

	if (lpMsg->type == 0xFF)
	{
		CharacterAttribute->PrintPlayer.ViewCurMP = lpMsg->ViewMP;
		CharacterAttribute->PrintPlayer.ViewCurBP = lpMsg->ViewBP;
	}
}

void GCFruitResultRecv(PMSG_FRUIT_RESULT_RECV * lpMsg)
{
	if (lpMsg->result == 0 || lpMsg->result == 3 || lpMsg->result == 6 || lpMsg->result == 17)
	{
		CharacterAttribute->PrintPlayer.ViewValue = lpMsg->ViewValue;
		CharacterAttribute->PrintPlayer.ViewPoint = lpMsg->ViewPoint;
		CharacterAttribute->PrintPlayer.ViewStrength = lpMsg->ViewStrength;
		CharacterAttribute->PrintPlayer.ViewDexterity = lpMsg->ViewDexterity;
		CharacterAttribute->PrintPlayer.ViewVitality = lpMsg->ViewVitality;
		CharacterAttribute->PrintPlayer.ViewEnergy = lpMsg->ViewEnergy;
		CharacterAttribute->PrintPlayer.ViewLeadership = lpMsg->ViewLeadership;
	}
}

void RecvPostItem(BYTE* ReceiveBuffer)
{

	PMSG_POSTITEM* lpMsg = (PMSG_POSTITEM*)ReceiveBuffer;

	ITEM* pNewItem = new ITEM;
	memset(pNewItem, 0, sizeof(ITEM));
	memcpy_s(pNewItem, sizeof(ITEM), lpMsg->item_data, sizeof(ITEM));
	g_pChatListBox->AddItemPost(pNewItem);

	g_ConsoleDebug->Write(MCD_RECEIVE, "[RecvPostItem] [%s] %s %d" , lpMsg->chatid, lpMsg->chatmsg, g_pChatListBox->GetCurrentRenderEndLine());
	char Name[11] = { '\0' };
	char Messase[61] = { '\0' };
	char Messase_API[128] = { '\0' };
	memcpy_s(&Name[0], 10, &lpMsg->chatid[0], 10);
	memcpy_s(&Messase[0], 60, &lpMsg->chatmsg[0], 60);

	Name[10] = '\0';
	Messase[60] = '\0';

	char* pFormat = gCustomMessage.GetMessage(87);
	if (pFormat != NULL && strlen(pFormat) > 0)
	{

		if (strstr(pFormat, "%s") != NULL)
		{
			sprintf_s(Messase_API, sizeof(Messase_API), pFormat, Messase);
		}
		else
		{

			sprintf_s(Messase_API, sizeof(Messase_API), "[POST] %s", Messase);
		}
	}
	else
	{

		sprintf_s(Messase_API, sizeof(Messase_API), "[POST] %s", Messase);
	}

	if (strlen(Messase_API) > 0)
	{
		g_pChatListBox->AddText(Name, Messase_API, SEASON3B::TYPE_UNION_MESSAGE);
	}
}

void CGLoadDataMuHelper(BYTE* ReceiveBuffer)
{
	PMSG_HELPER_DATA_SEND* lpMsg = (PMSG_HELPER_DATA_SEND*)ReceiveBuffer;

	BYTE BData[256] = { 0 };
	if (!lpMsg->result)
	{
		memcpy_s(BData, sizeof(BData), lpMsg->data, sizeof(BData));
		g_pNewUISystem->GetInstance()->Get_pNewUIMuHelper()->LoadConfig(BData);

	}
	else
	{
		g_pNewUISystem->GetInstance()->Get_pNewUIMuHelper()->ResetConfig();

	}
}

void CGSaveDataMuHelper(BYTE* ReceiveBuffer)
{
	PMSG_HELPER_DATA_SEND pMsg;

	pMsg.header.set(0xAE, sizeof(pMsg));

	pMsg.result = 1;

	memcpy(pMsg.data, ReceiveBuffer, sizeof(pMsg.data));

	BDataSend((BYTE*)& pMsg, sizeof(pMsg));

}

void StartMuHelper(int Type)
{

	if (Type == 0)
	{
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MuHelper))
		{
			SEASON3B::CreateOkMessageBox(GlobalText[3576]);
			return;
		}
		if (Hero->SafeZone)
		{
			SEASON3B::CreateOkMessageBox(GlobalText[3574]);
			return;
		}

		if (gSkillManager.FindHeroSkill((ActionSkillType)g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Skill[0]) == 0)
		{
			SEASON3B::CreateOkMessageBox(GlobalText[3577]);
			return;
		}

	}

	PMSG_HELPER_START_SEND pMsg;

	pMsg.header.set(0xBF, 0x51, sizeof(pMsg));

	pMsg.type = Type;

	BDataSend((BYTE*)& pMsg, pMsg.header.size);

}
void RecvStartMuHelper(BYTE* ReceiveBuffer)
{

	PMSG_HELPER_START_RECV* lpMsg = (PMSG_HELPER_START_RECV*)ReceiveBuffer;
	g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.TimerMoney = lpMsg->time;
	if (!lpMsg->result)
	{
		if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Started == 0)
		{
			g_pChatListBox->AddText("", GlobalText[3578], SEASON3B::TYPE_ERROR_MESSAGE);
			g_pChatListBox->AddText("", GlobalText[3582], SEASON3B::TYPE_ERROR_MESSAGE);

			g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Started = 1;
			g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.StartX = Hero->PositionX;
			g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.StartY = Hero->PositionY;
			g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.TimerMove = GetTickCount();

			if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.DarkSpirit)
			{
				PMSG_PET_ITEM_COMMAND_SEND pMsg;

				pMsg.header.set(0xA7, sizeof(pMsg));
				pMsg.type = 0;
				pMsg.command = 3;
				pMsg.index[0] = SET_NUMBERHB(-1);
				pMsg.index[1] = SET_NUMBERLB(-1);

				BDataSend((BYTE*)& pMsg, pMsg.header.size);
			}
		}
		else
		{
			char zbtemp[128] = { 0 };
			sprintf_s(zbtemp, GlobalText[3586], lpMsg->money);
			g_pChatListBox->AddText("", zbtemp, SEASON3B::TYPE_ERROR_MESSAGE);
		}
	}
	else
	{
		g_pChatListBox->AddText("", GlobalText[3579], SEASON3B::TYPE_ERROR_MESSAGE);
		g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Clear();
	}

}

void ReceiveNewHealthBar(BYTE* ReceiveBuffer)
{

	PMSG_NEW_HEALTH_BAR_RECV* lpMsg = (PMSG_NEW_HEALTH_BAR_RECV*)ReceiveBuffer;

	for (int n = 0; n < lpMsg->count; n++)
	{
		PMSG_NEW_HEALTH_RECV* lpInfo = (PMSG_NEW_HEALTH_RECV*)(((BYTE*)lpMsg) + sizeof(PMSG_NEW_HEALTH_BAR_RECV) + (sizeof(PMSG_NEW_HEALTH_RECV) * n));

		int Index = FindCharacterIndex(lpInfo->index);
		if (Index != MAX_CHARACTERS_CLIENT)
		{

			CHARACTER* c = &CharactersClient[Index];
			if (!c) continue;
			c->InfoHealBar.Clear();
			c->InfoHealBar.Type = lpInfo->type;
			c->InfoHealBar.Life = lpInfo->rate;
			c->InfoHealBar.SD = lpInfo->rate2;

			c->InfoHealBar.MaxLife = lpInfo->MaxLife;
			c->InfoHealBar.MaxSD = lpInfo->MaxSD;

			c->InfoHealBar.Level = lpInfo->Level;

		}

	}
}

void GCCharacterInfoRecv(BYTE* ReceiveBuffer)
{
	PRECEIVE_JOIN_MAP_SERVER* lpMsg = (PRECEIVE_JOIN_MAP_SERVER*)ReceiveBuffer;
#if(UseReconnect)
	g_pReconnect->ReconnectOnCharacterInfo();
#endif
	CharacterAttribute->PrintPlayer.ViewReset = lpMsg->ViewReset;
	CharacterAttribute->PrintPlayer.ViewMasterReset = lpMsg->ViewGR;
	CharacterAttribute->PrintPlayer.ViewPoint = lpMsg->ViewPoint;
	CharacterAttribute->PrintPlayer.ViewCurHP = lpMsg->ViewCurHP;
	CharacterAttribute->PrintPlayer.ViewMaxHP = lpMsg->ViewMaxHP;
	CharacterAttribute->PrintPlayer.ViewCurMP = lpMsg->ViewCurMP;
	CharacterAttribute->PrintPlayer.ViewMaxMP = lpMsg->ViewMaxMP;
	CharacterAttribute->PrintPlayer.ViewCurBP = lpMsg->ViewCurBP;
	CharacterAttribute->PrintPlayer.ViewMaxBP = lpMsg->ViewMaxBP;
	CharacterAttribute->PrintPlayer.ViewCurSD = lpMsg->ViewCurSD;
	CharacterAttribute->PrintPlayer.ViewMaxSD = lpMsg->ViewMaxSD;
	CharacterAttribute->PrintPlayer.ViewStrength = lpMsg->ViewStrength;
	CharacterAttribute->PrintPlayer.ViewDexterity = lpMsg->ViewDexterity;
	CharacterAttribute->PrintPlayer.ViewVitality = lpMsg->ViewVitality;
	CharacterAttribute->PrintPlayer.ViewEnergy = lpMsg->ViewEnergy;
	CharacterAttribute->PrintPlayer.ViewLeadership = lpMsg->ViewLeadership;
	CharacterAttribute->PrintPlayer.ViewAccountLevel = lpMsg->ViewAccountLevel;

}

void GCNewCharacterInfoRecv(BYTE* ReceiveBuffer)
{
	PMSG_NEW_CHARACTER_INFO_RECV* lpMsg = (PMSG_NEW_CHARACTER_INFO_RECV*)ReceiveBuffer;

	CharacterAttribute->Level = lpMsg->Level;
	CharacterAttribute->LevelUpPoint = lpMsg->LevelUpPoint;
	CharacterAttribute->Strength = lpMsg->Strength;
	CharacterAttribute->Dexterity = lpMsg->Dexterity;
	CharacterAttribute->Vitality = lpMsg->Vitality;
	CharacterAttribute->Energy = lpMsg->Energy;
	CharacterAttribute->Charisma = lpMsg->Leadership;
	CharacterAttribute->Life = lpMsg->Life;
	CharacterAttribute->LifeMax = lpMsg->MaxLife;
	CharacterAttribute->Mana = lpMsg->Mana;
	CharacterAttribute->ManaMax = lpMsg->MaxMana;
	CharacterAttribute->SkillMana = lpMsg->BP;
	CharacterAttribute->SkillManaMax = lpMsg->MaxBP;
	CharacterAttribute->Shield = lpMsg->Shield;
	CharacterAttribute->ShieldMax = lpMsg->MaxShield;
	CharacterAttribute->AddPoint = lpMsg->FruitAddPoint;
	CharacterAttribute->MaxAddPoint = lpMsg->MaxFruitAddPoint;
	CharacterAttribute->wMinusPoint = lpMsg->FruitSubPoint;
	CharacterAttribute->wMaxMinusPoint = lpMsg->MaxFruitSubPoint;
	CharacterAttribute->Experience = lpMsg->Experience;
	CharacterAttribute->NextExperince = lpMsg->NextExperience;

	CharacterAttribute->PrintPlayer.ViewReset = lpMsg->ViewReset;
	CharacterAttribute->PrintPlayer.ViewMasterReset = lpMsg->ViewGR;
	CharacterAttribute->PrintPlayer.ViewPoint = lpMsg->ViewPoint;
	CharacterAttribute->PrintPlayer.ViewCurHP = lpMsg->ViewCurHP;
	CharacterAttribute->PrintPlayer.ViewMaxHP = lpMsg->ViewMaxHP;
	CharacterAttribute->PrintPlayer.ViewCurMP = lpMsg->ViewCurMP;
	CharacterAttribute->PrintPlayer.ViewMaxMP = lpMsg->ViewMaxMP;
	CharacterAttribute->PrintPlayer.ViewCurBP = lpMsg->ViewCurBP;
	CharacterAttribute->PrintPlayer.ViewMaxBP = lpMsg->ViewMaxBP;
	CharacterAttribute->PrintPlayer.ViewCurSD = lpMsg->ViewCurSD;
	CharacterAttribute->PrintPlayer.ViewMaxSD = lpMsg->ViewMaxSD;
	CharacterAttribute->PrintPlayer.ViewStrength = lpMsg->ViewStrength;
	CharacterAttribute->PrintPlayer.ViewDexterity = lpMsg->ViewDexterity;
	CharacterAttribute->PrintPlayer.ViewVitality = lpMsg->ViewVitality;
	CharacterAttribute->PrintPlayer.ViewEnergy = lpMsg->ViewEnergy;
	CharacterAttribute->PrintPlayer.ViewLeadership = lpMsg->ViewLeadership;
	CharacterAttribute->PrintPlayer.ViewAccountLevel = lpMsg->ViewAccountLevel;

}
void GCNewCharacterCalcRecv(BYTE* ReceiveBuffer)
{
	PMSG_NEW_CHARACTER_CALC_RECV* lpMsg = (PMSG_NEW_CHARACTER_CALC_RECV*)ReceiveBuffer;
	CharacterAttribute->PrintPlayer.ViewCurHP = lpMsg->ViewCurHP;
	CharacterAttribute->PrintPlayer.ViewMaxHP = lpMsg->ViewMaxHP;
	CharacterAttribute->PrintPlayer.ViewCurMP = lpMsg->ViewCurMP;
	CharacterAttribute->PrintPlayer.ViewMaxMP = lpMsg->ViewMaxMP;
	CharacterAttribute->PrintPlayer.ViewCurBP = lpMsg->ViewCurBP;
	CharacterAttribute->PrintPlayer.ViewMaxBP = lpMsg->ViewMaxBP;
	CharacterAttribute->PrintPlayer.ViewCurSD = lpMsg->ViewCurSD;
	CharacterAttribute->PrintPlayer.ViewMaxSD = lpMsg->ViewMaxSD;
	CharacterAttribute->PrintPlayer.ViewAddStrength = lpMsg->ViewAddStrength;
	CharacterAttribute->PrintPlayer.ViewAddDexterity = lpMsg->ViewAddDexterity;
	CharacterAttribute->PrintPlayer.ViewAddVitality = lpMsg->ViewAddVitality;
	CharacterAttribute->PrintPlayer.ViewAddEnergy = lpMsg->ViewAddEnergy;
	CharacterAttribute->PrintPlayer.ViewAddLeadership = lpMsg->ViewAddLeadership;
	CharacterAttribute->PrintPlayer.ViewPhysiDamageMin = lpMsg->ViewPhysiDamageMin;
	CharacterAttribute->PrintPlayer.ViewPhysiDamageMax = lpMsg->ViewPhysiDamageMax;
	CharacterAttribute->PrintPlayer.ViewMagicDamageMin = lpMsg->ViewMagicDamageMin;
	CharacterAttribute->PrintPlayer.ViewMagicDamageMax = lpMsg->ViewMagicDamageMax;
	CharacterAttribute->PrintPlayer.ViewCurseDamageMin = lpMsg->ViewCurseDamageMin;
	CharacterAttribute->PrintPlayer.ViewCurseDamageMax = lpMsg->ViewCurseDamageMax;
	CharacterAttribute->PrintPlayer.ViewMulPhysiDamage = lpMsg->ViewMulPhysiDamage;
	CharacterAttribute->PrintPlayer.ViewDivPhysiDamage = lpMsg->ViewDivPhysiDamage;
	CharacterAttribute->PrintPlayer.ViewMulMagicDamage = lpMsg->ViewMulMagicDamage;
	CharacterAttribute->PrintPlayer.ViewDivMagicDamage = lpMsg->ViewDivMagicDamage;
	CharacterAttribute->PrintPlayer.ViewMulCurseDamage = lpMsg->ViewMulCurseDamage;
	CharacterAttribute->PrintPlayer.ViewDivCurseDamage = lpMsg->ViewDivCurseDamage;
	CharacterAttribute->PrintPlayer.ViewMagicDamageRate = lpMsg->ViewMagicDamageRate;
	CharacterAttribute->PrintPlayer.ViewCurseDamageRate = lpMsg->ViewCurseDamageRate;
	CharacterAttribute->PrintPlayer.ViewPhysiSpeed = lpMsg->ViewPhysiSpeed;
	CharacterAttribute->PrintPlayer.ViewMagicSpeed = lpMsg->ViewMagicSpeed;
	CharacterAttribute->PrintPlayer.ViewAttackSuccessRate = lpMsg->ViewAttackSuccessRate;
	CharacterAttribute->PrintPlayer.ViewAttackSuccessRatePvP = lpMsg->ViewAttackSuccessRatePvP;
	CharacterAttribute->PrintPlayer.ViewDefense = lpMsg->ViewDefense;
	CharacterAttribute->PrintPlayer.ViewDefenseSuccessRate = lpMsg->ViewDefenseSuccessRate;
	CharacterAttribute->PrintPlayer.ViewDefenseSuccessRatePvP = lpMsg->ViewDefenseSuccessRatePvP;
	CharacterAttribute->PrintPlayer.ViewDamageMultiplier = lpMsg->ViewDamageMultiplier;
	CharacterAttribute->PrintPlayer.ViewRFDamageMultiplierA = lpMsg->ViewRFDamageMultiplierA;
	CharacterAttribute->PrintPlayer.ViewRFDamageMultiplierB = lpMsg->ViewRFDamageMultiplierB;
	CharacterAttribute->PrintPlayer.ViewRFDamageMultiplierC = lpMsg->ViewRFDamageMultiplierC;
	CharacterAttribute->PrintPlayer.ViewDarkSpiritAttackDamageMin = lpMsg->ViewDarkSpiritAttackDamageMin;
	CharacterAttribute->PrintPlayer.ViewDarkSpiritAttackDamageMax = lpMsg->ViewDarkSpiritAttackDamageMax;
	CharacterAttribute->PrintPlayer.ViewDarkSpiritAttackSpeed = lpMsg->ViewDarkSpiritAttackSpeed;
	CharacterAttribute->PrintPlayer.ViewDarkSpiritAttackSuccessRate = lpMsg->ViewDarkSpiritAttackSuccessRate;
}

void GCPetCharSetRecv(PMSG_NEW_PET_CHARSET_RECV* lpMsg)
{
	ClearNewPetCharSet();

	for (int n = 0; n < lpMsg->count; n++)
	{
		NEW_PET_STRUCT* lpInfo = (NEW_PET_STRUCT*)(((BYTE*)lpMsg) + sizeof(PMSG_NEW_PET_CHARSET_RECV) + (sizeof(NEW_PET_STRUCT) * n));

		InsertNewPetCharSet(lpInfo->index, lpInfo->PetCharSet);
	}
}

void GCPetCharSetSelectCharacterRecv(PMSG_NEW_SELECT_CHARACTER_CHARSET_RECV * lpMsg)
{
	ClearNewPetCharSetSelectCharacter();

	for (int n = 0; n < lpMsg->count; n++)
	{
		PMSG_NEW_CHARACTER_CHARSET_RECV* lpInfo = (PMSG_NEW_CHARACTER_CHARSET_RECV*)(((BYTE*)lpMsg) + sizeof(PMSG_NEW_SELECT_CHARACTER_CHARSET_RECV) + (sizeof(PMSG_NEW_CHARACTER_CHARSET_RECV) * n));

		InsertNewPetCharSetSelectCharacter(lpInfo->Name, lpInfo->PetCharSet);
	}
}

void GCCharacterRegenRecv(PMSG_CHARACTER_REGEN_RECV* lpMsg)
{
	CharacterAttribute->PrintPlayer.ViewCurHP = lpMsg->ViewCurHP;
	CharacterAttribute->PrintPlayer.ViewCurMP = lpMsg->ViewCurMP;
	CharacterAttribute->PrintPlayer.ViewCurBP = lpMsg->ViewCurBP;
	CharacterAttribute->PrintPlayer.ViewCurSD = lpMsg->ViewCurSD;
}

void GCLevelUpRecv(PMSG_LEVEL_UP_RECV* lpMsg)
{
	CharacterAttribute->PrintPlayer.ViewPoint = lpMsg->ViewPoint;
	CharacterAttribute->PrintPlayer.ViewMaxHP = lpMsg->ViewMaxHP;
	CharacterAttribute->PrintPlayer.ViewMaxMP = lpMsg->ViewMaxMP;
	CharacterAttribute->PrintPlayer.ViewMaxBP = lpMsg->ViewMaxBP;
	CharacterAttribute->PrintPlayer.ViewMaxSD = lpMsg->ViewMaxSD;
	CharacterAttribute->PrintPlayer.ViewExperience = lpMsg->ViewExperience;
	CharacterAttribute->PrintPlayer.ViewNextExperience = lpMsg->ViewNextExperience;
}

void GCLevelUpPointRecv(PMSG_LEVEL_UP_POINT_RECV* lpMsg)
{
	if (lpMsg->result >= 16 && lpMsg->result <= 20)
	{
		CharacterAttribute->PrintPlayer.ViewPoint = lpMsg->ViewPoint;
		CharacterAttribute->PrintPlayer.ViewMaxHP = lpMsg->ViewMaxHP;
		CharacterAttribute->PrintPlayer.ViewMaxMP = lpMsg->ViewMaxMP;
		CharacterAttribute->PrintPlayer.ViewMaxBP = lpMsg->ViewMaxBP;
		CharacterAttribute->PrintPlayer.ViewMaxSD = lpMsg->ViewMaxSD;
		CharacterAttribute->PrintPlayer.ViewStrength = lpMsg->ViewStrength;
		CharacterAttribute->PrintPlayer.ViewDexterity = lpMsg->ViewDexterity;
		CharacterAttribute->PrintPlayer.ViewVitality = lpMsg->ViewVitality;
		CharacterAttribute->PrintPlayer.ViewEnergy = lpMsg->ViewEnergy;
		CharacterAttribute->PrintPlayer.ViewLeadership = lpMsg->ViewLeadership;
	}
}

void GCMonsterDamageRecv(PMSG_MONSTER_DAMAGE_RECV* lpMsg)
{
	CharacterAttribute->PrintPlayer.ViewCurHP = lpMsg->ViewCurHP;
	CharacterAttribute->PrintPlayer.ViewCurSD = lpMsg->ViewCurSD;
	CharacterAttribute->PrintPlayer.ViewDamageHP = lpMsg->ViewDamageHP;
	CharacterAttribute->PrintPlayer.ViewDamageSD = lpMsg->ViewDamageSD;
}

void GCMasterInfoRecv(PMSG_MASTER_INFO_RECV* lpMsg)
{
	CharacterAttribute->PrintPlayer.ViewMaxHP = lpMsg->ViewMaxHP;
	CharacterAttribute->PrintPlayer.ViewMaxMP = lpMsg->ViewMaxMP;
	CharacterAttribute->PrintPlayer.ViewMaxBP = lpMsg->ViewMaxBP;
	CharacterAttribute->PrintPlayer.ViewMaxSD = lpMsg->ViewMaxSD;
}

void GCMasterLevelUpRecv(PMSG_MASTER_LEVEL_UP_RECV* lpMsg)
{
	CharacterAttribute->PrintPlayer.ViewMaxHP = lpMsg->ViewMaxHP;
	CharacterAttribute->PrintPlayer.ViewMaxMP = lpMsg->ViewMaxMP;
	CharacterAttribute->PrintPlayer.ViewMaxBP = lpMsg->ViewMaxBP;
	CharacterAttribute->PrintPlayer.ViewMaxSD = lpMsg->ViewMaxSD;
	CharacterAttribute->PrintPlayer.ViewMasterExperience = lpMsg->ViewMasterExperience;
	CharacterAttribute->PrintPlayer.ViewMasterNextExperience = lpMsg->ViewMasterNextExperience;
}

void DataSend(BYTE* lpMsg, DWORD size)
{
	BDataSend(lpMsg, size);
}

void GCRecvCoin(PMSG_COIN_RECV* lpMsg)
{
	CharacterAttribute->PrintPlayer.Coin1 = lpMsg->Coin1;
	CharacterAttribute->PrintPlayer.Coin2 = lpMsg->Coin2;
	CharacterAttribute->PrintPlayer.Coin3 = lpMsg->Coin3;
}