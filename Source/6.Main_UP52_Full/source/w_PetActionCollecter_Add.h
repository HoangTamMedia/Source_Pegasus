
#ifdef PJH_ADD_PANDA_PET

#include "w_PetAction.h"
#include "ZzzBMD.h"
#include "w_PetActionCollecter.h"

#pragma once

BoostSmartPointer(PetActionCollecterAdd);
class PetActionCollecterAdd : public PetAction
{
public:
	static PetActionCollecterAddPtr Make();
	virtual ~PetActionCollecterAdd();
	virtual bool Release(OBJECT* obj, CHARACTER* Owner);

public:
	virtual bool Model(OBJECT* obj, CHARACTER* Owner, int targetKey, double tick, bool bForceRender);
	virtual bool Move(OBJECT* obj, CHARACTER* Owner, int targetKey, double tick, bool bForceRender);
	virtual bool Effect(OBJECT* obj, CHARACTER* Owner, int targetKey, double tick, bool bForceRender);
	virtual bool Sound(OBJECT* obj, CHARACTER* Owner, int targetKey, double tick, bool bForceRender);

	void FindZen(OBJECT* obj);
	bool CompTimeControl(const DWORD& dwCompTime, DWORD& dwTime);

public:
	typedef std::list< RootingItem > ItemList;
	enum ActionState
	{
		eAction_Stand = 0,
		eAction_Move = 1,
		eAction_Get = 2,
		eAction_Return = 3,

		eAction_End_NotUse,
	};

protected:
	PetActionCollecterAdd();

	RootingItem m_RootItem;
	bool m_isRooting;

	DWORD m_dwSendDelayTime;
	DWORD m_dwRootingTime;
	DWORD m_dwRoundCountDelay;
	ActionState m_state;

	float m_fRadWidthStand;
	float m_fRadWidthGet;

};
#endif

BoostSmartPointer(PetActionCollecterSkeleton);
class PetActionCollecterSkeleton : public PetActionCollecterAdd
{
public:
	static PetActionCollecterSkeletonPtr Make();
	virtual ~PetActionCollecterSkeleton();
	virtual bool Release(OBJECT* obj, CHARACTER* Owner);

public:
	virtual bool Move(OBJECT* obj, CHARACTER* Owner, int targetKey, double tick, bool bForceRender);
	virtual bool Effect(OBJECT* obj, CHARACTER* Owner, int targetKey, double tick, bool bForceRender);

protected:
	PetActionCollecterSkeleton();

	BOOL m_bIsMoving;
};
