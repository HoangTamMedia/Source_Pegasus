
#ifdef PSW_ADD_TESTMAP

#if !defined(AFX_TESTEVENTMAP_H__9ABFF262_D93D_4B3C_9620_681662E7A384__INCLUDED_)
#define AFX_TESTEVENTMAP_H__9ABFF262_D93D_4B3C_9620_681662E7A384__INCLUDED_

#pragma once

#include "w_BaseMap.h"

BoostSmartPointer( TestEventMap );
class TestEventMap : public BaseMap
{
public:
	static TestEventMapPtr	Make();
	virtual ~TestEventMap();

public:

	virtual bool CreateObject(OBJECT* o);

	virtual bool MoveObject(OBJECT* o);

	virtual bool RenderObjectVisual(OBJECT* o, BMD* b);

	virtual bool RenderObjectMesh(OBJECT* o, BMD* b, bool ExtraMon = 0);

	virtual void RenderAfterObjectMesh(OBJECT* o, BMD* b, bool ExtraMon = 0);

public:

	virtual CHARACTER* CreateMonster(int iType, int PosX, int PosY, int Key);

	virtual bool MoveMonsterVisual(OBJECT* o, BMD* b);

	virtual void MoveBlurEffect(CHARACTER* c, OBJECT* o, BMD* b);

	virtual bool RenderMonsterVisual(CHARACTER* c, OBJECT* o, BMD* b);

	virtual bool AttackEffectMonster(CHARACTER* c, OBJECT* o, BMD* b);

	virtual bool SetCurrentActionMonster(CHARACTER* c, OBJECT* o);

public:
	virtual bool PlayMonsterSound(OBJECT* o);
	virtual void PlayObjectSound(OBJECT* o);

public:
	const bool GetTest() const;

public:
	TestEventMap();

private:
	bool m_IsTest;

};

inline
const bool TestEventMap::GetTest() const
{
	return m_IsTest;
}

#endif

#endif
