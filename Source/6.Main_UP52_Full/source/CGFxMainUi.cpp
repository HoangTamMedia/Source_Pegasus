#include "stdafx.h"

#ifdef LDK_ADD_SCALEFORM

#include <assert.h>
#include "CGFxMainUi.h"
#include "WSclient.h"
#include "wsclientinline.h"
#include "ZzzBMD.h"
#include "ZzzCharacter.h"
#include "DSPlaySound.h"
#include "ZzzInfomation.h"
#include "CSItemOption.h"
#ifdef YDG_ADD_DOPPELGANGER_UI
#include "GMDoppelGanger1.h"
#include "GMDoppelGanger2.h"
#include "GMDoppelGanger3.h"
#include "GMDoppelGanger4.h"
#endif
#include "CSChaosCastle.h"
#include "UIJewelHarmony.h"
#include "UIManager.h"
#include "NewUICustomMessageBox.h"
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
#include "MonkSystem.h"
#endif

#ifdef FOR_WORK
#include "Utilities\Log\DebugAngel.h"
#endif

#include "NewUISystem.h"

CGFxMainUi* CGFxMainUi::Make(GFxRegistType _UIType, mapGFXContainer *_GfxRepo, const char* _pfilename, UInt _loadConstants, UPInt _memoryArena, UInt _renderFlag, GFxMovieView::ScaleModeType _scaleType, FontConfig* _pconfig, const char* _languag)
{
	if(_GfxRepo == NULL)
		return NULL;

	CGFxMainUi* temp( new CGFxMainUi );

	if(!temp->InitGFx(_pfilename, _loadConstants, _memoryArena, _renderFlag, _scaleType, _pconfig, _languag))
	{
		delete temp;
		temp = NULL;

		return NULL;
	}

	_GfxRepo->insert(make_pair(_UIType, temp));

	return temp;
}

CGFxMainUi::CGFxMainUi()
{
	m_bVisible = FALSE;
	m_bLock = FALSE;
	m_bWireFrame = FALSE;
	m_dwMovieLastTime = 0;

	m_iHpPercent = 0;
	m_isIntoxication = false;
	m_iMpPercent = 0;
	m_iSDPercent = 0;
	m_iAGPercent = 0;
	m_iExpPercent = 0;
	m_iExpMin = 0;
	m_iExpMax = 0;
	m_iViewType = 0;

	m_bMasterLv = false;

	m_bStaVisible = false;
	m_iStaPercent = 0;

	for(int i=0; i<MAX_ITEM_SLOT; i++)
	{
		m_iItemType[i] = -1;
		m_iItemLevel[i] = 0;
		m_iItemCount[i] = 0;
	}

	m_iUseItemSlotNum = -1;
	m_iOverItemSlot = -1;

	for(int i=0; i<MAX_SKILL_HOT_KEY; ++i)
	{
		m_iHotKeySkillIndex[i] = -1;
		m_isHotKeySkillCantUse[i] = false;
		m_iHotKeySkillType[i] = -1;
	}

	m_isSkillSlotVisible = false;
	m_iSkillSlotCount = 0;
	m_iPetSlotCount = 0;
	for(int i=0; i<MAX_MAGIC; ++i)
	{
		m_iSkillSlotIndex[i] = -1;
		m_iSkillSlotType[i] = -1;
		m_isSkillSlotCantUse[i] = false;
	}
}

CGFxMainUi::~CGFxMainUi()
{
	m_pUIMovie = NULL;
	m_pUIMovieDef = NULL;

	m_pFSHandler->Release();
	m_pEIHandler->Release();
}

bool CGFxMainUi::OnCreateDevice(SInt bufw, SInt bufh, SInt left, SInt top, SInt w, SInt h, UInt flags)
{
	if(!m_pUIMovie)
		return FALSE;

	if(!m_pRenderer->SetDependentVideoMode())
		return FALSE;

	m_pUIMovie->SetViewport(bufw, bufh, left, top, w, h, flags);

	if(bufw == 800 && bufh == 600)
	{
		m_iViewType = 1;
		m_pUIMovie->Invoke("_root.scene.SetViewSize", "%d", 1);
	}
	else if(bufw == 1024 && bufh == 768)
	{
		m_iViewType = 2;
		m_pUIMovie->Invoke("_root.scene.SetViewSize", "%d", 2);
	}
	else if(bufw == 1280 && bufh == 1024)
	{
		m_iViewType = 3;
		m_pUIMovie->Invoke("_root.scene.SetViewSize", "%d", 3);
	}

	return TRUE;
}

bool CGFxMainUi::OnResetDevice()
{

	return TRUE;
}

bool CGFxMainUi::OnLostDevice()
{
	if(m_pRenderer->ResetVideoMode())
		return FALSE;

	return TRUE;
}

bool CGFxMainUi::OnDestroyDevice()
{
	return TRUE;
}

bool CGFxMainUi::InitGFx(const char* _pfilename, UInt _loadConstants, UPInt _memoryArena, UInt _renderFlag, GFxMovieView::ScaleModeType _scaleType, FontConfig* _pconfig, const char* _languag)
{

  	if (_pconfig)
	{
 		_pconfig->Apply(&m_gfxLoader);
	}

	m_gfxLoader.SetLog(GPtr<GFxLog>(*new GFxPlayerLog()));

	m_pFSHandler = new CMainUIFSCHandler;
	m_gfxLoader.SetFSCommandHandler(GPtr<GFxFSCommandHandler>(m_pFSHandler));

	m_pEIHandler = new CMainUIEIHandler;
	m_pEIHandler->SetMainUi(this);
	m_gfxLoader.SetExternalInterface(GPtr<GFxExternalInterface>(m_pEIHandler));

	m_pRenderer = *GRendererOGL::CreateRenderer();
	m_pRenderer->SetDependentVideoMode();

	m_pRenderConfig = *new GFxRenderConfig(m_pRenderer, _renderFlag);
	if(!m_pRenderer || !m_pRenderConfig)
		return FALSE;
	m_gfxLoader.SetRenderConfig(m_pRenderConfig);

	m_pRenderStats = *new GFxRenderStats();
	m_gfxLoader.SetRenderStats(m_pRenderStats);

	GPtr<GFxFileOpener> pfileOpener = *new GFxFileOpener;
	m_gfxLoader.SetFileOpener(pfileOpener);

	if (!m_gfxLoader.GetMovieInfo(_pfilename, &m_gfxMovieInfo, 0, _loadConstants))
	{

		return FALSE;
	}

	m_pUIMovieDef = *(m_gfxLoader.CreateMovie(_pfilename, _loadConstants, _memoryArena));
	if(!m_pUIMovieDef)
		return FALSE;

	m_pUIMovie = *m_pUIMovieDef->CreateInstance(GFxMovieDef::MemoryParams(), false);
	if(!m_pUIMovie)
		return FALSE;

	if (m_pUIMovie->GetMeshCacheManager())
		m_pUIMovie->GetMeshCacheManager()->ClearCache();

	m_pUIMovie->SetViewScaleMode(_scaleType);

	m_pUIMovie->SetUserData(this);

	if(_languag == NULL) _languag = "Default";
 	m_pUIMovie->SetVariable("_global.gfxLanguage", GFxValue(_languag));

	m_pUIMovie->Advance(0.0f, 0);
	m_pUIMovie->SetBackgroundAlpha(0.0f);

	m_dwMovieLastTime = timeGetTime();

	m_pUIMovie->Invoke("_root.scene.SetClearSkillSlot", "");

	m_pUIMovie->Invoke("_root.scene.SetStaminaVisible", "%b", m_bStaVisible);

	return TRUE;
}

bool CGFxMainUi::Init()
{
	m_iHpPercent = 0;
	m_isIntoxication = false;
	m_iMpPercent = 0;
	m_iSDPercent = 0;
	m_iAGPercent = 0;
	m_iExpPercent = 0;
	m_iExpMin = 0;
	m_iExpMax = 0;
	m_bMasterLv = false;

	m_bStaVisible = false;
	m_iStaPercent = 0;

	for(int i=0; i<MAX_ITEM_SLOT; i++)
	{
		m_iItemType[i] = -1;
		m_iItemLevel[i] = 0;
		m_iItemCount[i] = 0;
	}

	m_iUseItemSlotNum = -1;

	for(int i=0; i<MAX_SKILL_HOT_KEY; ++i)
	{
		m_iHotKeySkillIndex[i] = -1;
		m_isHotKeySkillCantUse[i] = false;
		m_iHotKeySkillType[i] = -1;
	}

	m_isSkillSlotVisible = false;
	m_iSkillSlotCount = 0;
	m_iPetSlotCount = 0;
	for(int i=0; i<MAX_MAGIC; ++i)
	{
		m_iSkillSlotIndex[i] = -1;
		m_iSkillSlotType[i] = -1;
		m_isSkillSlotCantUse[i] = false;
	}

	SetItemClearHotKey();
	SetSkillClearHotKey();

	return TRUE;
}

void CGFxMainUi::UpdateMenuBtns()
{
#ifdef ASG_ADD_UI_QUEST_PROGRESS_ETC

	if (!g_QuestMng.IsQuestIndexByEtcListEmpty())
	{

		m_pUIMovie->Invoke("_root.scene.SetMainBtnBlink", "%d %b %d", 1, true, 500);
	}
#endif
}

void CGFxMainUi::UpdateGaugeHpMp()
{
	WORD wLifeMax, wLife, wManaMax, wMana;

	if(IsMasterLevel( Hero->Class ) == true )
	{
		wLifeMax = Master_Level_Data.wMaxLife;
		wLife = min(max(0, CharacterAttribute->Life), wLifeMax);
		wManaMax = Master_Level_Data.wMaxMana;
		wMana = min(max(0, CharacterAttribute->Mana), wManaMax);
	}
	else
	{
		wLifeMax = CharacterAttribute->LifeMax;
		wLife = min(max(0, CharacterAttribute->Life), wLifeMax);
		wManaMax = CharacterAttribute->ManaMax;
		wMana = min(max(0, CharacterAttribute->Mana), wManaMax);
	}

	if(wLifeMax > 0)
	{
		if(wLife > 0 && (wLife / (float)wLifeMax) < 0.2f)
		{
			PlayBuffer(SOUND_HEART);
		}
	}

	int iLife = 0;
	int iMana = 0;
	if(wLifeMax > 0)
	{
		iLife = int( (float)wLife / (float)wLifeMax * 100.0f );

		if(iLife <= 0)
			iLife = 0;
	}
	if(wManaMax > 0)
	{
		iMana = int((float)wMana / (float)wManaMax * 100.0f);

		if(iMana <= 0)
			iMana = 0;
	}

	bool _intoxication = g_isCharacterBuff((&Hero->Object), eDeBuff_Poison);
	if(m_isIntoxication != _intoxication)
	{
		m_isIntoxication = _intoxication;
		m_pUIMovie->Invoke("_root.scene.SetChangeIntoxication", "%b", m_isIntoxication);
	}

	if(m_iHpPercent != iLife)
	{
		m_iHpPercent = iLife;
		m_pUIMovie->Invoke("_root.scene.SetChangeHp", "%d %d %d", m_iHpPercent, (int)wLife, (int)wLifeMax);
	}

	if(m_iMpPercent != iMana)
	{
		m_iMpPercent = iMana;
		m_pUIMovie->Invoke("_root.scene.SetChangeMp", "%d %d %d", m_iMpPercent, (int)wMana, (int)wManaMax);
	}
}

void CGFxMainUi::UpdateGaugeSd()
{
	WORD wMaxShield,wShield;

	if(IsMasterLevel(Hero->Class) == true)
	{
		wMaxShield = max (1, Master_Level_Data.wMaxShield);
		wShield = min (wMaxShield, CharacterAttribute->Shield);
	}
	else
	{
		wMaxShield = max (1, CharacterAttribute->ShieldMax);
		wShield = min (wMaxShield, CharacterAttribute->Shield);
	}

	int iShield;
	if(wMaxShield > 0)
	{
		iShield = int( (float)wShield / (float)wMaxShield * 100.0f);
	}

	if(m_iSDPercent != iShield)
	{
		m_iSDPercent = iShield;
		m_pUIMovie->Invoke("_root.scene.SetChangeSd", "%d %d %d", m_iSDPercent, (int)wShield, (int)wMaxShield);
	}
}

void CGFxMainUi::UpdateGaugeAg()
{
	WORD dwMaxSkillMana,dwSkillMana;

	if(IsMasterLevel(Hero->Class) == true)
	{

		dwMaxSkillMana = max(1, Master_Level_Data.wMaxBP);
		dwSkillMana = min(dwMaxSkillMana, CharacterAttribute->SkillMana);
	}
	else
	{
		dwMaxSkillMana = max(1, CharacterAttribute->SkillManaMax);
		dwSkillMana = min(dwMaxSkillMana, CharacterAttribute->SkillMana);
	}

	int iSkillMana;
	if(dwMaxSkillMana > 0)
	{
		iSkillMana = (int)( (float)dwSkillMana / (float)dwMaxSkillMana * 100.0f);

		if(iSkillMana <= 0)
			iSkillMana = 0;
	}

	if(m_iAGPercent != iSkillMana)
	{
		m_iAGPercent = iSkillMana;
		m_pUIMovie->Invoke("_root.scene.SetChangeAg", "%d %d %d", m_iAGPercent, (int)dwSkillMana, (int)dwMaxSkillMana);
	}
}

void CGFxMainUi::UpdateGaugeExp()
{
	__int64 wLevel;
	__int64 dwNexExperience;
	__int64 dwExperience;

	bool _bMasterLv = false;

	if(IsMasterLevel(CharacterAttribute->Class) == true)
	{
		_bMasterLv = true;
		wLevel = (__int64)Master_Level_Data.nMLevel;
		dwNexExperience = (__int64)Master_Level_Data.lNext_MasterLevel_Experince;
		dwExperience = (__int64)Master_Level_Data.lMasterLevel_Experince;
	}
	else
	{
		_bMasterLv = false;
		wLevel = CharacterAttribute->Level;
		dwNexExperience = CharacterAttribute->NextExperince;
		dwExperience = CharacterAttribute->Experience;
	}

	if(dwNexExperience == 0 && dwExperience == 0) return;

	if(IsMasterLevel(CharacterAttribute->Class) == true)
	{
		__int64 iTotalLevel = wLevel + 400;
		__int64 iTOverLevel = iTotalLevel - 255;
		__int64 iBaseExperience = 0;

		__int64 iData_Master =
			(
			(
			(__int64)9 + (__int64)iTotalLevel
			)
			* (__int64)iTotalLevel
			* (__int64)iTotalLevel
			* (__int64)10
			)
			+
			(
			(
			(__int64)9 + (__int64)iTOverLevel
			)
			* (__int64)iTOverLevel
			* (__int64)iTOverLevel
			* (__int64)1000
			);
		iBaseExperience = (iData_Master - (__int64)3892250000) / (__int64)2;

		double fNeedExp = (double)dwNexExperience - (double)iBaseExperience;

		double fExp = (double)dwExperience - (double)iBaseExperience;

		if(dwExperience < iBaseExperience)
		{
			fExp = 0.f;
		}

		if(m_bMasterLv != _bMasterLv)
		{
			m_bMasterLv = _bMasterLv;
			m_pUIMovie->Invoke("_root.scene.SetChangeMasterExp", "%b", m_bMasterLv);
		}

		int _iExpPercent = 0;
		if(fNeedExp > 0)
		{
			_iExpPercent = int( (float)fExp / (float)fNeedExp * 100.0f );

			if(_iExpPercent < 0)
				_iExpPercent = 0;
		}

		if(m_iExpPercent != _iExpPercent || m_iExpMin != fExp || m_iExpMax != fNeedExp)
		{
			m_iExpPercent = _iExpPercent;
			m_iExpMin = fExp;
			m_iExpMax = fNeedExp;

			m_pUIMovie->Invoke("_root.scene.SetChangeExp", "%d %d %d", _iExpPercent, (int)fExp, (int)fNeedExp);
		}
	}
	else
	{
		WORD wPriorLevel = wLevel - 1;
		DWORD dwPriorExperience = 0;

		if(wPriorLevel > 0)
		{
			dwPriorExperience = (9 + wPriorLevel) * wPriorLevel * wPriorLevel * 10;

			if(wPriorLevel > 255)
			{
				int iLevelOverN = wPriorLevel - 255;
				dwPriorExperience += (9 + iLevelOverN) * iLevelOverN * iLevelOverN * 1000;
			}
		}

		double fNeedExp = (double)dwNexExperience - (double)dwPriorExperience;

		double fExp = (double)dwExperience - (double)dwPriorExperience;

		if(fNeedExp < 0 || fExp < 0) return;

		if(dwExperience < dwPriorExperience)
		{
			fExp = 0.f;
		}

		int _iExpPercent;
		if(fNeedExp > 0)
		{
			_iExpPercent = int( (float)fExp / (float)fNeedExp * 100.0f );

			if(_iExpPercent < 0)
				_iExpPercent = 0;
		}

		if(m_iExpPercent != _iExpPercent || m_iExpMin != fExp || m_iExpMax != fNeedExp)
		{
			m_iExpPercent = _iExpPercent;
			m_iExpMin = fExp;
			m_iExpMax = fNeedExp;

			m_pUIMovie->Invoke("_root.scene.SetChangeExp", "%d %d %d", _iExpPercent, (int)fExp, (int)fNeedExp);
		}
	}
}

void CGFxMainUi::UpdateGaugeStamina()
{

#ifdef GFX_UI_TEST_CODE
	m_iStaPercent = m_iStaPercent > 100 ? 0 : m_iStaPercent+1;
#endif

}

void CGFxMainUi::UpdateItemSlot()
{
	int iIndex = -1;
	if(m_iUseItemSlotNum != -1)
	{
		iIndex = GetHotKeyItemIndex(m_iUseItemSlotNum);
		m_iUseItemSlotNum = -1;
	}

	if(iIndex != -1)
	{
		ITEM* pItem = NULL;
		pItem = g_pMyInventory->FindItem(iIndex);
#ifdef PSW_SECRET_ITEM
		if( ( pItem->Type>=ITEM_POTION+78 && pItem->Type<=ITEM_POTION+82 ) )
		{
			list<eBuffState> secretPotionbufflist;
			secretPotionbufflist.push_back( eBuff_SecretPotion1 );
			secretPotionbufflist.push_back( eBuff_SecretPotion2 );
			secretPotionbufflist.push_back( eBuff_SecretPotion3 );
			secretPotionbufflist.push_back( eBuff_SecretPotion4 );
			secretPotionbufflist.push_back( eBuff_SecretPotion5 );

			if( g_isCharacterBufflist( (&Hero->Object), secretPotionbufflist ) != eBuffNone ) {
				SEASON3B::CreateOkMessageBox(GlobalText[2530], RGBA(255, 30, 0, 255) );
			}
			else {
				SendRequestUse(iIndex, 0);
			}
		}
		else
#endif
		{
			SendRequestUse(iIndex, 0);
		}
	}

	for(int i=0; i<MAX_ITEM_SLOT; i++)
	{
		if(m_iItemType[i] == -1) continue;

		int temp = GetHotKeyItemIndex(i, true);
		if(temp != m_iItemCount[i])
		{
			m_iItemCount[i] = temp;
			m_pUIMovie->Invoke("_root.scene.SetChangeItemCount", "%d %d", i, temp);
		}
	}
}

