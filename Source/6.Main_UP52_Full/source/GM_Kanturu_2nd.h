
#if !defined(AFX_GM_KANTURU_IN_H__37F1E344_37B1_497A_9B04_84409C768A74__INCLUDED_)
#define AFX_GM_KANTURU_IN_H__37F1E344_37B1_497A_9B04_84409C768A74__INCLUDED_

#pragma once

#include "ZzzBMD.h"

namespace M38Kanturu2nd
{

	bool Create_Kanturu2nd_Object(OBJECT* o);
	bool Move_Kanturu2nd_Object(OBJECT* o);
	bool Render_Kanturu2nd_ObjectVisual(OBJECT* o, BMD* b);
	bool Render_Kanturu2nd_ObjectMesh(OBJECT* o, BMD* b,bool ExtraMon = 0);
	void Render_Kanturu2nd_AfterObjectMesh(OBJECT* o, BMD* b);

	CHARACTER*	Create_Kanturu2nd_Monster(int iType, int PosX, int PosY, int Key);
	bool	Set_CurrentAction_Kanturu2nd_Monster(CHARACTER* c, OBJECT* o);
	bool	AttackEffect_Kanturu2nd_Monster(CHARACTER* c, OBJECT* o, BMD* b);
	bool	Move_Kanturu2nd_MonsterVisual(CHARACTER* c,OBJECT* o, BMD* b);
	void	Move_Kanturu2nd_BlurEffect(CHARACTER* c, OBJECT* o, BMD* b);
	bool	Render_Kanturu2nd_MonsterObjectMesh(OBJECT* o, BMD* b,int ExtraMon);
	bool	Render_Kanturu2nd_MonsterVisual(CHARACTER* c, OBJECT* o, BMD* b);

	bool		Is_Kanturu2nd();
	bool		Is_Kanturu2nd_3rd();

	void	Sound_Kanturu2nd_Object(OBJECT* o);
	void	PlayBGM();
};

class CTrapCanon
{
public:
	CTrapCanon();
	~CTrapCanon();

private:
	void Initialize();
	void Destroy();

public:
	void Open_TrapCanon();
	CHARACTER* Create_TrapCanon(int iPosX, int iPosY, int iKey);
	void Render_Object(OBJECT* o, BMD* b);
	void Render_Object_Visual(CHARACTER* c, OBJECT* o, BMD* b);
	void Render_AttackEffect(CHARACTER* c, OBJECT* o, BMD* b);
};

extern CTrapCanon g_TrapCanon;

#endif