void CGFxMainUi::UpdateSkillSlot()
{
	if(Hero != NULL && m_iHotKeySkillIndex[0] == -1)
	{
		SetSkillHotKey(0, Hero->CurrentSkill);
	}
	if(Hero != NULL)
	{

		SetSkillSlot();
	}

	for(int i=0; i<MAX_SKILL_HOT_KEY; i++)
	{

		if(m_iHotKeySkillIndex[i] != -1 && m_iHotKeySkillType[i] != CharacterAttribute->Skill[m_iHotKeySkillIndex[i]])
		{
			SetSkillHotKey(i, m_iHotKeySkillIndex[i], true);
		}

		bool bCantSkill = GetSkillDisable(i, m_iHotKeySkillIndex);
		if(bCantSkill != m_isHotKeySkillCantUse[i])
		{
			m_isHotKeySkillCantUse[i] = bCantSkill;
			m_pUIMovie->Invoke("_root.scene.SetSkillSlotDisable", "%d %b", i, bCantSkill);
		}
	}

	if(m_isSkillSlotVisible)
	{
		bool _skillChange = false;

		for(int i=0; i<(m_iSkillSlotCount+m_iPetSlotCount); i++)
		{

			if(m_iSkillSlotIndex[i] != -1 && m_iSkillSlotType[i] != CharacterAttribute->Skill[m_iSkillSlotIndex[i]])
			{
				_skillChange = true;
			}

			bool bCantSkill = GetSkillDisable(i, m_iSkillSlotIndex);
			if(bCantSkill != m_isSkillSlotCantUse[i])
			{
				m_isSkillSlotCantUse[i] = bCantSkill;
				m_pUIMovie->Invoke("_root.scene.SetTooltipSkillDisable", "%d %b", i, bCantSkill);
			}
		}

		if(_skillChange)
		{
			m_iSkillSlotCount = 0;
			m_iPetSlotCount = 0;
			SetSkillSlot();
		}
	}

	if(Hero->m_pPet == NULL)
	{
		if(Hero->CurrentSkill >= AT_PET_COMMAND_DEFAULT && Hero->CurrentSkill < AT_PET_COMMAND_END)
		{
			Hero->CurrentSkill = 0;
		}
	}

}

bool CGFxMainUi::Update()
{
	if(m_bVisible == FALSE)
		return TRUE;

	if(!m_pUIMovie)
		return FALSE;

	UpdateMenuBtns();

	UpdateGaugeHpMp();

	UpdateGaugeSd();

	UpdateGaugeAg();

	UpdateGaugeExp();

	UpdateGaugeStamina();

	UpdateItemSlot();

	UpdateSkillSlot();

	DWORD mtime = timeGetTime();
	float deltaTime = ((float)(mtime - m_dwMovieLastTime)) / 1000.0f;
	m_dwMovieLastTime = mtime;
	m_pUIMovie->Advance(deltaTime, 0);

	return TRUE;
}

void CGFxMainUi::RenderObjectScreen(int Type,int ItemLevel,int Option1,int ExtOption,vec3_t Target,int Select,bool PickUp)
{
	OBJECT ObjectSelect;

	int Level = (ItemLevel>>3)&15;
	vec3_t Direction,Position;

	VectorSubtract(Target,MousePosition,Direction);
	if(PickUp)
		VectorMA(MousePosition,0.07f,Direction,Position);
	else
		VectorMA(MousePosition,0.1f,Direction,Position);

	if(Type == MODEL_SWORD+0)
	{
		Position[0] -= 0.02f;
		Position[1] += 0.03f;
		Vector(180.f,270.f,15.f,ObjectSelect.Angle);
	}

	else if(Type==MODEL_BOW+7 || Type==MODEL_BOW+15 )
	{
		Vector(0.f,270.f,15.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_SPEAR+0)
	{
		Position[1] += 0.05f;
		Vector(0.f,90.f,20.f,ObjectSelect.Angle);
	}
	else if( Type==MODEL_BOW+17)
	{
		Vector(0.f,90.f,15.f,ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELM+31)
	{
		Position[1] -= 0.06f;
		Position[0] += 0.03f;
		Vector(-90.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELM+30)
	{
		Position[1] += 0.07f;
		Position[0] -= 0.03f;
		Vector(-90.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if( Type == MODEL_ARMOR+30)
	{
		Position[1] += 0.1f;
		Vector(-90.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if( Type == MODEL_ARMOR+29)
	{
		Position[1] += 0.07f;
		Vector(-90.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if( Type == MODEL_BOW+21)
	{
		Position[1] += 0.12f;
		Vector(180.f,-90.f,15.f,ObjectSelect.Angle);
	}
	else if( Type == MODEL_STAFF+12)
	{
		Position[1] -= 0.1f;
		Position[0] += 0.025f;
		Vector(180.f,0.f,8.f,ObjectSelect.Angle);
	}
	else if (Type >= MODEL_STAFF+21 && Type <= MODEL_STAFF+29)
	{
		Vector(0.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if( Type == MODEL_MACE+14)
	{
		Position[1] += 0.1f;
		Position[0] -= 0.01;
		Vector(180.f,90.f,13.f,ObjectSelect.Angle);
	}

	else if(Type == MODEL_ARMOR+34)
	{
		Position[1] += 0.03f;
		Vector(-90.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELM+35)
	{
		Position[0] -= 0.02f;
		Position[1] += 0.05f;
		Vector(-90.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_ARMOR+35)
	{
		Position[1] += 0.05f;
		Vector(-90.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_ARMOR+36)
	{
		Position[1] -= 0.05f;
		Vector(-90.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_ARMOR+37)
	{
		Position[1] -= 0.05f;
		Vector(-90.f,0.f,0.f,ObjectSelect.Angle);
	}

	else if (MODEL_HELM+39 <= Type && MODEL_HELM+44 >= Type)
	{
		Position[1] -= 0.05f;
		Vector(-90.f,25.f,0.f,ObjectSelect.Angle);
	}

	else if(MODEL_ARMOR+38 <= Type && MODEL_ARMOR+44 >= Type)
	{
		Position[1] -= 0.08f;
		Vector(-90.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_SWORD+24)
	{
		Position[0] -= 0.02f;
		Position[1] += 0.03f;
		Vector(180.f,90.f,15.f,ObjectSelect.Angle);
	}
	else if( Type == MODEL_MACE+15)
	{
		Position[1] += 0.05f;
		Vector(180.f,90.f,13.f,ObjectSelect.Angle);
	}
#ifdef ADD_SOCKET_ITEM
	else if(Type == MODEL_BOW+22 || Type == MODEL_BOW+23)
	{
		Position[0] -= 0.10f;
		Position[1] += 0.08f;
		Vector(180.f,-90.f,15.f,ObjectSelect.Angle);
	}
#else
	else if( Type == MODEL_BOW+22)
	{
		Position[1] += 0.12f;
		Vector(180.f,90.f,15.f,ObjectSelect.Angle);
	}
#endif
	else if(Type == MODEL_STAFF+13)
	{
		Position[0] += 0.02f;
		Position[1] += 0.02f;
		Vector(180.f,90.f,8.f,ObjectSelect.Angle);
	}
	else if(Type==MODEL_BOW+20)
	{
		Vector(180.f,-90.f,15.f,ObjectSelect.Angle);
	}
	else if(Type>=MODEL_BOW+8 && Type<MODEL_BOW+MAX_ITEM_INDEX)
	{
		Vector(90.f,180.f,20.f,ObjectSelect.Angle);
	}
	else if ( Type==MODEL_SPEAR+10 )
	{
		Vector(180.f,270.f,20.f,ObjectSelect.Angle);
	}
	else if(Type >= MODEL_SWORD && Type < MODEL_STAFF+MAX_ITEM_INDEX)
	{
		switch (Type)
		{
		case MODEL_STAFF+14:							Position[1] += 0.04f;	break;
		case MODEL_STAFF+17:	Position[0] += 0.02f;	Position[1] += 0.03f;	break;
		case MODEL_STAFF+18:	Position[0] += 0.02f;							break;
		case MODEL_STAFF+19:	Position[0] -= 0.02f;	Position[1] -= 0.02f;	break;
		case MODEL_STAFF+20:	Position[0] += 0.01f;	Position[1] -= 0.01f;	break;
		}

		if(!ItemAttribute[Type-MODEL_ITEM].TwoHand)
		{
			Vector(180.f,270.f,15.f,ObjectSelect.Angle);
		}
		else
		{
			Vector(180.f,270.f,25.f,ObjectSelect.Angle);
		}

	}
	else if(Type>=MODEL_SHIELD && Type<MODEL_SHIELD+MAX_ITEM_INDEX)
	{
		Vector(270.f,270.f,0.f,ObjectSelect.Angle);
	}
	else if(Type==MODEL_HELPER+3)
	{
		Vector(-90.f,-90.f,0.f,ObjectSelect.Angle);
	}
	else if ( Type==MODEL_HELPER+4 )
	{
		Vector(-90.f,-90.f,0.f,ObjectSelect.Angle);
	}
	else if ( Type==MODEL_HELPER+5 )
	{
		Vector(-90.f,-35.f,0.f,ObjectSelect.Angle);
	}
	else if ( Type==MODEL_HELPER+31 )
	{
		Vector(-90.f,-90.f,0.f,ObjectSelect.Angle);
	}
	else if ( Type==MODEL_HELPER+30 )
	{
		Vector ( -90.f, 0.f, 0.f, ObjectSelect.Angle );
	}
	else if ( Type==MODEL_EVENT+16 )
	{
		Vector ( -90.f, 0.f, 0.f, ObjectSelect.Angle );
	}
	else if ( Type==MODEL_HELPER+16 || Type == MODEL_HELPER+17 )
	{
		Vector(270.f,-10.f,0.f,ObjectSelect.Angle);
	}
	else if ( Type==MODEL_HELPER+18 )
	{
		Vector(290.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if ( Type==MODEL_EVENT+11 )
	{
#ifdef FRIEND_EVENT
		if ( Type==MODEL_EVENT+11 && Level==2 )
		{
			Vector(270.f,0.f,0.f,ObjectSelect.Angle);
		}
		else
#endif
		{
			Vector(-90.f, -20.f, -20.f, ObjectSelect.Angle);
		}
	}
	else if ( Type==MODEL_EVENT+12)
	{
		Vector(250.f, 140.f, 0.f, ObjectSelect.Angle);
	}
	else if (Type==MODEL_EVENT+14)
	{
		Vector(255.f, 160.f, 0.f, ObjectSelect.Angle);
	}
	else if (Type==MODEL_EVENT+15)
	{
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if ( Type>=MODEL_HELPER+21 && Type<=MODEL_HELPER+24 )
	{
		Vector(270.f, 160.f, 20.f, ObjectSelect.Angle);
	}
	else if ( Type==MODEL_HELPER+29 )
	{
		Vector(290.f,0.f,0.f,ObjectSelect.Angle);
	}

	else if(Type == MODEL_HELPER+32)
	{
		Position[0] += 0.01f;
		Position[1] -= 0.03f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+33)
	{
		Position[1] += 0.02f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+34)
	{
		Position[0] += 0.01f;
		Position[1] += 0.02f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+35)
	{
		Position[0] += 0.01f;
		Position[1] += 0.02f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+36)
	{
		Position[0] += 0.01f;
		Position[1] += 0.05f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+37)
	{
		Position[0] += 0.01f;
		Position[1] += 0.04f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#ifdef CSK_PCROOM_ITEM
	else if(Type >= MODEL_POTION+55 && Type <= MODEL_POTION+57)
	{
		Position[1] += 0.02f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
	else if(Type == MODEL_HELPER+49)
	{
		Position[1] -= 0.04f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+50)
	{
		Position[1] -= 0.03f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+51)
	{
		Position[1] -= 0.02f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_POTION+64)
	{
		Position[1] += 0.02f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+52)
	{
		Position[1] += 0.045f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+53)
	{
		Position[1] += 0.04f;
		Vector(270.f, 120.f, 0.f, ObjectSelect.Angle);
	}

	else if(Type == MODEL_WING+37)
	{
		Position[1] += 0.05f;
		Vector(270.f,-10.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_WING+38)
	{
		Position[1] += 0.05f;
		Vector(270.f,-10.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_WING+39)
	{
		Position[1] += 0.08f;
		Vector(270.f,-10.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_WING+40)
	{
		Position[1] += 0.05f;
		Vector(270.f,-10.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_WING+42)
	{
		Position[1] += 0.05f;
		Vector(270.f,0.f,2.f,ObjectSelect.Angle);
	}
#ifdef CSK_FREE_TICKET

	else if(Type == MODEL_HELPER+46)
	{
		Position[1] -= 0.04f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+47)
	{
		Position[1] -= 0.04f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+48)
	{
		Position[1] -= 0.04f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef CSK_CHAOS_CARD

	else if(Type == MODEL_POTION+54)
	{
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef CSK_RARE_ITEM

	else if(Type == MODEL_POTION+58)
	{
		Position[1] += 0.07f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_POTION+59 || Type == MODEL_POTION+60)
	{
		Position[1] += 0.06f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_POTION+61 || Type == MODEL_POTION+62)
	{
		Position[1] += 0.06f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef CSK_LUCKY_CHARM
	else if( Type == MODEL_POTION+53 )
	{
		Position[1] += 0.042f;
		Vector(180.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef CSK_LUCKY_SEAL
#ifdef PBG_FIX_ITEMANGLE
	else if( Type == MODEL_HELPER+43 )
	{
		Position[1] -= 0.027f;
		Position[0] += 0.005f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELPER+44 )
	{
		Position[1] -= 0.03f;
		Position[0] += 0.005f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELPER+45 )
	{
		Position[1] -= 0.02f;
		Position[0] += 0.005f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#else
	else if( Type == MODEL_HELPER+43 )
	{
		Position[1] += 0.082f;
		Vector(90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELPER+44 )
	{
		Position[1] += 0.08f;
		Vector(90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELPER+45 )
	{
		Position[1] += 0.07f;
		Vector(90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#endif
#ifdef PSW_ELITE_ITEM
	else if( Type >= MODEL_POTION+70 && Type <= MODEL_POTION+71 )
	{
		Position[0] += 0.01f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PSW_SCROLL_ITEM
	else if( Type >= MODEL_POTION+72 && Type <= MODEL_POTION+77 )
	{
		Position[1] += 0.08f;
		Vector(0.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PSW_SEAL_ITEM
	else if( Type == MODEL_HELPER+59 )
	{
		Position[0] += 0.01f;
		Position[1] += 0.02f;
		Vector(90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PSW_FRUIT_ITEM
	else if( Type >= MODEL_HELPER+54 && Type <= MODEL_HELPER+58 )
	{
		Position[1] -= 0.02f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PSW_SECRET_ITEM
	else if( Type >= MODEL_POTION+78 && Type <= MODEL_POTION+82 )
	{
		Position[1] += 0.01f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PSW_INDULGENCE_ITEM
	else if( Type == MODEL_HELPER+60 )
	{
		Position[1] -= 0.06f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PSW_CURSEDTEMPLE_FREE_TICKET
	else if( Type == MODEL_HELPER+61 )
	{
		Position[1] -= 0.04f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PSW_RARE_ITEM
	else if(Type == MODEL_POTION+83)
	{
		Position[1] += 0.06f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PSW_CHARACTER_CARD
	else if(Type == MODEL_POTION+91)
	{
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PSW_NEW_CHAOS_CARD
	else if(Type == MODEL_POTION+92)
	{
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_POTION+93)
	{
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_POTION+95)
	{
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PSW_NEW_ELITE_ITEM
	else if( Type == MODEL_POTION+94 )
	{
		Position[0] += 0.01f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef CSK_EVENT_CHERRYBLOSSOM
	else if( Type >= MODEL_POTION+84 && Type <= MODEL_POTION+90 )
	{
		if( Type == MODEL_POTION+84 )
		{
			Position[1] += 0.01f;
			Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
		}
		else if( Type == MODEL_POTION+85 )
		{
			Position[1] -= 0.01f;
			Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
		}
		else if( Type == MODEL_POTION+86 )
		{
			Position[1] += 0.01f;
			Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
		}
		else if( Type == MODEL_POTION+87 )
		{
			Position[1] += 0.01f;
			Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
		}
		else if( Type == MODEL_POTION+88 )
		{
			Position[1] += 0.015f;
			Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
		}
		else if( Type == MODEL_POTION+89 )
		{
			Position[1] += 0.015f;
			Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
		}
		else if( Type == MODEL_POTION+90 )
		{
			Position[1] += 0.015f;
			Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
		}
	}
#endif
#ifdef PSW_ADD_PC4_SEALITEM
	else if(Type == MODEL_HELPER+62)
	{
#ifdef PBG_FIX_ITEMANGLE
		Position[0] += 0.01f;
		Position[1] -= 0.03f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
#else
		Position[0] += 0.01f;
		Position[1] += 0.08f;
		Vector(90.f, 0.f, 0.f, ObjectSelect.Angle);
#endif
	}
#endif
#ifdef PSW_ADD_PC4_SEALITEM
	else if(Type == MODEL_HELPER+63)
	{
		Position[0] += 0.01f;
		Position[1] += 0.082f;
		Vector(90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PSW_ADD_PC4_SCROLLITEM
	else if(Type >= MODEL_POTION+97 && Type <= MODEL_POTION+98)
	{
		Position[1] += 0.09f;
		Vector(0.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PSW_ADD_PC4_CHAOSCHARMITEM
	else if( Type == MODEL_POTION+96 )
	{
#ifdef PBG_FIX_ITEMANGLE
		Position[1] -= 0.013f;
		Position[0] += 0.003f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
#else
		Position[1] += 0.13f;
		Vector(90.f, 0.f, 0.f, ObjectSelect.Angle);
#endif
	}
#endif
#ifdef LDK_ADD_PC4_GUARDIAN
	else if( MODEL_HELPER+64 <= Type && Type <= MODEL_HELPER+65 )
	{
		switch(Type)
		{
		case MODEL_HELPER+64:
			Position[1] -= 0.05f;
			break;
		case MODEL_HELPER+65:
			Position[1] -= 0.02f;
			break;
		}
		Vector(270.f, -10.f, 0.f, ObjectSelect.Angle);
	}
#endif
	else if (Type == MODEL_POTION+65)
	{
		Position[1] += 0.05f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION+66)
	{
		Position[1] += 0.11f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION+67)
	{
		Position[1] += 0.11f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}

	else if(Type == MODEL_HELPER+39)
	{
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#ifdef CSK_LUCKY_SEAL
	else if( Type == MODEL_HELPER+43 )
	{

		Position[1] -= 0.03f;
		Vector(90.f, 0.f, 180.f, ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELPER+44 )
	{
		Position[1] += 0.08f;
		Vector(90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELPER+45 )
	{
		Position[1] += 0.07f;
		Vector(90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif

	else if(Type == MODEL_HELPER+40)
	{
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+41)
	{
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_HELPER+51)
	{
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}

	else if(Type == MODEL_HELPER+42)
	{
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type==MODEL_HELPER+38 )
	{
		Position[0] += 0.00f;
		Position[1] += 0.02f;
		Vector( -48-150.f, 0.f, 0.f, ObjectSelect.Angle);
	}
	else if(Type == MODEL_POTION+41)
	{
		Position[1] += 0.02f;
		Vector(270.f, 90.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type==MODEL_POTION+42 )
	{
		Position[1] += 0.02f;
		Vector(270.f, -10.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type==MODEL_POTION+43 || Type==MODEL_POTION+44 )
	{
		Position[0] -= 0.04f;
		Position[1] += 0.02f;
		Position[2] += 0.02f;
		Vector( 270.f, -10.f, -45.f, ObjectSelect.Angle );
	}
	else if(Type>=MODEL_HELPER+12 && Type<MODEL_HELPER+MAX_ITEM_INDEX && Type!=MODEL_HELPER+14  && Type!=MODEL_HELPER+15)
	{
		Vector(270.f+90.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type==MODEL_POTION+12)
	{
		switch(Level)
		{
		case 0:Vector(180.f,0.f,0.f,ObjectSelect.Angle);break;
		case 1:Vector(270.f,90.f,0.f,ObjectSelect.Angle);break;
		case 2:Vector(90.f,0.f,0.f,ObjectSelect.Angle);break;
		}
	}
	else if(Type==MODEL_EVENT+5)
	{
		Vector(270.f,180.f,0.f,ObjectSelect.Angle);
	}
	else if(Type==MODEL_EVENT+6)
	{
		Vector(270.f,90.f,0.f,ObjectSelect.Angle);
	}
	else if(Type==MODEL_EVENT+7)
	{
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type==MODEL_POTION+20)
	{
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if ( Type==MODEL_POTION+27 )
	{
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if ( Type == MODEL_POTION+63 )
	{
		Position[1] += 0.08f;
		Vector(-50.f,-60.f,0.f,ObjectSelect.Angle);
	}
	else if ( Type == MODEL_POTION+52)
	{

		Vector(270.f,-25.f,0.f,ObjectSelect.Angle);
	}
#ifdef _PVP_MURDERER_HERO_ITEM
	else if ( Type==MODEL_POTION+30 )
	{
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
#endif
	else if(Type >= MODEL_ETC+19 && Type <= MODEL_ETC+27)
	{
		Position[0] += 0.03f;
		Position[1] += 0.03f;
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_WING+7)
	{
		Position[0] += 0.005f;
		Position[1] -= 0.015f;
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_ARMOR+10)
	{
		Position[1] -= 0.1f;
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_PANTS+10)
	{
		Position[1] -= 0.08f;
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_ARMOR+11)
	{
		Position[1] -= 0.1f;
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type == MODEL_PANTS+11)
	{
		Position[1] -= 0.08f;
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
#ifdef CSK_ADD_SKILL_BLOWOFDESTRUCTION
	else if(Type == MODEL_WING+44)
	{
		Position[0] += 0.005f;
		Position[1] -= 0.015f;
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
#endif

#ifdef PJH_SEASON4_SPRITE_NEW_SKILL_RECOVER
	else if(Type == MODEL_WING+46
#ifdef PJH_SEASON4_SPRITE_NEW_SKILL_MULTI_SHOT
		|| Type==MODEL_WING+45
#endif
		)
	{
		Position[0] += 0.005f;
		Position[1] -= 0.015f;
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
#endif
	else
	{
		Vector(270.f,-10.f,0.f,ObjectSelect.Angle);
	}
#ifdef ADD_SEED_SPHERE_ITEM

	if(Type >= MODEL_WING+60 && Type <= MODEL_WING+65)
	{
		Vector(10.f,-10.f,10.f,ObjectSelect.Angle);
	}
	else if(Type >= MODEL_WING+70 && Type <= MODEL_WING+74)
	{
		Vector(0.f,0.f,0.f,ObjectSelect.Angle);
	}
	else if(Type >= MODEL_WING+100 && Type <= MODEL_WING+129)
	{
		Vector(0.f,0.f,0.f,ObjectSelect.Angle);
	}
#endif

#ifdef LDK_ADD_RUDOLPH_PET
	else if( Type == MODEL_HELPER+67 )
	{
		Position[1] -= 0.05f;
		Vector(270.f, 40.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef YDG_ADD_SKELETON_PET
	else if( Type == MODEL_HELPER+123 )
	{
		Position[1] -= 0.05f;
		Vector(270.f, 40.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef YDG_ADD_HEALING_SCROLL
	else if(Type == MODEL_POTION+140)
	{
		Position[1] += 0.09f;
		Vector(0.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef LJH_ADD_RARE_ITEM_TICKET_FROM_7_TO_12
	else if(Type >= MODEL_POTION+145 && Type <= MODEL_POTION+150)
	{
		Position[0] += 0.010f;
		Position[1] += 0.040f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef LJH_ADD_FREE_TICKET_FOR_DOPPELGANGGER_BARCA_BARCA_7TH

	else if(Type >= MODEL_HELPER+125 && Type <= MODEL_HELPER+127)
	{
		Position[0] += 0.007f;
		Position[1] -= 0.035f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef ASG_ADD_CHARGED_CHANNEL_TICKET
	else if (Type == MODEL_HELPER+124)
	{
		Position[1] -= 0.04f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PJH_ADD_PANDA_PET
	else if( Type == MODEL_HELPER+80 )
	{
		Position[1] -= 0.05f;
		Vector(270.f, 40.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef LDK_ADD_CS7_UNICORN_PET
	else if( Type == MODEL_HELPER+106 )
	{
		Position[0] += 0.01f;
		Position[1] -= 0.05f;
		Vector(255.f, 45.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef LDK_ADD_SNOWMAN_CHANGERING
	else if( Type == MODEL_HELPER+68 )
	{
		Position[0] += 0.02f;
		Position[1] -= 0.02f;
		Vector(300.f, 10.f, 20.f, ObjectSelect.Angle);
	}
#endif
#ifdef PJH_ADD_PANDA_CHANGERING
	else if( Type == MODEL_HELPER+76 )
	{

		Position[1] -= 0.02f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef YDG_ADD_SKELETON_CHANGE_RING
	else if( Type == MODEL_HELPER+122 )
	{
		Position[0] += 0.01f;
		Position[1] -= 0.035f;
		Vector(290.f, -20.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef LJH_ADD_ITEMS_EQUIPPED_FROM_INVENTORY_SYSTEM
	else if( Type == MODEL_HELPER+128 )
	{
		Position[0] += 0.017f;
		Position[1] -= 0.053f;
		Vector(270.f, -20.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELPER+129 )
	{
		Position[0] += 0.012f;
		Position[1] -= 0.045f;
		Vector(270.f, -20.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELPER+134 )
	{
		Position[0] += 0.005f;
		Position[1] -= 0.033f;
		Vector(270.f, -20.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef LJH_ADD_ITEMS_EQUIPPED_FROM_INVENTORY_SYSTEM_PART_2
	else if( Type == MODEL_HELPER+130 )
	{
		Position[0] += 0.007f;
		Position[1] += 0.005f;
		Vector(270.f, -20.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELPER+131 )
	{
		Position[0] += 0.017f;
		Position[1] -= 0.053f;
		Vector(270.f, -20.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELPER+132 )
	{
		Position[0] += 0.007f;
		Position[1] += 0.045f;
		Vector(270.f, -20.f, 0.f, ObjectSelect.Angle);
	}
	else if( Type == MODEL_HELPER+133 )
	{
		Position[0] += 0.017f;
		Position[1] -= 0.053f;
		Vector(270.f, -20.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef YDG_ADD_CS5_REVIVAL_CHARM
	else if( Type == MODEL_HELPER+69 )
	{
		Position[0] += 0.005f;
		Position[1] -= 0.05f;
		Vector(270.f, -30.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef YDG_ADD_CS5_PORTAL_CHARM
	else if( Type == MODEL_HELPER+70 )
	{
		Position[0] += 0.040f;
		Position[1] -= 0.000f;
		Vector(270.f, -0.f, 70.f, ObjectSelect.Angle);
	}
#endif
#ifdef ASG_ADD_CS6_GUARD_CHARM
	else if (Type == MODEL_HELPER+81)
	{
		Position[0] += 0.005f;
		Position[1] += 0.035f;
		Vector(-90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef ASG_ADD_CS6_ITEM_GUARD_CHARM
	else if (Type == MODEL_HELPER+82)
	{
		Position[0] += 0.005f;
		Position[1] += 0.035f;
		Vector(-90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef ASG_ADD_CS6_ASCENSION_SEAL_MASTER
	else if (Type == MODEL_HELPER+93)
	{
		Position[0] += 0.005f;
		Vector(-90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef ASG_ADD_CS6_WEALTH_SEAL_MASTER
	else if (Type == MODEL_HELPER+94)
	{
		Position[0] += 0.005f;
		Vector(-90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PBG_ADD_SANTAINVITATION

	else if( Type == MODEL_HELPER+66 )
	{
		Position[0] += 0.01f;
		Position[1] -= 0.05f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#ifdef KJH_PBG_ADD_SEVEN_EVENT_2008

	else if( Type == MODEL_POTION+100 )
	{
		Position[0] += 0.01f;
		Position[1] -= 0.05f;
		Vector(0.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#ifdef YDG_ADD_FIRECRACKER_ITEM
	else if (Type == MODEL_POTION+99)
	{
		Position[0] += 0.02f;
		Position[1] -= 0.03f;

		Vector(290.f,-40.f,0.f,ObjectSelect.Angle);
	}
#endif
#ifdef LDK_ADD_GAMBLERS_WEAPONS
	else if( Type == MODEL_STAFF+33 )
	{
		Position[0] += 0.02f;
		Position[1] -= 0.06f;
		Vector(180.f,90.f,10.f,ObjectSelect.Angle);
	}
	else if( Type == MODEL_STAFF+34 )
	{
		Position[1] -= 0.05f;
		Vector(180.f,90.f,10.f,ObjectSelect.Angle);
	}
	else if( Type == MODEL_SPEAR+11 )
	{
		Position[1] += 0.02f;
		Vector(180.f,90.f,15.f,ObjectSelect.Angle);
	}
	else if( Type == MODEL_MACE+18 )
	{
		Position[0] -= 0.03f;
		Position[1] += 0.06f;
		Vector(180.f,90.f,2.f,ObjectSelect.Angle);
	}
	else if( Type == MODEL_BOW+24 )
	{
		Position[0] -= 0.07f;
		Position[1] += 0.07f;
		Vector(180.f,-90.f,15.f,ObjectSelect.Angle);
	}
#endif
#ifdef YDG_ADD_SKILL_FLAME_STRIKE
	else if(Type == MODEL_WING+47)
	{
		Position[0] += 0.005f;
		Position[1] -= 0.015f;
		Vector(270.f,0.f,0.f,ObjectSelect.Angle);
	}
#endif
#ifdef LDK_ADD_GAMBLE_RANDOM_ICON

	else if ( Type==MODEL_HELPER+71 || Type==MODEL_HELPER+72 || Type==MODEL_HELPER+73 || Type==MODEL_HELPER+74 || Type==MODEL_HELPER+75 )
	{
		Position[1] += 0.07f;
		Vector(270.f,180.f,0.f,ObjectSelect.Angle);
		if(Select != 1)
		{
			ObjectSelect.Angle[1] = WorldTime*0.2f;
		}
	}
#endif
#ifdef LDS_ADD_CS6_CHARM_MIX_ITEM_WING
	else if( Type >= MODEL_TYPE_CHARM_MIXWING+EWS_BEGIN
		&& Type <= MODEL_TYPE_CHARM_MIXWING+EWS_END )
	{
		Position[1] -= 0.03f;
		Vector(-90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef LDS_ADD_PCROOM_ITEM_JPN_6TH
	else if(Type == MODEL_HELPER+96)
	{
		Position[0] -= 0.001f;
		Position[1] += 0.028f;
		Vector(90.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PBG_ADD_CHARACTERCARD

	else if(Type == MODEL_HELPER+97 || Type == MODEL_HELPER+98 || Type == MODEL_POTION+91)
	{
		Position[1] -= 0.04f;
		Position[0] += 0.002f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#ifdef PBG_ADD_CHARACTERSLOT
	else if(Type == MODEL_HELPER+99)
	{
		Position[0] += 0.002f;
		Position[1] += 0.025f;
		Vector(270.0f, 180.0f, 45.0f, ObjectSelect.Angle);
	}
#endif
#ifdef PBG_ADD_SECRETITEM

	else if(Type >= MODEL_HELPER+117 && Type <= MODEL_HELPER+120)
	{
		Position[0] += 0.01f;
		Position[1] -= 0.05f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#ifdef YDG_ADD_DOPPELGANGER_ITEM
	else if ( Type==MODEL_POTION+110 )
	{
		Position[0] += 0.005f;
		Position[1] -= 0.02f;
	}
	else if ( Type==MODEL_POTION+111 )
	{
		Position[0] += 0.01f;
		Position[1] -= 0.02f;
	}
#endif
#ifdef YDG_ADD_CS7_CRITICAL_MAGIC_RING
	else if(Type == MODEL_HELPER+107)
	{

		Position[0] -= 0.0f;
		Position[1] += 0.0f;
		Vector(90.0f, 225.0f, 45.0f, ObjectSelect.Angle);
	}
#endif
#ifdef YDG_ADD_CS7_MAX_AG_AURA
	else if(Type == MODEL_HELPER+104)
	{

		Position[0] += 0.01f;
		Position[1] -= 0.03f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#ifdef YDG_ADD_CS7_MAX_SD_AURA
	else if(Type == MODEL_HELPER+105)
	{

		Position[0] += 0.01f;
		Position[1] -= 0.03f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#ifdef YDG_ADD_CS7_PARTY_EXP_BONUS_ITEM
	else if(Type == MODEL_HELPER+103)
	{

		Position[0] += 0.01f;
		Position[1] += 0.01f;
		Vector(0.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#ifdef YDG_ADD_CS7_ELITE_SD_POTION
	else if(Type == MODEL_POTION+133)
	{

		Position[0] += 0.01f;
		Position[1] -= 0.0f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#ifdef LDK_ADD_EMPIREGUARDIAN_ITEM
	else if( MODEL_POTION+101 <= Type && Type <= MODEL_POTION+109 )
	{
		switch(Type)
		{
		case MODEL_POTION+101:
			{
				Position[0] += 0.005f;

			}break;
		case MODEL_POTION+102:
			{
				Position[0] += 0.005f;
				Position[1] += 0.05f;
				Vector(0.0f, 0.0f, 30.0f, ObjectSelect.Angle);
			}break;
		case MODEL_POTION+103:
		case MODEL_POTION+104:
		case MODEL_POTION+105:
		case MODEL_POTION+106:
		case MODEL_POTION+107:
		case MODEL_POTION+108:
			{
				Position[0] += 0.005f;
				Position[1] += 0.05f;
				Vector(0.0f, 0.0f, 30.0f, ObjectSelect.Angle);
			}break;
		case MODEL_POTION+109:
			{
				Position[0] += 0.005f;
				Position[1] += 0.05f;
				Vector(0.0f, 0.0f, 0.0f, ObjectSelect.Angle);
			}break;
		}
	}
#endif
#if defined(LDS_ADD_INGAMESHOP_ITEM_RINGSAPPHIRE) || defined(LDS_ADD_INGAMESHOP_ITEM_RINGRUBY) || defined(LDS_ADD_INGAMESHOP_ITEM_RINGTOPAZ) || defined(LDS_ADD_INGAMESHOP_ITEM_RINGAMETHYST)
	else if( Type >= MODEL_HELPER+109 && Type <= MODEL_HELPER+112 )
	{

		Position[0] += 0.025f;
		Position[1] -= 0.035f;
		Vector(270.0f, 25.0f, 25.0f, ObjectSelect.Angle);
	}
#endif
#if defined(LDS_ADD_INGAMESHOP_ITEM_AMULETRUBY) || defined(LDS_ADD_INGAMESHOP_ITEM_AMULETEMERALD) || defined(LDS_ADD_INGAMESHOP_ITEM_AMULETSAPPHIRE)
	else if( Type >= MODEL_HELPER+113 && Type <= MODEL_HELPER+115 )
	{

		Position[0] += 0.005f;
		Position[1] -= 0.00f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#if defined(LDS_ADD_INGAMESHOP_ITEM_KEYSILVER) || defined(LDS_ADD_INGAMESHOP_ITEM_KEYGOLD)
	else if( Type >= MODEL_POTION+112 && Type <= MODEL_POTION+113 )
	{

		Position[0] += 0.05f;
		Position[1] += 0.009f;
		Vector(270.0f, 180.0f, 45.0f, ObjectSelect.Angle);
	}
#endif
#ifdef LDK_ADD_INGAMESHOP_GOBLIN_GOLD

	else if( Type == MODEL_POTION+120 )
	{
		Position[0] += 0.01f;
		Position[1] += 0.05f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);

	}
#endif
#ifdef LDK_ADD_INGAMESHOP_PACKAGE_BOX
	else if( MODEL_POTION+134 <= Type && Type <= MODEL_POTION+139 )
	{
		Position[0] += 0.00f;
		Position[1] += 0.05f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#ifdef LDK_ADD_INGAMESHOP_NEW_WEALTH_SEAL
	else if( Type == MODEL_HELPER+116 )
	{
#ifdef PBG_FIX_ITEMANGLE
		Position[1] -= 0.03f;
		Position[0] += 0.005f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
#else
		Position[1] += 0.08f;
		Vector(90.f, 0.f, 0.f, ObjectSelect.Angle);
#endif
	}
#endif
#ifdef LDS_ADD_INGAMESHOP_ITEM_PRIMIUMSERVICE6
	else if( Type >= MODEL_POTION+114 && Type <= MODEL_POTION+119 )
	{
		Position[0] += 0.00f;
		Position[1] += 0.06f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#ifdef LDS_ADD_INGAMESHOP_ITEM_COMMUTERTICKET4
	else if( Type >= MODEL_POTION+126 && Type <= MODEL_POTION+129 )
	{
		Position[0] += 0.00f;
		Position[1] += 0.06f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#ifdef LDS_ADD_INGAMESHOP_ITEM_SIZECOMMUTERTICKET3
	else if( Type >= MODEL_POTION+130 && Type <= MODEL_POTION+132 )
	{
		Position[0] += 0.00f;
		Position[1] += 0.06f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
#endif
#ifdef LDS_ADD_INGAMESHOP_ITEM_PASSCHAOSCASTLE
	else if( Type == MODEL_HELPER+121 )
	{
		Position[1] -= 0.04f;
		Vector(270.f, 0.f, 0.f, ObjectSelect.Angle);
	}
#endif
#ifdef PBG_ADD_NEWCHAR_MONK_ITEM
	else if(Type == MODEL_HELM+59 )
	{
		Position[1] -= 0.05f;
		Vector(-90.f,25.f,0.f,ObjectSelect.Angle);
	}
	else if(Type==MODEL_WING+49)
	{
		Vector ( -90.f, 0.f, 0.f, ObjectSelect.Angle );
	}
	else if(Type==MODEL_WING+50)
	{
		Position[1] += 0.05f;
		Vector(270.f,-10.f,0.f,ObjectSelect.Angle);
	}
#endif

#ifdef ADD_SOCKET_ITEM

	switch (Type)
	{
	case MODEL_SWORD+26:
		{
			Position[0] -= 0.02f;
			Position[1] += 0.04f;
			Vector(180.f,270.f,10.f,ObjectSelect.Angle);
		}break;
	case MODEL_SWORD+27:
		{
			Vector(180.f,270.f,15.f,ObjectSelect.Angle);
		}break;
	case MODEL_SWORD+28:
		{
			Position[1] += 0.02f;
			Vector(180.f,270.f,10.f,ObjectSelect.Angle);
		}break;
	case MODEL_MACE+16:
		{
			Position[0] -= 0.02f;
			Vector(180.f,270.f,15.f,ObjectSelect.Angle);
		}
		break;
	case MODEL_MACE+17:
		{
			Position[0] -= 0.02f;
			Position[1] += 0.04f;
			Vector(180.f,270.f,15.f,ObjectSelect.Angle);
		}break;

	case MODEL_STAFF+30:
		{
			Vector(180.f,90.f,10.f,ObjectSelect.Angle);
		}break;
	case MODEL_STAFF+31:
		{
			Vector(180.f,90.f,10.f,ObjectSelect.Angle);
		}break;
	case MODEL_STAFF+32:
		{
			Vector(180.f,90.f,10.f,ObjectSelect.Angle);
		}break;
	}
#endif

#ifdef LDK_FIX_CAOS_THUNDER_STAFF_ROTATION

	switch(Type)
	{
	case MODEL_STAFF+7:
		{
			Vector(0.f,0.f,205.f,ObjectSelect.Angle);
		}break;
	}
#endif

#ifdef KJH_FIX_20080904_INVENTORY_ITEM_RENDER
	switch(Type)
	{
	case MODEL_WING+8:
	case MODEL_WING+9:
	case MODEL_WING+10:
	case MODEL_WING+11:
		{
			Position[0] += 0.005f;
			Position[1] -= 0.02f;
		}break;
	case MODEL_POTION+21:
		{
			Position[0] += 0.005f;
			Position[1] -= 0.005f;
		}break;
	case MODEL_POTION+13:
	case MODEL_POTION+14:
	case MODEL_POTION+22:
		{
			Position[0] += 0.005f;
			Position[1] += 0.015f;
		}break;
	}
#endif

	if(1==Select)
	{
		ObjectSelect.Angle[1] = WorldTime*0.45f;
	}

	ObjectSelect.Type = Type;
	if(ObjectSelect.Type>=MODEL_HELM && ObjectSelect.Type<MODEL_BOOTS+MAX_ITEM_INDEX)
		ObjectSelect.Type = MODEL_PLAYER;
	else if(ObjectSelect.Type==MODEL_POTION+12)
	{
		if(Level==0)
		{
			ObjectSelect.Type = MODEL_EVENT;
			Type = MODEL_EVENT;
		}
		else if(Level==2)
		{
			ObjectSelect.Type = MODEL_EVENT+1;
			Type = MODEL_EVENT+1;
		}
	}

	BMD *b = &Models[ObjectSelect.Type];
	b->CurrentAction                 = 0;
	ObjectSelect.AnimationFrame      = 0;
	ObjectSelect.PriorAnimationFrame = 0;
	ObjectSelect.PriorAction         = 0;
	if(Type >= MODEL_HELM && Type<MODEL_HELM+MAX_ITEM_INDEX)
		b->BodyHeight = -160.f;
	else if(Type >= MODEL_ARMOR && Type<MODEL_ARMOR+MAX_ITEM_INDEX)
		b->BodyHeight = -100.f;
	else if(Type >= MODEL_GLOVES && Type<MODEL_GLOVES+MAX_ITEM_INDEX)
		b->BodyHeight = -70.f;
	else if(Type >= MODEL_PANTS && Type<MODEL_PANTS+MAX_ITEM_INDEX)
		b->BodyHeight = -50.f;
	else
		b->BodyHeight = 0.f;
	float Scale  = 0.f;

	if(Type>=MODEL_HELM && Type<MODEL_BOOTS+MAX_ITEM_INDEX)
	{
		if(Type>=MODEL_HELM && Type<MODEL_HELM+MAX_ITEM_INDEX)
		{
			Scale = MODEL_HELM+39 <= Type && MODEL_HELM+44 >= Type ? 0.007f : 0.0039f;
#ifdef LDS_FIX_ELFHELM_CILPIDREI_RESIZE
			if( Type == MODEL_HELM+31)
				Scale = 0.007f;
#endif
		}
		else if(Type>=MODEL_ARMOR && Type<MODEL_ARMOR+MAX_ITEM_INDEX)
			Scale = 0.0039f;
		else if(Type>=MODEL_GLOVES && Type<MODEL_GLOVES+MAX_ITEM_INDEX)
			Scale = 0.0038f;
		else if(Type>=MODEL_PANTS && Type<MODEL_PANTS+MAX_ITEM_INDEX)
			Scale = 0.0033f;
		else if(Type>=MODEL_BOOTS && Type<MODEL_BOOTS+MAX_ITEM_INDEX)
			Scale = 0.0032f;
#ifndef LDS_FIX_ELFHELM_CILPIDREI_RESIZE
		else if( Type == MODEL_HELM+31)
			Scale = 0.007f;
#endif
		else if (Type == MODEL_ARMOR+30)
			Scale = 0.0035f;
		else if (Type == MODEL_ARMOR+32)
			Scale = 0.0035f;
		else if (Type == MODEL_ARMOR+29)
			Scale = 0.0033f;

		if(Type == MODEL_ARMOR+34)
			Scale = 0.0032f;
		else if(Type == MODEL_ARMOR+35)
			Scale = 0.0032f;
		else if(Type == MODEL_GLOVES+38)
			Scale = 0.0032f;
	}
	else
	{
		if(Type==MODEL_WING+6)
			Scale = 0.0015f;
		else if(Type==MODEL_COMPILED_CELE || Type==MODEL_COMPILED_SOUL)
			Scale = 0.004f;
		else if ( Type>=MODEL_WING+32 && Type<=MODEL_WING+34)
		{
			Scale = 0.001f;
			Position[1] -= 0.05f;
		}
#ifdef ADD_SEED_SPHERE_ITEM
		else if(Type >= MODEL_WING+60 && Type <= MODEL_WING+65)
			Scale = 0.0022f;
		else if(Type >= MODEL_WING+70 && Type <= MODEL_WING+74)
			Scale = 0.0017f;
		else if(Type >= MODEL_WING+100 && Type <= MODEL_WING+129)
			Scale = 0.0017f;
#endif
		else if(Type>=MODEL_WING && Type<MODEL_WING+MAX_ITEM_INDEX)
		{
			Scale = 0.002f;
		}

		else
			if ( Type==MODEL_POTION+45 || Type==MODEL_POTION+49)
			{
				Scale = 0.003f;
			}
			else
				if(Type>=MODEL_POTION+46 && Type<=MODEL_POTION+48)
				{
					Scale = 0.0025f;
				}
				else
					if(Type == MODEL_POTION+50)
					{
						Scale = 0.001f;

					}
#ifdef GIFT_BOX_EVENT
					else
						if ( Type>=MODEL_POTION+32 && Type<=MODEL_POTION+34)
						{
							Scale = 0.002f;
							Position[1] += 0.05f;
							Vector(0.f,ObjectSelect.Angle[1],0.f,ObjectSelect.Angle);
						}
						else
							if ( Type>=MODEL_EVENT+21 && Type<=MODEL_EVENT+23)
							{
								Scale = 0.002f;
								if(Type==MODEL_EVENT+21)
									Position[1] += 0.08f;
								else
									Position[1] += 0.06f;
								Vector(0.f,ObjectSelect.Angle[1],0.f,ObjectSelect.Angle);
							}
#endif
							else if(Type==MODEL_POTION+21)
								Scale = 0.002f;
							else if(Type == MODEL_BOW+19)
								Scale = 0.002f;
							else if(Type==MODEL_EVENT+11)
								Scale = 0.0015f;
							else if ( Type==MODEL_HELPER+4 )
								Scale = 0.0015f;
							else if ( Type==MODEL_HELPER+5 )
								Scale = 0.005f;
							else if ( Type==MODEL_HELPER+30 )
								Scale = 0.002f;
							else if ( Type==MODEL_EVENT+16 )
								Scale = 0.002f;
#ifdef MYSTERY_BEAD
							else if ( Type==MODEL_EVENT+19 )
								Scale = 0.0025f;
#endif
							else if(Type==MODEL_HELPER+16)
								Scale = 0.002f;
							else if(Type==MODEL_HELPER+17)
								Scale = 0.0018f;
							else if(Type==MODEL_HELPER+18)
								Scale = 0.0018f;
#ifdef CSK_FREE_TICKET

							else if(Type == MODEL_HELPER+46)
							{
								Scale = 0.0018f;
							}
							else if(Type == MODEL_HELPER+47)
							{
								Scale = 0.0018f;
							}
							else if(Type == MODEL_HELPER+48)
							{
								Scale = 0.0018f;
							}
#endif
#ifdef CSK_CHAOS_CARD

							else if(Type == MODEL_POTION+54)
							{
								Scale = 0.0024f;
							}
#endif
#ifdef CSK_RARE_ITEM

							else if(Type == MODEL_POTION+58)
							{
								Scale = 0.0012f;
							}
							else if(Type == MODEL_POTION+59 || Type == MODEL_POTION+60)
							{
								Scale = 0.0010f;
							}
							else if(Type == MODEL_POTION+61 || Type == MODEL_POTION+62)
							{
								Scale = 0.0009f;
							}
#endif
#ifdef CSK_LUCKY_CHARM
							else if( Type == MODEL_POTION+53 )
							{
								Scale = 0.00078f;
							}
#endif
#ifdef CSK_LUCKY_SEAL
							else if( Type == MODEL_HELPER+43 || Type == MODEL_HELPER+44 || Type == MODEL_HELPER+45 )
							{
								Scale = 0.0021f;
							}
#endif
#ifdef PSW_ELITE_ITEM
							else if( Type >= MODEL_POTION+70 && Type <= MODEL_POTION+71 )
							{
								Scale = 0.0028f;
							}
#endif
#ifdef PSW_SCROLL_ITEM
							else if( Type >= MODEL_POTION+72 && Type <= MODEL_POTION+77 )
							{
								Scale = 0.0025f;
							}
#endif
#ifdef PSW_SEAL_ITEM
							else if( Type == MODEL_HELPER+59 )
							{
								Scale = 0.0008f;
							}
#endif
#ifdef PSW_FRUIT_ITEM
							else if( Type >= MODEL_HELPER+54 && Type <= MODEL_HELPER+58 )
							{
								Scale = 0.004f;
							}
#endif
#ifdef PSW_SECRET_ITEM
							else if( Type >= MODEL_POTION+78 && Type <= MODEL_POTION+82 )
							{
								Scale = 0.0025f;
							}
#endif
#ifdef PSW_INDULGENCE_ITEM
							else if( Type == MODEL_HELPER+60 )
							{
								Scale = 0.005f;
							}
#endif
#ifdef PSW_CURSEDTEMPLE_FREE_TICKET
							else if( Type == MODEL_HELPER+61 )
							{
								Scale = 0.0018f;
							}
#endif
#ifdef PSW_RARE_ITEM
							else if(Type == MODEL_POTION+83)
							{
								Scale = 0.0009f;
							}
#endif
#ifdef CSK_LUCKY_SEAL
							else if( Type == MODEL_HELPER+43 || Type == MODEL_HELPER+44 || Type == MODEL_HELPER+45 )
							{
								Scale = 0.0021f;
							}
#endif
#ifdef PSW_CHARACTER_CARD
							else if(Type == MODEL_POTION+91)
							{
								Scale = 0.0034f;
							}
#endif
#ifdef PSW_NEW_CHAOS_CARD
							else if(Type == MODEL_POTION+92)
							{
								Scale = 0.0024f;
							}
							else if(Type == MODEL_POTION+93)
							{
								Scale = 0.0024f;
							}
							else if(Type == MODEL_POTION+95)
							{
								Scale = 0.0024f;
							}
#endif
#ifdef PSW_NEW_ELITE_ITEM
							else if( Type == MODEL_POTION+94 )
							{
								Scale = 0.0022f;
							}
#endif
#ifdef CSK_EVENT_CHERRYBLOSSOM
							else if( Type == MODEL_POTION+84 )
							{
								Scale = 0.0031f;
							}
							else if( Type == MODEL_POTION+85 )
							{
								Scale = 0.0044f;
							}
							else if( Type == MODEL_POTION+86 )
							{
								Scale = 0.0031f;
							}
							else if( Type == MODEL_POTION+87 )
							{
								Scale = 0.0061f;
							}
							else if( Type == MODEL_POTION+88 )
							{
								Scale = 0.0035f;
							}
							else if( Type == MODEL_POTION+89 )
							{
								Scale = 0.0035f;
							}
							else if( Type == MODEL_POTION+90 )
							{
								Scale = 0.0035f;
							}
#endif
#ifdef PSW_ADD_PC4_SEALITEM
							else if(Type >= MODEL_HELPER+62 && Type <= MODEL_HELPER+63)
							{
								Scale = 0.002f;
							}
#endif
#ifdef PSW_ADD_PC4_SCROLLITEM
							else if(Type >= MODEL_POTION+97 && Type <= MODEL_POTION+98)
							{
								Scale = 0.003f;
							}
#endif
#ifdef PSW_ADD_PC4_CHAOSCHARMITEM
							else if( Type == MODEL_POTION+96 )
							{
								Scale = 0.0028f;
							}
#endif
#ifdef LDK_ADD_PC4_GUARDIAN
							else if( MODEL_HELPER+64 == Type || Type == MODEL_HELPER+65 )
							{
								switch(Type)
								{
								case MODEL_HELPER+64: Scale = 0.0005f; break;
								case MODEL_HELPER+65: Scale = 0.0016f; break;
								}
							}
#endif
#ifdef LDK_ADD_RUDOLPH_PET
							else if( Type == MODEL_HELPER+67 )
							{
								Scale = 0.0015f;
							}
#endif
#ifdef PJH_ADD_PANDA_PET
							else if( Type == MODEL_HELPER+80 )
							{
								Scale = 0.0020f;
							}
#endif
#ifdef LDK_ADD_SNOWMAN_CHANGERING
							else if( Type == MODEL_HELPER+68 )
							{
								Scale = 0.0026f;
							}
#endif
#ifdef PJH_ADD_PANDA_CHANGERING
							else if( Type == MODEL_HELPER+76 )
							{
								Scale = 0.0026f;
							}
#endif
#ifdef YDG_ADD_CS5_REVIVAL_CHARM
							else if( Type == MODEL_HELPER+69 )
							{
								Scale = 0.0023f;
							}
#endif
#ifdef YDG_ADD_CS5_PORTAL_CHARM
							else if( Type == MODEL_HELPER+70 )
							{
								Scale = 0.0018f;
							}
#endif
#ifdef ASG_ADD_CS6_GUARD_CHARM
							else if (Type == MODEL_HELPER+81)
								Scale = 0.0012f;
#endif
#ifdef ASG_ADD_CS6_ITEM_GUARD_CHARM
							else if (Type == MODEL_HELPER+82)
								Scale = 0.0012f;
#endif
#ifdef ASG_ADD_CS6_ASCENSION_SEAL_MASTER
							else if (Type == MODEL_HELPER+93)
								Scale = 0.0021f;
#endif
#ifdef ASG_ADD_CS6_WEALTH_SEAL_MASTER
							else if (Type == MODEL_HELPER+94)
								Scale = 0.0021f;
#endif
							else if(Type==MODEL_SWORD+19)
							{
								if ( ItemLevel>=0 )
								{
									Scale = 0.0025f;
								}
								else
								{
									Scale = 0.001f;
									ItemLevel = 0;
								}
							}
							else if(Type==MODEL_STAFF+10)
							{
								if ( ItemLevel>=0 )
								{
									Scale = 0.0019f;
								}
								else
								{
									Scale = 0.001f;
									ItemLevel = 0;
								}
							}
							else if(Type==MODEL_BOW+18)
							{
								if ( ItemLevel>=0 )
								{
									Scale = 0.0025f;
								}
								else
								{
									Scale = 0.0015f;
									ItemLevel = 0;
								}
							}
							else if ( Type>=MODEL_MACE+8 && Type<=MODEL_MACE+11 )
							{
								Scale = 0.003f;
							}
							else if(Type == MODEL_MACE+12)
							{
								Scale = 0.0025f;
							}
#ifdef LDK_ADD_GAMBLERS_WEAPONS
							else if( Type == MODEL_MACE+18 )
							{
								Scale = 0.0024f;
							}
#endif
							else if(Type == MODEL_EVENT+12)
							{
								Scale = 0.0012f;
							}
							else if(Type == MODEL_EVENT+13)
							{
								Scale = 0.0025f;
							}
							else if ( Type == MODEL_EVENT+14)
							{
								Scale = 0.0028f;
							}
							else if ( Type == MODEL_EVENT+15)
							{
								Scale = 0.0023f;
							}
							else if ( Type>=MODEL_POTION+22 && Type<MODEL_POTION+25 )
							{
								Scale = 0.0025f;
							}
							else if ( Type>=MODEL_POTION+25 && Type<MODEL_POTION+27 )
							{
								Scale = 0.0028f;
							}
							else if ( Type == MODEL_POTION+63)
							{
								Scale = 0.007f;
							}
#ifdef YDG_ADD_FIRECRACKER_ITEM
							else if ( Type == MODEL_POTION+99)
							{
								Scale = 0.0025f;
							}
#endif

							else if ( Type == MODEL_POTION+52)
							{
								Scale = 0.0014f;
							}
#ifdef _PVP_MURDERER_HERO_ITEM
							else if ( Type==MODEL_POTION+30 )
							{
								Scale = 0.002f;
							}
#endif
							else if( Type==MODEL_HELPER+38 )
							{
								Scale = 0.0025f;
							}
							else if( Type==MODEL_POTION+41 )
							{
								Scale = 0.0035f;
							}
							else if( Type==MODEL_POTION+42 )
							{
								Scale = 0.005f;
							}
							else if( Type==MODEL_POTION+43 )
							{
								Position[1] += -0.005f;
								Scale = 0.0035f;
							}
							else if( Type==MODEL_POTION+44 )
							{
								Position[1] += -0.005f;
								Scale = 0.004f;
							}
							else if ( Type==MODEL_POTION+7 )
							{
								Scale = 0.0025f;
							}
#ifdef CSK_LUCKY_SEAL
							else if( Type == MODEL_HELPER+43 || Type == MODEL_HELPER+44 || Type == MODEL_HELPER+45 )
							{
								Scale = 0.0021f;
							}
#endif
							else if ( Type==MODEL_HELPER+7 )
							{
								Scale = 0.0025f;
							}
							else if ( Type==MODEL_HELPER+11 )
							{
								Scale = 0.0025f;
							}

							else if(Type == MODEL_HELPER+32)
							{
								Scale = 0.0019f;
							}
							else if(Type == MODEL_HELPER+33)
							{
								Scale = 0.004f;
							}
							else if(Type == MODEL_HELPER+34)
							{
								Scale = 0.004f;
							}
							else if(Type == MODEL_HELPER+35)
							{
								Scale = 0.004f;
							}
							else if(Type == MODEL_HELPER+36)
							{
								Scale = 0.007f;
							}
							else if(Type == MODEL_HELPER+37)
							{
								Scale = 0.005f;
							}
							else if( Type == MODEL_BOW+21)
							{
								Scale = 0.0022f;
							}
#ifdef CSK_PCROOM_ITEM
							else if(Type >= MODEL_POTION+55 && Type <= MODEL_POTION+57)
							{
								Scale = 0.0014f;
							}
#endif
							else if(Type == MODEL_HELPER+49)
							{
								Scale = 0.0013f;
							}
							else if(Type == MODEL_HELPER+50)
							{
								Scale = 0.003f;
							}
							else if(Type == MODEL_HELPER+51)
							{
								Scale = 0.003f;
							}
							else if(Type == MODEL_POTION+64)
							{
								Scale = 0.003f;
							}
							else if (Type == MODEL_POTION+65)
								Scale = 0.003f;
							else if (Type == MODEL_POTION+66)
								Scale = 0.0035f;
							else if (Type == MODEL_POTION+67)
								Scale = 0.0035f;
							else if (Type == MODEL_POTION+68)
								Scale = 0.003f;
							else if (Type == MODEL_HELPER+52)
								Scale = 0.005f;
							else if (Type == MODEL_HELPER+53)
								Scale = 0.005f;

							else if(Type == MODEL_SWORD+24)
							{
								Scale = 0.0028f;
							}
							else if(Type == MODEL_BOW+22)
							{
								Scale = 0.0020f;
							}
#ifdef ADD_SOCKET_ITEM
							else if( Type == MODEL_BOW+23 )
							{
								Scale = 0.0032f;
							}
#endif
							else if( Type==MODEL_HELPER+14 || Type==MODEL_HELPER+15 )
							{
								Scale = 0.003f;
							}
#ifdef KJH_PBG_ADD_SEVEN_EVENT_2008

							else if(Type == MODEL_POTION+100)
							{
								Scale = 0.0040f;
							}
#endif
							else if(Type>=MODEL_POTION && Type<MODEL_POTION+MAX_ITEM_INDEX)
							{
								Scale = 0.0035f;
							}
							else if(Type>=MODEL_SPEAR && Type<MODEL_SPEAR+MAX_ITEM_INDEX)
							{
#ifdef LDK_FIX_INVENTORY_SPEAR_SCALE

								if(Type == MODEL_SPEAR+10)
									Scale = 0.0018f;
#ifdef LDK_ADD_GAMBLERS_WEAPONS
								else if( Type == MODEL_SPEAR+11 )
									Scale = 0.0025f;
#endif
								else
									Scale = 0.0021f;
#else
								if(MODEL_SPEAR+10)
									Scale = 0.0018f;
								else
									Scale = 0.0021f;
#endif
							}
							else if(Type>=MODEL_STAFF && Type<MODEL_STAFF+MAX_ITEM_INDEX)
							{
								if (Type >= MODEL_STAFF+14 && Type <= MODEL_STAFF+20)
									Scale = 0.0028f;
								else if (Type >= MODEL_STAFF+21 && Type <= MODEL_STAFF+29)
									Scale = 0.004f;
#ifdef LDK_ADD_GAMBLERS_WEAPONS
								else if( Type == MODEL_STAFF+33 )
									Scale = 0.0028f;
								else if( Type == MODEL_STAFF+34 )
									Scale = 0.0028f;
#endif
								else
									Scale = 0.0022f;
							}
							else if(Type==MODEL_BOW+15)
								Scale = 0.0011f;
							else if(Type==MODEL_BOW+7)
								Scale = 0.0012f;
							else if(Type==MODEL_EVENT+6)
								Scale = 0.0039f;
							else if(Type==MODEL_EVENT+8)
								Scale = 0.0015f;
							else if(Type==MODEL_EVENT+9)
								Scale = 0.0019f;
							else
							{
								Scale = 0.0025f;
							}

#ifdef LDS_ADD_CS6_CHARM_MIX_ITEM_WING
							if( Type >= MODEL_TYPE_CHARM_MIXWING+EWS_BEGIN
								&& Type <= MODEL_TYPE_CHARM_MIXWING+EWS_END )
							{
								Scale = 0.0020f;
							}
#endif

#ifdef USE_EVENT_ELDORADO
							if(Type==MODEL_EVENT+10)
							{
								Scale = 0.001f;
							}
#endif
#ifdef LDS_ADD_PCROOM_ITEM_JPN_6TH
							else if(Type == MODEL_HELPER+96)
							{
								Scale = 0.0031f;
							}
#endif
							else if(Type >= MODEL_ETC+19 && Type <= MODEL_ETC+27)
							{
								Scale = 0.0023f;
							}
#ifdef PBG_ADD_SANTAINVITATION
							else if(Type == MODEL_HELPER+66)
							{
								Scale = 0.0020f;
							}
#endif
#ifdef YDG_ADD_HEALING_SCROLL
							else if(Type == MODEL_POTION+140)
							{
								Scale = 0.0026f;
							}
#endif
#ifdef YDG_ADD_SKELETON_CHANGE_RING
							else if( Type == MODEL_HELPER+122 )
							{
								Scale = 0.0033f;
							}
#endif
#ifdef YDG_ADD_SKELETON_PET
							else if( Type == MODEL_HELPER+123 )
							{
								Scale = 0.0009f;
							}
#endif
#ifdef LJH_ADD_RARE_ITEM_TICKET_FROM_7_TO_12
							else if(Type >= MODEL_POTION+145 && Type <= MODEL_POTION+150)
							{
								Scale = 0.0018f;
							}
#endif
#ifdef LJH_ADD_FREE_TICKET_FOR_DOPPELGANGGER_BARCA_BARCA_7TH

							else if(Type >= MODEL_HELPER+125 && Type <= MODEL_HELPER+127)
							{
								Scale = 0.0013f;
							}
#endif
#ifdef LJH_ADD_ITEMS_EQUIPPED_FROM_INVENTORY_SYSTEM
							else if( Type == MODEL_HELPER+128 )
							{
								Scale = 0.0035f;
							}
							else if( Type == MODEL_HELPER+129 )
							{
								Scale = 0.0035f;
							}
							else if( Type == MODEL_HELPER+134 )
							{
								Scale = 0.0033f;
							}
#endif
#ifdef LJH_ADD_ITEMS_EQUIPPED_FROM_INVENTORY_SYSTEM_PART_2
							else if( Type == MODEL_HELPER+130 )
							{
								Scale = 0.0032f;
							}
							else if( Type == MODEL_HELPER+131 )
							{
								Scale = 0.0033f;
							}
							else if( Type == MODEL_HELPER+132 )
							{
								Scale = 0.0025f;
							}
							else if( Type == MODEL_HELPER+133 )
							{
								Scale = 0.0033f;
							}
#endif
#ifdef LDK_ADD_GAMBLE_RANDOM_ICON

							else if ( Type==MODEL_HELPER+71 || Type==MODEL_HELPER+72 || Type==MODEL_HELPER+73 || Type==MODEL_HELPER+74 || Type==MODEL_HELPER+75 )
							{
								Scale = 0.0019f;
							}
#endif
#ifdef LDK_ADD_GAMBLERS_WEAPONS
							else if( Type == MODEL_BOW+24 )
							{
								Scale = 0.0023f;
							}
#endif
#ifdef PBG_ADD_CHARACTERCARD

							else if(Type == MODEL_HELPER+97 || Type == MODEL_HELPER+98 || Type == MODEL_POTION+91)
							{
								Scale = 0.0028f;
							}
#endif
#ifdef PBG_ADD_CHARACTERSLOT
							else if(Type == MODEL_HELPER+99)
							{
								Scale = 0.0025f;
							}
#endif
#ifdef PBG_ADD_SECRETITEM

							else if(Type >= MODEL_HELPER+117 && Type <= MODEL_HELPER+120)
							{

#ifdef PBG_MOD_SECRETITEM
								Scale = 0.0022f;
#else
								Scale = 0.0035f;
#endif
							}
#endif
#ifdef YDG_ADD_DOPPELGANGER_ITEM
							else if (Type == MODEL_POTION+110)
							{
								Scale = 0.004f;
							}
#endif
#ifdef YDG_ADD_CS7_CRITICAL_MAGIC_RING
							else if(Type == MODEL_HELPER+107)
							{

								Scale = 0.0034f;
							}
#endif
#ifdef YDG_ADD_CS7_ELITE_SD_POTION
							else if(Type == MODEL_POTION+133)
							{
								Scale = 0.0030f;
							}
#endif
#ifdef YDG_ADD_CS7_MAX_SD_AURA
							else if(Type == MODEL_HELPER+105)
							{
								Scale = 0.002f;
							}
#endif
#ifdef LDK_ADD_EMPIREGUARDIAN_ITEM
							else if( MODEL_POTION+101 <= Type && Type <= MODEL_POTION+109 )
							{
								switch(Type)
								{
								case MODEL_POTION+101:
									{
										Scale = 0.004f;
									}break;
								case MODEL_POTION+102:
									{
										Scale = 0.005f;
									}break;
								case MODEL_POTION+103:
								case MODEL_POTION+104:
								case MODEL_POTION+105:
								case MODEL_POTION+106:
								case MODEL_POTION+107:
								case MODEL_POTION+108:
									{
										Scale = 0.004f;
									}break;
								case MODEL_POTION+109:
									{
										Scale = 0.003f;
									}break;
								}
							}
#endif
#ifdef LDK_ADD_CS7_UNICORN_PET
							else if( Type == MODEL_HELPER+106 )
							{
								Scale = 0.0015f;
							}
#endif
#ifdef LDK_ADD_INGAMESHOP_SMALL_WING
							else if( Type == MODEL_WING+130 )
							{
								Scale = 0.0012f;
							}
#endif
#ifdef LDK_ADD_INGAMESHOP_PACKAGE_BOX
							else if( Type >= MODEL_POTION+134 && Type <= MODEL_POTION+139 )
							{
								Scale = 0.0050f;
							}
#endif
#if defined(LDS_ADD_INGAMESHOP_ITEM_RINGSAPPHIRE)||defined(LDS_ADD_INGAMESHOP_ITEM_RINGRUBY)||defined(LDS_ADD_INGAMESHOP_ITEM_RINGTOPAZ)||defined(LDS_ADD_INGAMESHOP_ITEM_RINGAMETHYST)
							else if( Type >= MODEL_HELPER+109 && Type <= MODEL_HELPER+112  )
							{
								Scale = 0.0045f;
							}
#endif
#if defined(LDS_ADD_INGAMESHOP_ITEM_AMULETRUBY)||defined(LDS_ADD_INGAMESHOP_ITEM_AMULETEMERALD)||defined(LDS_ADD_INGAMESHOP_ITEM_AMULETSAPPHIRE)
							else if( Type >= MODEL_HELPER+113 && Type <= MODEL_HELPER+115 )
							{
								Scale = 0.0018f;
							}
#endif
#if defined(LDS_ADD_INGAMESHOP_ITEM_KEYSILVER) || defined(LDS_ADD_INGAMESHOP_ITEM_KEYGOLD)
							else if( Type >= MODEL_POTION+112 && Type <= MODEL_POTION+113 )
							{
								Scale = 0.0032f;
							}
#endif
#ifdef LDK_ADD_INGAMESHOP_NEW_WEALTH_SEAL
							else if( Type == MODEL_HELPER+116 )
							{
								Scale = 0.0021f;
							}
#endif
#ifdef LDS_ADD_INGAMESHOP_ITEM_PRIMIUMSERVICE6
							else if( Type >= MODEL_POTION+114 && Type <= MODEL_POTION+119 )
							{
								Scale = 0.0038f;
							}
#endif
#ifdef LDS_ADD_INGAMESHOP_ITEM_COMMUTERTICKET4
							else if( Type >= MODEL_POTION+126 && Type <= MODEL_POTION+129 )
							{
								Scale = 0.0038f;
							}
#endif
#ifdef LDS_ADD_INGAMESHOP_ITEM_SIZECOMMUTERTICKET3
							else if( Type >= MODEL_POTION+130 && Type <= MODEL_POTION+132 )
							{
								Scale = 0.0038f;
							}
#endif
#ifdef LDS_ADD_INGAMESHOP_ITEM_PASSCHAOSCASTLE
							else if( Type == MODEL_HELPER+121 )
							{
								Scale = 0.0018f;

							}
#endif
#ifdef ASG_ADD_CHARGED_CHANNEL_TICKET
							else if(Type == MODEL_HELPER+124)
								Scale = 0.0018f;
#endif
#ifdef PBG_ADD_NEWCHAR_MONK_ITEM
							else if(Type >= MODEL_WING+49 && Type <= MODEL_WING+50)
							{
								Scale = 0.002f;
							}
#endif
	}

	b->Animation(BoneTransform,ObjectSelect.AnimationFrame,ObjectSelect.PriorAnimationFrame,ObjectSelect.PriorAction,ObjectSelect.Angle,ObjectSelect.HeadAngle,false,false);

	CHARACTER Armor;
	OBJECT *o      = &Armor.Object;
	o->Type        = Type;
	ItemObjectAttribute(o);
	o->LightEnable = false;
	Armor.Class    = 2;

#ifdef PBG_ADD_ITEMRESIZE
	int ScreenPos_X=0, ScreenPos_Y=0;
	Projection(Position,&ScreenPos_X, &ScreenPos_Y);
	if(g_pInGameShop->IsInGameShopRect(ScreenPos_X, ScreenPos_Y))
	{
		o->Scale = Scale * g_pInGameShop->GetRateScale();
	}
	else
#endif
#ifdef NEW_USER_INTERFACE
		if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PARTCHARGE_SHOP) == true) {
			float ChangeScale = (640.f/static_cast<float>(TheShell().GetWindowSize().x))*3.7f;
			o->Scale = Scale-(Scale/ChangeScale);
		}
		else
#endif
		{
#ifdef MOD_RESOLUTION_BY_UI_RENDER_ITEM_RESIZING

			switch(m_iViewType)
			{
			case 1:
				o->Scale = Scale;
				break;
			case 2:
				o->Scale = Scale * 0.8;
				break;
			case 3:
				o->Scale = Scale * 0.6;
				break;
			}
#else
			o->Scale = Scale;
#endif
		}

		VectorCopy(Position,o->Position);

		vec3_t Light;
		float  alpha = o->Alpha;

		Vector(1.f,1.f,1.f,Light);

		RenderPartObject(o,Type,NULL,Light,alpha,ItemLevel,Option1,ExtOption,true,true,true);
}

bool CGFxMainUi::RenderModel()
{
	if(m_bVisible == FALSE)
		return TRUE;

	if(!m_pUIMovie)
		return FALSE;

	ITEM* pItem = NULL;
	float x[4];
	float y;
	vec3_t Position;

	switch(m_iViewType)
	{
	case 1:
		x[0] = 640 * (217.0f / 800.0f) + 9.5f;
		x[1] = 640 * (259.0f / 800.0f) + 9.5f;
		x[2] = 640 * (301.0f / 800.0f) + 9.5f;
		x[3] = 640 * (343.0f / 800.0f) + 9.5f;
		y = 480 * (555.0f / 600.0f) + 18.0f;
		break;
	case 2:
		x[0] = 640 * (325.0f / 1024.0f) + 9.5f;
		x[1] = 640 * (367.0f / 1024.0f) + 9.5f;
		x[2] = 640 * (409.0f / 1024.0f) + 9.5f;
		x[3] = 640 * (451.0f / 1024.0f) + 9.5f;
		y = 480 * (718.0f / 768.0f) + 18.0f;
		break;
	case 3:
		x[0] = 640 * (449.0f / 1280.0f) + 9.5f;
		x[1] = 640 * (491.0f / 1280.0f) + 9.5f;
		x[2] = 640 * (533.0f / 1280.0f) + 9.5f;
		x[3] = 640 * (575.0f / 1280.0f) + 9.5f;
		y = 480 * (965.0f / 1024.0f) + 18.0f;
		break;
	}

	for(int i=0; i<MAX_ITEM_SLOT; ++i)
	{
		if(m_iItemType[i] == -1) continue;

		int iIndex = GetHotKeyItemIndex(i);
		if(iIndex != -1)
		{
			ITEM* pItem = g_pMyInventory->FindItem(iIndex);

			if(pItem)
			{
				CreateScreenVector(x[i], y, Position, false);
				RenderObjectScreen(pItem->Type+MODEL_ITEM, pItem->Level, pItem->Option1, pItem->ExtOption, Position, (m_iOverItemSlot == i ? true : false), false);
			}
		}
	}

	return TRUE;
}

bool CGFxMainUi::Render()
{
	if(m_bVisible == FALSE)
		return TRUE;

	if(!m_pUIMovie)
		return FALSE;

	if(m_bWireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	m_pUIMovie->Display();

	if(m_bWireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return TRUE;
}

GFxMovieView* CGFxMainUi::GetMovie()
{
	if(!m_pUIMovie)
		return NULL;

	return m_pUIMovie;
}

void CGFxMainUi::SetSkillClearHotKey()
{
	if(!m_pUIMovie) return;

	for(int i=0; i<MAX_SKILL_HOT_KEY; ++i)
	{
		m_iHotKeySkillIndex[i] = -1;
 		m_isHotKeySkillCantUse[i] = false;
	}

	m_isSkillSlotVisible = false;
	m_iSkillSlotCount = 0;
	m_iPetSlotCount = 0;
	for(int i=0; i<MAX_MAGIC; ++i)
	{
		m_iSkillSlotIndex[i] = -1;
		m_isSkillSlotCantUse[i] = false;
		m_iSkillSlotType[i] = -1;
	}

 	m_pUIMovie->Invoke("_root.scene.SetClearSkillSlot", "");
}

void CGFxMainUi::SetSkillHotKey(int iHotKey, int _skillType, bool _invoke)
{
	if(!m_pUIMovie || _skillType == -1 || iHotKey < 0) return;

	if(iHotKey != 0)
	{
		for(int i=1; i<MAX_SKILL_HOT_KEY; ++i)
		{
			if(m_iHotKeySkillIndex[i] == _skillType)
			{
				m_iHotKeySkillIndex[i] = -1;
				m_iHotKeySkillType[i] = -1;

				if(_invoke)
					m_pUIMovie->Invoke("_root.scene.SetChangeSkillSlot", "%d %d %d %b %d", i, 0, 0, false, 0);

				break;
			}
		}
	}
	else if(iHotKey == 0 && _invoke == false)
	{
		Hero->CurrentSkill = _skillType;
	}

	m_iHotKeySkillIndex[iHotKey] = _skillType;

	if(_invoke)
	{
		int outSkillNum, outTextureNum;
		bool temp = GetSkillNumber(_skillType, &outSkillNum, &outTextureNum);

		if(temp == false)
		{
			m_iHotKeySkillIndex[iHotKey] = -1;
			m_iHotKeySkillType[iHotKey] = -1;
			return;
		}

		m_iHotKeySkillType[iHotKey] = CharacterAttribute->Skill[_skillType];
		bool _bDisable = m_isHotKeySkillCantUse[iHotKey];

		m_pUIMovie->Invoke("_root.scene.SetChangeSkillSlot", "%d %d %d %b %d", iHotKey, outTextureNum, outSkillNum, _bDisable, _skillType);
	}
}

int CGFxMainUi::GetSkillHotKey(int iHotKey)
{
	if(!m_pUIMovie || iHotKey <= 0 ) return -1;

	return m_iHotKeySkillIndex[iHotKey];
}

void  CGFxMainUi::SetSkillSlot()
{
	BYTE bySkillNumber = CharacterAttribute->SkillNumber;

	if(bySkillNumber > 0)
	{

		WORD iSkillType  = 0;
		int _iIndex[MAX_MAGIC];
		int _iType[MAX_MAGIC];
		int _iCount = 0;

		for(int i=0; i<MAX_MAGIC; ++i)
		{
			iSkillType = CharacterAttribute->Skill[i];

			if(iSkillType != 0 && (iSkillType < AT_SKILL_STUN || iSkillType > AT_SKILL_REMOVAL_BUFF))
			{
				BYTE bySkillUseType = SkillAttribute[iSkillType].SkillUseType;

				if(bySkillUseType == SKILL_USE_TYPE_MASTER || bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
				{
					continue;
				}

				int outSkillNum, outTextureNum;
				bool temp = GetSkillNumber(i, &outSkillNum, &outTextureNum);

				if(temp == false) continue;

				_iIndex[_iCount] = i;
				_iType[_iCount] = iSkillType;
				_iCount ++;
			}
		}

		if(m_iSkillSlotCount != _iCount)
		{
			m_iSkillSlotCount = 0;
			memset(m_iSkillSlotIndex, -1, sizeof(m_iSkillSlotIndex));

 			m_pUIMovie->Invoke("_root.scene.SetTooltipSkillClear", "");

			for(int i=0; i<_iCount; i++)
			{
				int outSkillNum, outTextureNum;
				if(GetSkillNumber(_iIndex[i], &outSkillNum, &outTextureNum))
				{
					m_iSkillSlotIndex[i] = _iIndex[i];
					m_iSkillSlotType[i] = _iType[i];
					m_iSkillSlotCount ++;

					m_pUIMovie->Invoke("_root.scene.SetTooltipSkill", "%d %d %b %d", outTextureNum, outSkillNum, false, _iIndex[i]);
				}
			}
		}

		if(Hero->m_pPet == NULL) return;

		for(int i=AT_PET_COMMAND_DEFAULT; i<AT_PET_COMMAND_END; ++i)
		{

			int outSkillNum, outTextureNum;
			bool temp = GetSkillNumber(i, &outSkillNum, &outTextureNum);

			if(temp == false) continue;

			if(m_iSkillSlotIndex[m_iSkillSlotCount + i] == i && m_iSkillSlotType[m_iSkillSlotCount + i] == CharacterAttribute->Skill[i]) continue;

			m_iSkillSlotIndex[m_iSkillSlotCount + i] = i;
			m_iSkillSlotType[m_iSkillSlotCount + i] = CharacterAttribute->Skill[i];
			m_iPetSlotCount ++;

			m_pUIMovie->Invoke("_root.scene.SetTooltipSkill", "%d %d %b %d", outTextureNum, outSkillNum, false, i);
		}
	}
}

bool CGFxMainUi::GetAttackDamage ( int* iMinDamage, int* iMaxDamage )
{
	int AttackDamageMin;
	int AttackDamageMax;

	ITEM *r = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT];
	ITEM *l = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT];
	if ( PickItem.Number>0 && SrcInventory==Inventory )
	{

		switch ( SrcInventoryIndex)
		{
		case EQUIPMENT_WEAPON_RIGHT:
			r = &PickItem;
			break;
		case EQUIPMENT_WEAPON_LEFT:
			l = &PickItem;
			break;
		}
	}
#ifdef ADD_SOCKET_ITEM
	if( GetEquipedBowType( ) == BOWTYPE_CROSSBOW )
#else
	if( (r->Type>=ITEM_BOW+8  && r->Type<ITEM_BOW+15)	||
		(r->Type>=ITEM_BOW+16 && r->Type<ITEM_BOW+17)	||
		(r->Type>=ITEM_BOW+18 && r->Type<ITEM_BOW+MAX_ITEM_INDEX)
		)
#endif
	{
		AttackDamageMin = CharacterAttribute->AttackDamageMinRight;
		AttackDamageMax = CharacterAttribute->AttackDamageMaxRight;
	}
#ifdef ADD_SOCKET_ITEM
	else if( GetEquipedBowType( ) == BOWTYPE_BOW )
#else
	else if((l->Type>=ITEM_BOW && l->Type<ITEM_BOW+7)
		|| l->Type==ITEM_BOW+17
		|| l->Type==ITEM_BOW+20
		|| l->Type == ITEM_BOW+21
		|| l->Type == ITEM_BOW+22
		)
#endif
	{
		AttackDamageMin = CharacterAttribute->AttackDamageMinLeft;
		AttackDamageMax = CharacterAttribute->AttackDamageMaxLeft;
	}
	else if(r->Type == -1)
	{
		AttackDamageMin = CharacterAttribute->AttackDamageMinLeft;
		AttackDamageMax = CharacterAttribute->AttackDamageMaxLeft;
	}
	else if(r->Type >= ITEM_STAFF && r->Type < ITEM_SHIELD)
	{
		AttackDamageMin = CharacterAttribute->AttackDamageMinLeft;
		AttackDamageMax = CharacterAttribute->AttackDamageMaxLeft;
	}
	else
	{
		AttackDamageMin = CharacterAttribute->AttackDamageMinRight;
		AttackDamageMax = CharacterAttribute->AttackDamageMaxRight;
	}

	bool Alpha = false;
	if ( GetBaseClass(Hero->Class)==CLASS_KNIGHT || GetBaseClass(Hero->Class)==CLASS_DARK )
	{
		if ( l->Type>=ITEM_SWORD && l->Type<ITEM_STAFF+MAX_ITEM_INDEX && r->Type>=ITEM_SWORD && r->Type<ITEM_STAFF+MAX_ITEM_INDEX )
		{
			Alpha = true;
			AttackDamageMin = ((CharacterAttribute->AttackDamageMinRight*55)/100+(CharacterAttribute->AttackDamageMinLeft*55)/100);
			AttackDamageMax = ((CharacterAttribute->AttackDamageMaxRight*55)/100+(CharacterAttribute->AttackDamageMaxLeft*55)/100);
		}
	}
	else if(GetBaseClass(Hero->Class) == CLASS_ELF )
	{
		if ( ( r->Type>=ITEM_BOW && r->Type<ITEM_BOW+MAX_ITEM_INDEX ) &&
			( l->Type>=ITEM_BOW && l->Type<ITEM_BOW+MAX_ITEM_INDEX ) )
		{

			if ( ( l->Type==ITEM_BOW+7 && ((l->Level>>3)&15)>=1 ) || ( r->Type==ITEM_BOW+15 && ((r->Level>>3)&15)>=1 ) )
			{
				Alpha = true;
			}
		}
	}
#ifdef PBG_ADD_NEWCHAR_MONK
	else if(GetBaseClass(Hero->Class) == CLASS_RAGEFIGHTER)
	{
		if(l->Type>=ITEM_SWORD && l->Type<ITEM_MACE+MAX_ITEM_INDEX && r->Type>=ITEM_SWORD && r->Type<ITEM_MACE+MAX_ITEM_INDEX)
		{
			Alpha = true;
			AttackDamageMin = ((CharacterAttribute->AttackDamageMinRight+CharacterAttribute->AttackDamageMinLeft)*60/100);
			AttackDamageMax = ((CharacterAttribute->AttackDamageMaxRight+CharacterAttribute->AttackDamageMaxLeft)*65/100);
		}
	}
#endif

	if ( CharacterAttribute->Ability&ABILITY_PLUS_DAMAGE )
	{
		AttackDamageMin += 15;
		AttackDamageMax += 15;
	}

	*iMinDamage = AttackDamageMin;
	*iMaxDamage = AttackDamageMax;

	return Alpha;
}

void CGFxMainUi::SetSkillInfo(int Type)
{
	char lpszName[256];
	int  iMinDamage, iMaxDamage;
	int  HeroClass = GetBaseClass ( Hero->Class );
	int  iMana, iDistance, iSkillMana;

	GString _title = "";
	GString _info = "";
	GString _attrib = "";
	GString _caution = "";

#ifdef PET_SYSTEM

	if ( AT_PET_COMMAND_DEFAULT <= Type && Type < AT_PET_COMMAND_END )
	{
		if ( GetBaseClass(Hero->Class)==CLASS_DARK_LORD )
		{
			int cmdType = Type-AT_PET_COMMAND_DEFAULT;

			_title = GlobalText[1219+cmdType];

			switch ( cmdType )
			{
			case PET_CMD_DEFAULT:
				{
					_info += GlobalText[1223];
					_info += "\n\n";
				}break;

			case PET_CMD_RANDOM:
				{
					_info += GlobalText[1224];
					_info += "\n\n";
				}break;

			case PET_CMD_OWNER:
				{
					_info += GlobalText[1225];
					_info += "\n\n";
				}break;

			case PET_CMD_TARGET:
				{
					_info += GlobalText[1226];
					_info += "\n\n";
				}break;
			}
		}
	}
	else
#endif
	{
		int  AttackDamageMin, AttackDamageMax;
		int  iSkillMinDamage, iSkillMaxDamage;

		int  SkillType = CharacterAttribute->Skill[Type];
		CharacterMachine->GetMagicSkillDamage( CharacterAttribute->Skill[Type], &iMinDamage, &iMaxDamage);
		CharacterMachine->GetSkillDamage( CharacterAttribute->Skill[Type], &iSkillMinDamage, &iSkillMaxDamage );

		GetAttackDamage ( &AttackDamageMin, &AttackDamageMax );

		iSkillMinDamage += AttackDamageMin;
		iSkillMaxDamage += AttackDamageMax;
		GetSkillInformation( CharacterAttribute->Skill[Type], 1, lpszName, &iMana, &iDistance, &iSkillMana);

		if ( CharacterAttribute->Skill[Type]==AT_SKILL_STRONG_PIER && Hero->Weapon[0].Type!=-1 )
		{
			for ( int i=0; i<CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].SpecialNum; i++ )
			{
				if ( CharacterMachine->Equipment[0].Special[i]==AT_SKILL_LONG_SPEAR )
				{
					wsprintf ( lpszName, "%s", GlobalText[1200] );
					break;
				}
			}
		}
		_title = lpszName;

		WORD Dexterity;
		WORD Energy;

		WORD Strength;
		WORD Vitality;
		WORD Charisma;

		Dexterity= CharacterAttribute->Dexterity+ CharacterAttribute->AddDexterity;
		Energy	 = CharacterAttribute->Energy   + CharacterAttribute->AddEnergy;

		Strength	=	CharacterAttribute->Strength+ CharacterAttribute->AddStrength;
		Vitality	=	CharacterAttribute->Vitality+ CharacterAttribute->AddVitality;
		Charisma	=	CharacterAttribute->Charisma+ CharacterAttribute->AddCharisma;

		int skillattackpowerRate = 0;

		StrengthenCapability rightinfo, leftinfo;

		ITEM* rightweapon = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT];
		ITEM* leftweapon  = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT];

		int rightlevel = (rightweapon->Level>>3)&15;

		if( rightlevel >= rightweapon->Jewel_Of_Harmony_OptionLevel )
		{
			g_pUIJewelHarmonyinfo->GetStrengthenCapability( &rightinfo, rightweapon, 1 );
		}

		int leftlevel = (leftweapon->Level>>3)&15;

		if( leftlevel >= leftweapon->Jewel_Of_Harmony_OptionLevel )
		{
			g_pUIJewelHarmonyinfo->GetStrengthenCapability( &leftinfo, leftweapon, 1 );
		}

		if( rightinfo.SI_isSP )
		{
			skillattackpowerRate += rightinfo.SI_SP.SI_skillattackpower;
			skillattackpowerRate += rightinfo.SI_SP.SI_magicalpower;
		}
		if( leftinfo.SI_isSP )
		{
			skillattackpowerRate += leftinfo.SI_SP.SI_skillattackpower;
		}

		if (HeroClass==CLASS_WIZARD || HeroClass==CLASS_SUMMONER)
		{
			if ( CharacterAttribute->Skill[Type]==AT_SKILL_WIZARDDEFENSE || (AT_SKILL_SOUL_UP <= CharacterAttribute->Skill[Type] && CharacterAttribute->Skill[Type] <= AT_SKILL_SOUL_UP+4))
			{
	#ifdef KJH_FIX_WOPS_K29544_SOULBARRIER_UPGRADE_TOOLTIP
				int iDamageShield;

				if( CharacterAttribute->Skill[Type]==AT_SKILL_WIZARDDEFENSE )
				{
					iDamageShield = (int)(10+(Dexterity/50.f)+(Energy/200.f));
				}
				else if((AT_SKILL_SOUL_UP <= CharacterAttribute->Skill[Type]) && (CharacterAttribute->Skill[Type] <= AT_SKILL_SOUL_UP+4))
				{
					iDamageShield = (int)(10+(Dexterity/50.f)+(Energy/200.f)) + ((CharacterAttribute->Skill[Type]-AT_SKILL_SOUL_UP+1)*5);
				}
	#else KJH_FIX_WOPS_K29544_SOULBARRIER_UPGRADE_TOOLTIP
				int iDamageShield = (int)(10+(Dexterity/50.f)+(Energy/200.f));
	#endif
				int iDeleteMana   = (int)(CharacterAttribute->ManaMax*0.02f);
				int iLimitTime    = (int)(60+(Energy/40.f));

				char temp[256];
				ZeroMemory(temp, sizeof(temp));
				sprintf(temp,GlobalText[578],iDamageShield);
				_info += temp;
				_info += "\n\n";
				ZeroMemory(temp, sizeof(temp));
				sprintf(temp,GlobalText[880],iDeleteMana);
				_info += temp;
				_info += "\n\n";
				ZeroMemory(temp, sizeof(temp));
				sprintf(temp,GlobalText[881],iLimitTime);
				_info += temp;
				_info += "\n\n";
			}
	#ifdef KJH_ADD_SKILL_SWELL_OF_MAGICPOWER

			else if( SkillType != AT_SKILL_SWELL_OF_MAGICPOWER )
	#else
			else
	#endif
			{
				WORD bySkill = CharacterAttribute->Skill[Type];
				if (!(AT_SKILL_STUN <= bySkill && bySkill <= AT_SKILL_MANA)
					&& !(AT_SKILL_ALICE_THORNS <= bySkill && bySkill <= AT_SKILL_ALICE_ENERVATION)
					&& bySkill != AT_SKILL_TELEPORT
					&& bySkill != AT_SKILL_TELEPORT_B)
				{
	#ifdef ASG_ADD_SUMMON_RARGLE
					if (AT_SKILL_SUMMON_EXPLOSION <= bySkill && bySkill <= AT_SKILL_SUMMON_POLLUTION)
	#else
					if (AT_SKILL_SUMMON_EXPLOSION <= bySkill && bySkill <= AT_SKILL_SUMMON_REQUIEM)
	#endif
					{
						CharacterMachine->GetCurseSkillDamage(bySkill, &iMinDamage, &iMaxDamage);

						char temp[256];
						ZeroMemory(temp, sizeof(temp));
						sprintf(temp, GlobalText[1692], iMinDamage, iMaxDamage);
						_info += temp;
						_info += "\n\n";
					}
					else
					{
						char temp[256];
						ZeroMemory(temp, sizeof(temp));
						sprintf(temp,GlobalText[170],iMinDamage + skillattackpowerRate,iMaxDamage + skillattackpowerRate);
						_info += temp;
						_info += "\n\n";
					}
				}
			}
		}
		if ( HeroClass==CLASS_KNIGHT || HeroClass==CLASS_DARK || HeroClass==CLASS_ELF || HeroClass==CLASS_DARK_LORD
	#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
			|| HeroClass==CLASS_RAGEFIGHTER
	#endif
			)
		{
			switch ( CharacterAttribute->Skill[Type] )
			{
			case AT_SKILL_TELEPORT :
			case AT_SKILL_TELEPORT_B :
			case AT_SKILL_SOUL_UP:
			case AT_SKILL_SOUL_UP+1:
			case AT_SKILL_SOUL_UP+2:
			case AT_SKILL_SOUL_UP+3:
			case AT_SKILL_SOUL_UP+4:

			case AT_SKILL_HEAL_UP:
			case AT_SKILL_HEAL_UP+1:
			case AT_SKILL_HEAL_UP+2:
			case AT_SKILL_HEAL_UP+3:
			case AT_SKILL_HEAL_UP+4:

			case AT_SKILL_LIFE_UP:
			case AT_SKILL_LIFE_UP+1:
			case AT_SKILL_LIFE_UP+2:
			case AT_SKILL_LIFE_UP+3:
			case AT_SKILL_LIFE_UP+4:

			case AT_SKILL_WIZARDDEFENSE :
			case AT_SKILL_BLOCKING :
			case AT_SKILL_VITALITY :
			case AT_SKILL_HEALING :
	#ifdef PJH_SEASON4_MASTER_RANK4
			case AT_SKILL_DEF_POWER_UP:
			case AT_SKILL_DEF_POWER_UP+1:
			case AT_SKILL_DEF_POWER_UP+2:
			case AT_SKILL_DEF_POWER_UP+3:
			case AT_SKILL_DEF_POWER_UP+4:
	#endif
			case AT_SKILL_DEFENSE :
	#ifdef PJH_SEASON4_MASTER_RANK4
			case AT_SKILL_ATT_POWER_UP:
			case AT_SKILL_ATT_POWER_UP+1:
			case AT_SKILL_ATT_POWER_UP+2:
			case AT_SKILL_ATT_POWER_UP+3:
			case AT_SKILL_ATT_POWER_UP+4:
	#endif
			case AT_SKILL_ATTACK :
			case AT_SKILL_SUMMON :
			case AT_SKILL_SUMMON+1 :
			case AT_SKILL_SUMMON+2 :
			case AT_SKILL_SUMMON+3 :
			case AT_SKILL_SUMMON+4 :
			case AT_SKILL_SUMMON+5 :
			case AT_SKILL_SUMMON+6 :
	#ifdef ADD_ELF_SUMMON
			case AT_SKILL_SUMMON+7:
	#endif
			case AT_SKILL_IMPROVE_AG:
			case AT_SKILL_STUN:
			case AT_SKILL_REMOVAL_STUN:
			case AT_SKILL_MANA:
			case AT_SKILL_INVISIBLE:
			case AT_SKILL_REMOVAL_INVISIBLE:
			case AT_SKILL_REMOVAL_BUFF:
				break;
			case AT_SKILL_PARTY_TELEPORT:
			case AT_SKILL_ADD_CRITICAL:
				break;
			case AT_SKILL_ASHAKE_UP:
			case AT_SKILL_ASHAKE_UP+1:
			case AT_SKILL_ASHAKE_UP+2:
			case AT_SKILL_ASHAKE_UP+3:
			case AT_SKILL_ASHAKE_UP+4:
			case AT_SKILL_DARK_HORSE:
				_caution += GlobalText[1237];
				break;
			case AT_SKILL_BRAND_OF_SKILL:
				break;
			case AT_SKILL_PLASMA_STORM_FENRIR:
	#ifdef PBG_FIX_SKILL_RECOVER_TOOLTIP
			case AT_SKILL_RECOVER:
	#endif
	#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
			case AT_SKILL_ATT_UP_OURFORCES:
			case AT_SKILL_HP_UP_OURFORCES:
			case AT_SKILL_DEF_UP_OURFORCES:
	#endif
				break;
			default :
				char temp[256];
				ZeroMemory(temp, sizeof(temp));
				sprintf(temp,GlobalText[879],iSkillMinDamage,iSkillMaxDamage + skillattackpowerRate);
				_info += temp;
				_info += "\n\n";
				break;
			}
		}

		if(CharacterAttribute->Skill[Type] == AT_SKILL_PLASMA_STORM_FENRIR)
		{
			int iSkillDamage;
			GetSkillInformation_Damage(AT_SKILL_PLASMA_STORM_FENRIR, &iSkillDamage);

			if(HeroClass == CLASS_KNIGHT || HeroClass == CLASS_DARK)
			{
				iSkillMinDamage = (Strength/3)+(Dexterity/5)+(Vitality/5)+(Energy/7)+iSkillDamage;
			}
			else if(HeroClass == CLASS_WIZARD || HeroClass == CLASS_SUMMONER)
			{
				iSkillMinDamage = (Strength/5)+(Dexterity/5)+(Vitality/7)+(Energy/3)+iSkillDamage;
			}
			else if(HeroClass == CLASS_ELF)
			{
				iSkillMinDamage = (Strength/5)+(Dexterity/3)+(Vitality/7)+(Energy/5)+iSkillDamage;
			}
			else if(HeroClass == CLASS_DARK_LORD)
			{
				iSkillMinDamage = (Strength/5)+(Dexterity/5)+(Vitality/7)+(Energy/3)+(Charisma/3)+iSkillDamage;
			}
	#ifdef PBG_ADD_NEWCHAR_MONK
			else if(HeroClass == CLASS_RAGEFIGHTER)
			{
				iSkillMinDamage = (Strength/5)+(Dexterity/5)+(Vitality/3)+(Energy/7)+iSkillDamage;
			}
	#endif
			iSkillMaxDamage = iSkillMinDamage + 30;

			char temp[256];
			ZeroMemory(temp, sizeof(temp));
			sprintf(temp,GlobalText[879],iSkillMinDamage,iSkillMaxDamage + skillattackpowerRate);
			_info += temp;
			_info += "\n\n";
		}

		if(GetBaseClass(Hero->Class) == CLASS_ELF)
		{
			bool Success = true;
			switch(CharacterAttribute->Skill[Type])
			{
			case AT_SKILL_HEAL_UP:
			case AT_SKILL_HEAL_UP+1:
			case AT_SKILL_HEAL_UP+2:
			case AT_SKILL_HEAL_UP+3:
			case AT_SKILL_HEAL_UP+4:
				{
					int Cal = (Energy/5)+5;
					char temp[256];
					ZeroMemory(temp, sizeof(temp));
					sprintf(temp,GlobalText[171],(Cal) + (int)((float)Cal*(float)(SkillAttribute[CharacterAttribute->Skill[Type]].Damage/(float)100)));
					_info += temp;
					_info += "\n\n";
				}
				break;
			case AT_SKILL_HEALING:
				{
					char temp[256];
					ZeroMemory(temp, sizeof(temp));
					sprintf(temp,GlobalText[171],Energy/5+5);
					_info += temp;
					_info += "\n\n";
				}
				break;
	#ifdef PJH_SEASON4_MASTER_RANK4
			case AT_SKILL_DEF_POWER_UP:
			case AT_SKILL_DEF_POWER_UP+1:
			case AT_SKILL_DEF_POWER_UP+2:
			case AT_SKILL_DEF_POWER_UP+3:
			case AT_SKILL_DEF_POWER_UP+4:
				{
					int Cal = Energy/8+2;
					char temp[256];
					ZeroMemory(temp, sizeof(temp));
	#ifdef YDG_FIX_MASTERLEVEL_ELF_ATTACK_TOOLTIP
					sprintf(temp,GlobalText[172],(Cal) + (int)((float)Cal/(float)((float)SkillAttribute[CharacterAttribute->Skill[Type]].Damage/(float)10)));
	#else
					sprintf(temp,GlobalText[172],(int)((float)Cal/(float)((float)SkillAttribute[CharacterAttribute->Skill[Type]].Damage/(float)10)));
	#endif
					_info += temp;
					_info += "\n\n";
				}
				break;
	#endif
			case AT_SKILL_DEFENSE:
				{
					char temp[256];
					ZeroMemory(temp, sizeof(temp));
					sprintf(temp,GlobalText[172],Energy/8+2);
					_info += temp;
					_info += "\n\n";
				}
				break;
	#ifdef PJH_SEASON4_MASTER_RANK4
			case AT_SKILL_ATT_POWER_UP:
			case AT_SKILL_ATT_POWER_UP+1:
			case AT_SKILL_ATT_POWER_UP+2:
			case AT_SKILL_ATT_POWER_UP+3:
			case AT_SKILL_ATT_POWER_UP+4:
				{
					int Cal = Energy/7+3;
					char temp[256];
					ZeroMemory(temp, sizeof(temp));
	#ifdef YDG_FIX_MASTERLEVEL_ELF_ATTACK_TOOLTIP
					sprintf(temp,GlobalText[173],(Cal) + (int)((float)Cal/(float)((float)SkillAttribute[CharacterAttribute->Skill[Type]].Damage/(float)10)));
	#else
					sprintf(temp,GlobalText[173],(int)((float)Cal/(float)((float)SkillAttribute[CharacterAttribute->Skill[Type]].Damage/(float)10)));
	#endif
					_info += temp;
					_info += "\n\n";
				}
				break;
	#endif
			case AT_SKILL_ATTACK :
				{
					char temp[256];
					ZeroMemory(temp, sizeof(temp));
					sprintf(temp,GlobalText[173],Energy/7+3);
					_info += temp;
					_info += "\n\n";
				}
				break;
	#ifdef PBG_FIX_SKILL_RECOVER_TOOLTIP
			case AT_SKILL_RECOVER:
				{
					int Cal = Energy/4;
					char temp[256];
					ZeroMemory(temp, sizeof(temp));
					sprintf(temp,GlobalText[1782], (int)((float)Cal+(float)CharacterAttribute->Level));
					_info += temp;
					_info += "\n\n";
				}
				break;
	#endif
			default:Success = false;
			}
		}

	#ifdef KJH_ADD_SKILL_SWELL_OF_MAGICPOWER

		if( SkillType != AT_SKILL_SWELL_OF_MAGICPOWER )
		{
			if(iDistance)
			{
				char temp[256];
				ZeroMemory(temp, sizeof(temp));
				sprintf(temp,GlobalText[174],iDistance);
				_info += temp;
				_info += "\n\n";
			}
		}
	#else
		if(iDistance)
		{
			char temp[256];
			ZeroMemory(temp, sizeof(temp));
			sprintf(temp,GlobalText[174],iDistance);
			_info += temp;
			_info += "\n\n";
		}
	#endif

		char temp[256];
		ZeroMemory(temp, sizeof(temp));
		sprintf(temp,GlobalText[175],iMana);
		_info += temp;
		_info += "\n\n";
		if ( iSkillMana > 0)
		{
			char temp[256];
			ZeroMemory(temp, sizeof(temp));
			sprintf(temp,GlobalText[360],iSkillMana);
			_info += temp;
			_info += "\n\n";
		}
		if ( GetBaseClass(Hero->Class) == CLASS_KNIGHT )
		{
			if ( CharacterAttribute->Skill[Type]==AT_SKILL_SPEAR )
			{
				_caution += GlobalText[96];
			}

			if ( Hero->byExtensionSkill == 1 && CharacterAttribute->Level >= 220 )
			{
				if ( ( CharacterAttribute->Skill[Type] >= AT_SKILL_SWORD1 && CharacterAttribute->Skill[Type] <= AT_SKILL_SWORD5 )
					|| CharacterAttribute->Skill[Type]==AT_SKILL_WHEEL || CharacterAttribute->Skill[Type]==AT_SKILL_FURY_STRIKE
					|| CharacterAttribute->Skill[Type]==AT_SKILL_ONETOONE
	#ifdef PJH_SEASON4_MASTER_RANK4
					|| (AT_SKILL_ANGER_SWORD_UP <= CharacterAttribute->Skill[Type] && CharacterAttribute->Skill[Type] <= AT_SKILL_ANGER_SWORD_UP+4)
					|| (AT_SKILL_BLOW_UP <= CharacterAttribute->Skill[Type] && CharacterAttribute->Skill[Type] <= AT_SKILL_BLOW_UP+4)
	#endif
					|| (AT_SKILL_TORNADO_SWORDA_UP <= CharacterAttribute->Skill[Type] && CharacterAttribute->Skill[Type] <= AT_SKILL_TORNADO_SWORDA_UP+4)
					|| (AT_SKILL_TORNADO_SWORDB_UP <= CharacterAttribute->Skill[Type] && CharacterAttribute->Skill[Type] <= AT_SKILL_TORNADO_SWORDB_UP+4)
					)
				{

					_caution += GlobalText[99];
				}
	#ifdef CSK_FIX_SKILL_BLOWOFDESTRUCTION_COMBO
				else if(CharacterAttribute->Skill[Type] == AT_SKILL_BLOW_OF_DESTRUCTION)
				{

					_caution += GlobalText[2115];
				}
	#endif
			}
		}

		BYTE MasteryType = CharacterMachine->GetSkillMasteryType( CharacterAttribute->Skill[Type] );
		if ( MasteryType!=255 )
		{
			_attrib += GlobalText[1080+MasteryType];
		}

		int SkillUseType;
		int BrandType = SkillAttribute[SkillType].SkillBrand;
		SkillUseType = SkillAttribute[SkillType].SkillUseType;
		if ( SkillUseType==SKILL_USE_TYPE_BRAND )
		{

			char temp[256];
			ZeroMemory(temp, sizeof(temp));
			sprintf ( temp, GlobalText[1480], SkillAttribute[BrandType].Name );
			_caution += temp;

			ZeroMemory(temp, sizeof(temp));
			sprintf ( temp, GlobalText[1481], SkillAttribute[BrandType].Damage );
			_caution += temp;
		}
		SkillUseType = SkillAttribute[SkillType].SkillUseType;
		if ( SkillUseType==SKILL_USE_TYPE_MASTER )
		{
			_caution += GlobalText[1482];
			char temp[256];
			ZeroMemory(temp, sizeof(temp));
			sprintf ( temp, GlobalText[1483], SkillAttribute[SkillType].KillCount );
			_caution += temp;
		}

		if ( GetBaseClass(Hero->Class)==CLASS_DARK_LORD )
		{
			if ( CharacterAttribute->Skill[Type]==AT_SKILL_PARTY_TELEPORT && PartyNumber<=0 )
			{
				_caution += GlobalText[1185];
			}
		}

		if(CharacterAttribute->Skill[Type] == AT_SKILL_PLASMA_STORM_FENRIR)
		{
			_caution += GlobalText[1926];
			_caution += GlobalText[1927];
		}

		if(CharacterAttribute->Skill[Type] == AT_SKILL_INFINITY_ARROW)
		{
			_title = lpszName;
			_caution += GlobalText[2040];
			char temp[256];
			ZeroMemory(temp, sizeof(temp));
			sprintf(temp,GlobalText[175],iMana);
			_info += temp;
			_info += "\n\n";
			ZeroMemory(temp, sizeof(temp));
			sprintf(temp,GlobalText[360],iSkillMana);
			_info += temp;
			_info += "\n\n";
		}

		_info += "\n\n";

		if(CharacterAttribute->Skill[Type] == AT_SKILL_RUSH || CharacterAttribute->Skill[Type] == AT_SKILL_SPACE_SPLIT
			|| CharacterAttribute->Skill[Type] == AT_SKILL_DEEPIMPACT || CharacterAttribute->Skill[Type] == AT_SKILL_JAVELIN
			|| CharacterAttribute->Skill[Type] == AT_SKILL_ONEFLASH || CharacterAttribute->Skill[Type] == AT_SKILL_DEATH_CANNON
	#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
			|| CharacterAttribute->Skill[Type] == AT_SKILL_OCCUPY
	#endif
			)
		{
			_caution += GlobalText[2047];
		}
		if(CharacterAttribute->Skill[Type] == AT_SKILL_STUN || CharacterAttribute->Skill[Type] == AT_SKILL_REMOVAL_STUN
			|| CharacterAttribute->Skill[Type] == AT_SKILL_INVISIBLE || CharacterAttribute->Skill[Type] == AT_SKILL_REMOVAL_INVISIBLE
			|| CharacterAttribute->Skill[Type] == AT_SKILL_REMOVAL_BUFF)
		{
			_caution += GlobalText[2048];
		}
		if(CharacterAttribute->Skill[Type] == AT_SKILL_SPEAR)
		{
			_caution += GlobalText[2079];
		}

	#ifdef KJH_ADD_SKILL_SWELL_OF_MAGICPOWER
		if( SkillType == AT_SKILL_SWELL_OF_MAGICPOWER )
		{
			_attrib += GlobalText[2054];
		}
	#endif
	}

	int strSize = 0;

	char cTitle[256];
	EncodeUtf8(cTitle, _title.ToCStr());

	char cInfo[1024];
	EncodeUtf8(cInfo, _info.ToCStr());

	char cAttr[256];
	EncodeUtf8(cAttr, _attrib.ToCStr());

	char cCaut[256];
	EncodeUtf8(cCaut, _caution.ToCStr());

	m_pUIMovie->Invoke("_root.scene.SetSkillInfo", "%s%s%s%s", cTitle, cInfo, cAttr, cCaut);
}

void CGFxMainUi::SetItemClearHotKey()
{
	if(!m_pUIMovie) return;

	m_iUseItemSlotNum = -1;
	for(int i=0; i<MAX_ITEM_SLOT; i++)
	{
		m_iItemType[i] = -1;
		m_iItemLevel[i] = 0;
		m_iItemCount[i] = 0;
	}

	m_pUIMovie->Invoke("_root.scene.SetClearItemSlot", "");
}

void CGFxMainUi::SetItemHotKey(int _iHotKey, int _iItemType, int _iItemLevel)
{
	if(!m_pUIMovie) return;

	m_iItemType[_iHotKey] = _iItemType;
	m_iItemLevel[_iHotKey] = _iItemLevel;
	m_iItemCount[_iHotKey] = _iItemLevel;

	if(_iHotKey != -1 && CanRegisterItemHotKey(_iItemType) == true && GetHotKeyItemIndex(_iHotKey, true) > 0)
	{
		m_pUIMovie->Invoke("_root.scene.SetChangeItemSlot", "%d %d %d", _iHotKey, m_iItemType[_iHotKey], m_iItemLevel[_iHotKey]);
	}
	else
	{
		m_iItemType[_iHotKey] = -1;
		m_iItemLevel[_iHotKey] = 0;
		m_iItemCount[_iHotKey] = 0;
		m_pUIMovie->Invoke("_root.scene.SetChangeItemSlot", "%d %d %d", _iHotKey, -1, 0);
	}
}

int CGFxMainUi::GetItemHotKey(int iHotKey)
{
	if(!m_pUIMovie || iHotKey == -1 ) return -1;

	return m_iItemType[iHotKey];
}

int CGFxMainUi::GetItemHotKeyLevel(int iHotKey)
{
	if(!m_pUIMovie || iHotKey == -1 ) return 0;

	return m_iItemLevel[iHotKey];
}

bool CGFxMainUi::GetSkillNumber(int skillType, int *outSkillNum, int *outTextureNum)
{

	WORD bySkillType = CharacterAttribute->Skill[skillType];

	if(bySkillType == 0)
	{
		return false;
	}

	if(skillType >= AT_PET_COMMAND_DEFAULT)
	{
		bySkillType = skillType;
	}

	BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;
	int Skill_Icon = SkillAttribute[bySkillType].Magic_Icon;

	int iSkillNum = 0;
	int iKindofSkill = -1;

	float fU,fV;

	if(AT_PET_COMMAND_DEFAULT <= bySkillType && bySkillType <= AT_PET_COMMAND_END)
	{
		iKindofSkill = SKILL_TYPE_COMMAND;
		iSkillNum = ((bySkillType - AT_PET_COMMAND_DEFAULT) % 8 )+ 1;
	}
	else if(bySkillType == AT_SKILL_PLASMA_STORM_FENRIR)
	{
		iKindofSkill = SKILL_TYPE_COMMAND;
		iSkillNum = 5;
	}
	else if((AT_SKILL_ALICE_DRAINLIFE <= bySkillType && bySkillType <= AT_SKILL_ALICE_THORNS)

		)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = ((bySkillType - AT_SKILL_ALICE_DRAINLIFE) % 8) + 37;
	}
	else if(AT_SKILL_ALICE_SLEEP <= bySkillType && bySkillType <= AT_SKILL_ALICE_BLIND)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = ((bySkillType - AT_SKILL_ALICE_SLEEP) % 8) + 41;
	}
#ifdef ASG_ADD_SKILL_BERSERKER
	else if (bySkillType == AT_SKILL_ALICE_BERSERKER)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = 47;
	}
#endif
	else if (AT_SKILL_ALICE_WEAKNESS <= bySkillType && bySkillType <= AT_SKILL_ALICE_ENERVATION)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = (bySkillType - AT_SKILL_ALICE_WEAKNESS) + 45;
	}
	else if(AT_SKILL_SUMMON_EXPLOSION <= bySkillType && bySkillType <= AT_SKILL_SUMMON_REQUIEM)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = ((bySkillType - AT_SKILL_SUMMON_EXPLOSION) % 8) + 43;
	}
#ifdef ASG_ADD_SUMMON_RARGLE
	else if (bySkillType == AT_SKILL_SUMMON_POLLUTION)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = 48;
	}
#endif
#ifdef CSK_ADD_SKILL_BLOWOFDESTRUCTION
	else if (bySkillType == AT_SKILL_BLOW_OF_DESTRUCTION)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = 32;
	}
#endif
#ifdef PJH_SEASON4_DARK_NEW_SKILL_CAOTIC
	else if (bySkillType == AT_SKILL_GAOTIC)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = 100;
	}
#endif
#ifdef PJH_SEASON4_SPRITE_NEW_SKILL_RECOVER
	else if (bySkillType == AT_SKILL_RECOVER)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = 34;
	}
#endif
#ifdef PJH_SEASON4_SPRITE_NEW_SKILL_MULTI_SHOT
	else if (bySkillType == AT_SKILL_MULTI_SHOT)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = 97;
	}
#endif
#ifdef YDG_ADD_SKILL_FLAME_STRIKE
	else if (bySkillType == AT_SKILL_FLAME_STRIKE)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = 98;
	}
#endif
#ifdef YDG_ADD_SKILL_GIGANTIC_STORM
	else if (bySkillType == AT_SKILL_GIGANTIC_STORM)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = 99;
	}
#endif
#ifdef YDG_ADD_SKILL_LIGHTNING_SHOCK
	else if (bySkillType == AT_SKILL_LIGHTNING_SHOCK)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = 39;
	}
#endif
#ifdef PJH_ADD_MASTERSKILL
	else if(AT_SKILL_LIGHTNING_SHOCK_UP <= bySkillType && bySkillType <= AT_SKILL_LIGHTNING_SHOCK_UP+4)
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = (bySkillType - AT_SKILL_LIGHTNING_SHOCK_UP) + 103;
	}
#endif
#ifdef KJH_ADD_SKILL_SWELL_OF_MAGICPOWER
	else if( bySkillType == AT_SKILL_SWELL_OF_MAGICPOWER )
	{
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = 33;
	}
#endif
	else if(bySkillUseType == 4)
	{

		fU = (Skill_Icon % 12) + 1;
		fV = ((Skill_Icon/12)+4) * 12;
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = (int)fU + (int)fV;
	}
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	else if(bySkillType >= AT_SKILL_THRUST)
	{

		fU = (bySkillType - 260) % 12 + 1;
		fV = ((bySkillType - 260) / 12) * 12;
		iKindofSkill = SKILL_TYPE_SKILL3;
		iSkillNum = (int)fU + (int)fV;
	}
#endif
	else if(bySkillType >= 57)
	{

		fU = (bySkillType - 57) % 8 + 1;
		fV = ((bySkillType - 57) / 8) * 12;
		iKindofSkill = SKILL_TYPE_SKILL2;
		iSkillNum = (int)fU + (int)fV;
	}
	else
	{
		iKindofSkill = SKILL_TYPE_SKILL1;
		iSkillNum = bySkillType;
	}

	*outSkillNum = iSkillNum;
	*outTextureNum = iKindofSkill;

	if( iSkillNum == 0 || iKindofSkill == -1 ) return false;

	return true;
}

bool CGFxMainUi::GetSkillDisable(int slotNum, int* _array)
{

	WORD bySkillType = CharacterAttribute->Skill[_array[slotNum]];

	if(bySkillType == 0 || _array[slotNum] == -1) return false;

	if(_array[slotNum] >= AT_PET_COMMAND_DEFAULT)
	{
		bySkillType = _array[slotNum];
	}

#ifdef KJH_ADD_SKILLICON_RENEWAL

	bool bCantSkill = false;
#endif

	BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;
	int Skill_Icon = SkillAttribute[bySkillType].Magic_Icon;

#ifdef PBG_FIX_SKILL_DEMENDCONDITION
	if( !gSkillManager.DemendConditionCheckSkill( bySkillType ) )
	{
		bCantSkill = true;
	}
#endif

	if(IsCanBCSkill(bySkillType) == false)
	{
		bCantSkill = true;
	}

	if( g_isCharacterBuff((&Hero->Object), eBuff_AddSkill) && bySkillUseType == SKILL_USE_TYPE_BRAND )
	{
		bCantSkill = true;
	}

	if(bySkillType == AT_SKILL_SPEAR && ( Hero->Helper.Type<MODEL_HELPER+2 || Hero->Helper.Type>MODEL_HELPER+3 ) && Hero->Helper.Type != MODEL_HELPER+37)
	{
		bCantSkill = true;
	}

	if(bySkillType == AT_SKILL_SPEAR && (Hero->Helper.Type == MODEL_HELPER+2 || Hero->Helper.Type == MODEL_HELPER+3 || Hero->Helper.Type == MODEL_HELPER+37))
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if((iTypeL < ITEM_SPEAR || iTypeL >= ITEM_BOW) && (iTypeR < ITEM_SPEAR || iTypeR >= ITEM_BOW))
		{
			bCantSkill = true;
		}
	}

	if(bySkillType >= AT_SKILL_BLOCKING && bySkillType <= AT_SKILL_SWORD5 && (Hero->Helper.Type == MODEL_HELPER+2 || Hero->Helper.Type == MODEL_HELPER+3 || Hero->Helper.Type == MODEL_HELPER+37))
	{
		bCantSkill = true;
	}

	if((bySkillType == AT_SKILL_ICE_BLADE
#ifdef PJH_SEASON4_MASTER_RANK4
		||(AT_SKILL_POWER_SLASH_UP <= bySkillType && AT_SKILL_POWER_SLASH_UP+4 >= bySkillType)
#endif
		) && (Hero->Helper.Type == MODEL_HELPER+2 || Hero->Helper.Type == MODEL_HELPER+3 || Hero->Helper.Type == MODEL_HELPER+37))
	{
		bCantSkill = true;
	}

#ifdef PJH_SEASON4_MASTER_RANK4

#endif

	int iEnergy = CharacterAttribute->Energy+CharacterAttribute->AddEnergy;

	if(g_csItemOption.IsDisableSkill(bySkillType, iEnergy))
	{
		bCantSkill = true;
	}

	if(bySkillType == AT_SKILL_PARTY_TELEPORT && PartyNumber <= 0)
	{
		bCantSkill = true;
	}

#ifdef YDG_ADD_DOPPELGANGER_UI
	if (bySkillType == AT_SKILL_PARTY_TELEPORT && (IsDoppelGanger1() || IsDoppelGanger2() || IsDoppelGanger3() || IsDoppelGanger4()))
	{
		bCantSkill = true;
	}
#endif

	if(bySkillType == AT_SKILL_DARK_HORSE || (AT_SKILL_ASHAKE_UP <= bySkillType && bySkillType <= AT_SKILL_ASHAKE_UP+4))
	{
		BYTE byDarkHorseLife = 0;
		byDarkHorseLife = CharacterMachine->Equipment[EQUIPMENT_HELPER].Durability;
		if(byDarkHorseLife == 0 || Hero->Helper.Type != MODEL_HELPER+4)
		{
			bCantSkill = true;
		}
	}
#ifdef PJH_FIX_SPRIT

	if( bySkillType>=AT_PET_COMMAND_DEFAULT && bySkillType<AT_PET_COMMAND_END )
	{
		int iCharisma = CharacterAttribute->Charisma+CharacterAttribute->AddCharisma;
		PET_INFO PetInfo;
		giPetManager::GetPetInfo(PetInfo, 421-PET_TYPE_DARK_SPIRIT);
		int RequireCharisma = (185+(PetInfo.m_wLevel*15));
		if( RequireCharisma > iCharisma )
		{
			bCantSkill = true;
		}
	}
#endif

	if( (bySkillType == AT_SKILL_INFINITY_ARROW)
#ifdef KJH_ADD_SKILL_SWELL_OF_MAGICPOWER
		|| (bySkillType == AT_SKILL_SWELL_OF_MAGICPOWER)
#endif
		)
	{
#ifdef PJH_FIX_SKILL
		if(g_csItemOption.IsDisableSkill(bySkillType, iEnergy))
		{
			bCantSkill = true;
		}
#endif
		if( ( g_isCharacterBuff((&Hero->Object), eBuff_InfinityArrow) )
#ifdef KJH_ADD_SKILL_SWELL_OF_MAGICPOWER
			|| ( g_isCharacterBuff((&Hero->Object), eBuff_SwellOfMagicPower) )
#endif
			)
		{
			bCantSkill = true;
		}
	}
#ifdef KJH_FIX_WOPS_K20674_CHECK_STAT_USE_SKILL

	if( bySkillType == AT_SKILL_REDUCEDEFENSE
#ifdef YDG_FIX_BLOCK_STAFF_WHEEL
		|| (AT_SKILL_BLOOD_ATT_UP <= bySkillType && bySkillType <= AT_SKILL_BLOOD_ATT_UP+4)
#endif
		)
	{
		WORD Strength;
		const WORD wRequireStrength = 596;
		Strength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;
		if(Strength < wRequireStrength)
		{
			bCantSkill = true;
		}

#ifdef YDG_FIX_STAFF_FLAMESTRIKE_IN_CHAOSCASLE
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if ( !( iTypeR!=-1 &&
			( iTypeR<ITEM_STAFF || iTypeR>=ITEM_STAFF+MAX_ITEM_INDEX ) &&
			( iTypeL<ITEM_STAFF || iTypeL>=ITEM_STAFF+MAX_ITEM_INDEX ) ) )
		{
			bCantSkill = true;
		}
#endif
	}
#endif

#ifdef LDK_FIX_CHECK_STAT_USE_SKILL_PIERCING

	switch( bySkillType )
	{

	case AT_SKILL_PARALYZE:
		{
			WORD  Dexterity;
			const WORD wRequireDexterity = 646;
			Dexterity = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
			if(Dexterity < wRequireDexterity)
			{
				bCantSkill = true;
			}
		}break;
	}
#endif

#ifdef YDG_FIX_BLOCK_STAFF_WHEEL
	if( bySkillType == AT_SKILL_WHEEL
		|| (AT_SKILL_TORNADO_SWORDA_UP <= bySkillType && bySkillType <= AT_SKILL_TORNADO_SWORDA_UP+4)
		|| (AT_SKILL_TORNADO_SWORDB_UP <= bySkillType && bySkillType <= AT_SKILL_TORNADO_SWORDB_UP+4)
		)
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if ( !( iTypeR!=-1 && ( iTypeR<ITEM_STAFF || iTypeR>=ITEM_STAFF+MAX_ITEM_INDEX ) && ( iTypeL<ITEM_STAFF || iTypeL>=ITEM_STAFF+MAX_ITEM_INDEX ) ) )
		{
			bCantSkill = true;
		}
	}
#endif

	if(InChaosCastle() == true)
	{

		if( bySkillType == AT_SKILL_DARK_HORSE || bySkillType == AT_SKILL_RIDER
			|| (bySkillType >= AT_PET_COMMAND_DEFAULT && bySkillType <= AT_PET_COMMAND_TARGET)
			||(AT_SKILL_ASHAKE_UP <= bySkillType && bySkillType <= AT_SKILL_ASHAKE_UP+4))
		{
			bCantSkill = true;
		}
	}
	else
	{

		if(bySkillType == AT_SKILL_DARK_HORSE || (AT_SKILL_ASHAKE_UP <= bySkillType && bySkillType <= AT_SKILL_ASHAKE_UP+4))
		{
			BYTE byDarkHorseLife = 0;
			byDarkHorseLife = CharacterMachine->Equipment[EQUIPMENT_HELPER].Durability;
			if(byDarkHorseLife == 0)
			{
				bCantSkill = true;
			}
		}
	}

	int iCharisma = CharacterAttribute->Charisma+CharacterAttribute->AddCharisma;

	if(g_csItemOption.IsDisableSkill(bySkillType, iEnergy, iCharisma))
	{
		bCantSkill = true;
	}

#ifdef PJH_FIX_4_BUGFIX_33
	if(g_csItemOption.Special_Option_Check() == false && (bySkillType == AT_SKILL_ICE_BLADE||(AT_SKILL_POWER_SLASH_UP <= bySkillType && AT_SKILL_POWER_SLASH_UP+4 >= bySkillType)))
	{
		bCantSkill = true;
	}

	if(g_csItemOption.Special_Option_Check(1) == false && (bySkillType == AT_SKILL_CROSSBOW||(AT_SKILL_MANY_ARROW_UP <= bySkillType && AT_SKILL_MANY_ARROW_UP+4 >= bySkillType)))
	{
		bCantSkill = true;
	}
#endif

	if(true == false)
	{

	}
#ifdef PJH_SEASON4_SPRITE_NEW_SKILL_MULTI_SHOT
	else if (bySkillType == AT_SKILL_MULTI_SHOT)
	{
		if (GetEquipedBowType_Skill() == BOWTYPE_NONE)
		{
			bCantSkill = true;
		}
	}
#endif
#ifdef YDG_ADD_SKILL_FLAME_STRIKE
	else if (bySkillType == AT_SKILL_FLAME_STRIKE)
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if ( !(iTypeR!=-1 && ( iTypeR<ITEM_STAFF || iTypeR>=ITEM_STAFF+MAX_ITEM_INDEX ) && ( iTypeL<ITEM_STAFF || iTypeL>=ITEM_STAFF+MAX_ITEM_INDEX )) )
		{
			bCantSkill = true;
		}
	}
#endif

#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	if(!g_CMonkSystem.IsSwordformGlovesUseSkill(bySkillType))
	{
		bCantSkill = true;
	}
	if(g_CMonkSystem.IsRideNotUseSkill(bySkillType, Hero->Helper.Type))
	{
		bCantSkill = true;
	}

	ITEM* pLeftRing = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];
	ITEM* pRightRing = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];

	if(g_CMonkSystem.IsChangeringNotUseSkill(pLeftRing->Type, pRightRing->Type, pLeftRing->Level, pRightRing->Level)
		&& (GetBaseClass(Hero->Class) == CLASS_RAGEFIGHTER))
	{
		bCantSkill = true;
	}
#endif

	return bCantSkill;
}

void CGFxMainUi::GetSkillDelay(int skillType, int* _array)
{

	WORD bySkillType = CharacterAttribute->Skill[skillType];
	bool bCantSkill = false;

#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	if((bySkillType == AT_SKILL_GIANTSWING || bySkillType == AT_SKILL_DRAGON_KICK
		|| bySkillType == AT_SKILL_DRAGON_LOWER) && (bCantSkill))
		return;
#endif
}

int CGFxMainUi::GetHotKeyItemIndex(int iType, bool bItemCount)
{
	int iStartItemType, iEndItemType = 0;
	int i, j;

	switch(iType)
	{
	case 0:
		if(GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{

			if(m_iItemType[iType] >= ITEM_POTION+4 && m_iItemType[iType] <= ITEM_POTION+6)
			{
				iStartItemType = ITEM_POTION+6; iEndItemType = ITEM_POTION+4;
			}
			else
			{
				iStartItemType = ITEM_POTION+3; iEndItemType = ITEM_POTION+0;
			}
		}
		break;
	case 1:
		if(GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{

			if(m_iItemType[iType] >= ITEM_POTION+0 && m_iItemType[iType] <= ITEM_POTION+3)
			{
				iStartItemType = ITEM_POTION+3; iEndItemType = ITEM_POTION+0;
			}
			else
			{
				iStartItemType = ITEM_POTION+6; iEndItemType = ITEM_POTION+4;
			}
		}
		break;
	case 2:
		if(GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{

			if(m_iItemType[iType] >= ITEM_POTION+0 && m_iItemType[iType] <= ITEM_POTION+3)
			{
				iStartItemType = ITEM_POTION+3; iEndItemType = ITEM_POTION+0;
			}

			else if(m_iItemType[iType] >= ITEM_POTION+4 && m_iItemType[iType] <= ITEM_POTION+6)
			{
				iStartItemType = ITEM_POTION+6; iEndItemType = ITEM_POTION+4;
			}
			else
			{
				iStartItemType = ITEM_POTION+8; iEndItemType = ITEM_POTION+8;
			}
		}
		break;
	case 3:
		if(GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{

			if(m_iItemType[iType] >= ITEM_POTION+0 && m_iItemType[iType] <= ITEM_POTION+3)
			{
				iStartItemType = ITEM_POTION+3; iEndItemType = ITEM_POTION+0;
			}

			else if(m_iItemType[iType] >= ITEM_POTION+4 && m_iItemType[iType] <= ITEM_POTION+6)
			{
				iStartItemType = ITEM_POTION+6; iEndItemType = ITEM_POTION+4;
			}
			else
			{
				iStartItemType = ITEM_POTION+37; iEndItemType = ITEM_POTION+35;
			}
		}
		break;
	}

	int iItemCount = 0;
	ITEM* pItem = NULL;

	int iNumberofItems = g_pMyInventory->GetInventoryCtrl()->GetNumberOfItems();
	for(i=iStartItemType; i>=iEndItemType; --i)
	{
		if(bItemCount)
		{
			for(j=0; j<iNumberofItems; ++j)
			{
				pItem = g_pMyInventory->GetInventoryCtrl()->GetItem(j);
				if(pItem == NULL)
				{
					continue;
				}

				if(
					(pItem->Type == i && ((pItem->Level>>3)&15) == m_iItemLevel[iType])
					|| (pItem->Type == i && (pItem->Type >= ITEM_POTION+0 && pItem->Type <= ITEM_POTION+3))
					)
				{
					if(pItem->Type == ITEM_POTION+9
						|| pItem->Type == ITEM_POTION+10
						|| pItem->Type == ITEM_POTION+20
						)
					{
						iItemCount++;
					}
					else
					{
						iItemCount += pItem->Durability;
					}
				}
			}
		}
		else
		{
			int iIndex = -1;

			if(i >= ITEM_POTION+0 && i <= ITEM_POTION+3)
			{
				iIndex = g_pMyInventory->FindItemReverseIndex(i);
			}
			else
			{
				iIndex = g_pMyInventory->FindItemReverseIndex(i, m_iItemLevel[iType]);
			}

			if (-1 != iIndex)
			{
				pItem = g_pMyInventory->FindItem(iIndex);
				if((pItem->Type != ITEM_POTION+7
					&& pItem->Type != ITEM_POTION+10
					&& pItem->Type != ITEM_POTION+20)
					|| ((pItem->Level>>3)&15) == m_iItemLevel[iType]
				)
				{
					return iIndex;
				}
			}
		}
	}

	if(bItemCount == true)
	{
		return iItemCount;
	}

	return -1;
}

bool CGFxMainUi::GetHotKeyCommonItem(IN int iHotKey, OUT int& iStart, OUT int& iEnd)
{
	switch(m_iItemType[iHotKey])
	{
	case ITEM_POTION+7:
	case ITEM_POTION+8:
	case ITEM_POTION+9:
	case ITEM_POTION+10:
	case ITEM_POTION+20:
	case ITEM_POTION+46:
	case ITEM_POTION+47:
	case ITEM_POTION+48:
	case ITEM_POTION+49:
	case ITEM_POTION+50:
#ifdef PSW_ELITE_ITEM
	case ITEM_POTION+70:
	case ITEM_POTION+71:
#endif
#ifdef PSW_ELITE_ITEM
	case ITEM_POTION+78:
	case ITEM_POTION+79:
	case ITEM_POTION+80:
	case ITEM_POTION+81:
	case ITEM_POTION+82:
#endif
#ifdef PSW_NEW_ELITE_ITEM
	case ITEM_POTION+94:
#endif
#ifdef CSK_EVENT_CHERRYBLOSSOM
	case ITEM_POTION+85:
	case ITEM_POTION+86:
	case ITEM_POTION+87:
#endif
#ifdef YDG_ADD_CS7_ELITE_SD_POTION
	case ITEM_POTION+133:
#endif

		if(m_iItemType[iHotKey] != ITEM_POTION+20 || m_iItemLevel[iHotKey] == 0)
		{
			iStart = iEnd = m_iItemType[iHotKey];
			return true;
		}
		break;
	default:

		if(m_iItemType[iHotKey] >= ITEM_POTION+35 && m_iItemType[iHotKey] <= ITEM_POTION+37)
		{
			iStart = ITEM_POTION+37; iEnd = ITEM_POTION+35;
			return true;
		}

		else if(m_iItemType[iHotKey] >= ITEM_POTION+38 && m_iItemType[iHotKey] <= ITEM_POTION+40)
		{
			iStart = ITEM_POTION+40; iEnd = ITEM_POTION+38;
			return true;
		}
		break;
	}

	return false;
}

void CMainUIFSCHandler::Callback(GFxMovieView* pmovie, const char* pcommand, const char* parg)
{

	if(strcmp(pcommand, "onClickShopBtn") == 0)
	{
#ifdef FOR_WORK
		DebugAngel_Write("InGameShopStatue.Txt", "CallStack - CNewUIHotKey.UpdateKeyEvent()\r\n");
#endif

		if(g_pInGameShop->IsInGameShopOpen() == false)
			return;

#ifdef KJH_MOD_SHOP_SCRIPT_DOWNLOAD

		if( g_InGameShopSystem->IsScriptDownload() == true )
		{
			if( g_InGameShopSystem->ScriptDownload() == false )
				return;
		}

		if( g_InGameShopSystem->IsBannerDownload() == true )
		{
#ifdef KJH_FIX_INGAMESHOP_INIT_BANNER
			if( g_InGameShopSystem->BannerDownload() == true )
			{

				g_pInGameShop->InitBanner(g_InGameShopSystem->GetBannerFileName(), g_InGameShopSystem->GetBannerURL());
			}
#else
			g_InGameShopSystem->BannerDownload();
#endif
		}
#endif

		if( g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INGAMESHOP) == false)
		{

			if( g_InGameShopSystem->GetIsRequestShopOpenning() == false )
			{
				SendRequestIGS_CashShopOpen(0);
				g_InGameShopSystem->SetIsRequestShopOpenning(true);
			}
		}
		else
		{
			SendRequestIGS_CashShopOpen(1);
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_INGAMESHOP);
		}
		PlayBuffer(SOUND_CLICK01);
	}
	else if(strcmp(pcommand, "onClickCharacterBtn") == 0)
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_CHARACTER);
		PlayBuffer(SOUND_CLICK01);

#ifdef ASG_ADD_UI_QUEST_PROGRESS_ETC
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER))
			g_QuestMng.SendQuestIndexByEtcSelection();
#endif
	}
	else if(strcmp(pcommand, "onClickQuestBtn") == 0)
	{
		if (g_pNPCShop->IsSellingItem() == false)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);
			PlayBuffer(SOUND_CLICK01);
		}
	}
	else if(strcmp(pcommand, "onClickPortalBtn") == 0)
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_MOVEMAP);
		PlayBuffer(SOUND_CLICK01);
	}
	else if(strcmp(pcommand, "onClickCommunityBtn") == 0)
	{
		if(InChaosCastle() == true
#ifndef CSK_FIX_CHAOSFRIENDWINDOW
			&& g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHAOSCASTLE_TIME) == true
#endif
			)
		{
			return;
		}

		int iLevel = CharacterAttribute->Level;
		if(iLevel < 6)
		{
			if(g_pChatListBox->CheckChatRedundancy(GlobalText[1067]) == FALSE)
			{
				g_pChatListBox->AddText("",GlobalText[1067],SEASON3B::TYPE_SYSTEM_MESSAGE);
			}
		}
		else
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_FRIEND);
		}

		PlayBuffer(SOUND_CLICK01);
	}
	else if(strcmp(pcommand, "onClickSystemBtn") == 0)
	{
		if(g_MessageBox->IsEmpty())
		{
			SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CSystemMenuMsgBoxLayout));
			PlayBuffer(SOUND_CLICK01);
		}
	}
}

void CMainUIEIHandler::Callback(GFxMovieView* pmovieView, const char* methodName, const GFxValue* args, UInt argCount)
{
	pMainUi->GetLog()->LogMessage("\nCallback! %s, nargs = %d\n", (methodName)?methodName:"(null)", argCount);
	for (UInt i = 0; i < argCount; ++i)
	{
		switch(args[i].GetType())
		{
		case GFxValue::VT_Null:		pMainUi->GetLog()->LogMessage("NULL"); break;
		case GFxValue::VT_String:	pMainUi->GetLog()->LogMessage("%s", args[i].GetString()); break;
		case GFxValue::VT_Number:	pMainUi->GetLog()->LogMessage("%f", args[i].GetNumber()); break;
		case GFxValue::VT_Boolean:	pMainUi->GetLog()->LogMessage("%s", (args[i].GetBool())?"true":"false"); break;

		default:
			pMainUi->GetLog()->LogMessage("unknown"); break;
			break;
		}
		pMainUi->GetLog()->LogMessage("%s", (i == argCount - 1) ? "" : ", ");
	}
	pMainUi->GetLog()->LogMessage("\n");

	if(strcmp(methodName, "error") == 0 )
	{

		int aa = -1;

		for(int i=0; i<argCount; i++)
		{
			switch(args[i].GetType())
			{
			case GFxValue::VT_String:
				break;

			case GFxValue::VT_Number:
				aa = args[i].GetNumber();
				break;
			}
		}

	}
	else if(strcmp(methodName, "plzSetSkill") == 0)
	{

		pMainUi->SetSkillSlot();
	}
	else if(strcmp(methodName, "plzSkillInfo") == 0 && argCount == 1 && args[0].GetType() == GFxValue::VT_Number)
	{
		int temp = args[0].GetNumber();
		pMainUi->SetSkillInfo(temp);
	}
	else if(strcmp(methodName, "skillSlotVisible") == 0 && argCount == 1 && args[0].GetType() == GFxValue::VT_Boolean)
	{
		pMainUi->SetSkillSlotVisible(args[0].GetBool());
	}
	else if(strcmp(methodName, "ItemSlotPosition") == 0 && argCount == 3 && args[0].GetType() == GFxValue::VT_Number)
	{
		int slotNum = (int)args[0].GetNumber();

	}
	else if(strcmp(methodName, "onClickItemBtn") == 0 && argCount == 3 && args[0].GetType() == GFxValue::VT_Number)
	{

		pMainUi->SetUseItemSlotNum( (int)args[0].GetNumber() );
		int _type = args[1].GetNumber();
		int _count = args[2].GetNumber();
	}
	else if(strcmp(methodName, "onOverItemBtn") == 0 && argCount == 3 && args[0].GetType() == GFxValue::VT_Number)
	{

		pMainUi->SetOverItemSlotNum( (int)args[0].GetNumber() );
		int _type = args[1].GetNumber();
		int _count = args[2].GetNumber();
	}
	else if(strcmp(methodName, "onSkillSet") == 0 && argCount == 4 && args[0].GetType() == GFxValue::VT_Number )
	{

		int _texture = (int)args[0].GetNumber();
		int _skill = (int)args[1].GetNumber();
		bool _disabled = args[2].GetBool();
		int _return = (int)args[3].GetNumber();

		pMainUi->SetSkillHotKey(0, _return, false);
	}
	else if(strcmp(methodName, "onChangeSkill") == 0 && argCount == 5 && args[0].GetType() == GFxValue::VT_Number )
	{
		int _slot = (int)args[0].GetNumber();
		int _texture = (int)args[1].GetNumber();
		int _skill = (int)args[2].GetNumber();
		bool _disabled = args[3].GetBool();
		int _return = (int)args[4].GetNumber();

		pMainUi->SetSkillHotKey(_slot, _return, false);
	}
	else if(strcmp(methodName, "SetVisiblePopup") == 0 && argCount == 6 && args[0].GetType() == GFxValue::VT_Number)
	{
		CGFXBase* tempClass = GFxProcess::GetInstancePtr()->Find(GFxRegistType::eGFxRegist_InfoPupup);

		if( tempClass != NULL )
		{
			GFxMovieView* tempMovie = tempClass->GetMovie();
			tempMovie->Invoke("_root.scene.SetVisiblePopup", "%d %b %d %d %s %s", (int)args[0].GetNumber(), args[1].GetBool(), (int)args[2].GetNumber(), (int)args[3].GetNumber(), args[4].GetString(), args[5].GetString());
		}
	}
	else if(strcmp(methodName, "SetVisibleInfoPopup") == 0 && argCount == 7 && args[0].GetType() == GFxValue::VT_Boolean)
	{
		CGFXBase* tempClass = GFxProcess::GetInstancePtr()->Find(GFxRegistType::eGFxRegist_InfoPupup);

		if( tempClass != NULL )
		{
			GFxMovieView* tempMovie = tempClass->GetMovie();
			tempMovie->Invoke("_root.scene.SetVisibleInfoPopup", "%b %d %d %s %s %s %s", args[0].GetBool(), (int)args[1].GetNumber(), (int)args[2].GetNumber(), args[3].GetString(), args[4].GetString(), args[5].GetString(), args[6].GetString());
		}
	}
	else if(strcmp(methodName, "SetItemCount") == 0 && argCount == 3 && args[0].GetType() == GFxValue::VT_Number)
	{
		CGFXBase* tempClass = GFxProcess::GetInstancePtr()->Find(GFxRegistType::eGFxRegist_InfoPupup);

		if( tempClass != NULL )
		{
			GFxMovieView* tempMovie = tempClass->GetMovie();
			tempMovie->Invoke("_root.scene.SetItemCount", "%d %s %b", (int)args[0].GetNumber(), args[1].GetString(), args[2].GetBool());
		}
	}
}

#endif
