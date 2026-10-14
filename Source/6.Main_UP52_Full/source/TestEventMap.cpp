
#include "stdafx.h"
#include "TestEventMap.h"

#ifdef PSW_ADD_TESTMAP

TestEventMapPtr	TestEventMap::Make()
{
	TestEventMapPtr map( new TestEventMap );
	return map;
}

TestEventMap::TestEventMap() : m_IsTest( true )
{

}

TestEventMap::~TestEventMap()
{

}

bool TestEventMap::CreateObject(OBJECT* o)
{
	return false;
}

bool TestEventMap::MoveObject(OBJECT* o)
{
	return false;
}

bool TestEventMap::RenderObjectVisual(OBJECT* o, BMD* b)
{
	return false;
}

bool TestEventMap::RenderObjectMesh(OBJECT* o, BMD* b, bool ExtraMon)
{
	return false;
}

void TestEventMap::RenderAfterObjectMesh(OBJECT* o, BMD* b, bool ExtraMon)
{

}

CHARACTER* TestEventMap::CreateMonster(int iType, int PosX, int PosY, int Key)
{
	CHARACTER* pCharacter = NULL;
	return pCharacter;
}

bool TestEventMap::MoveMonsterVisual(OBJECT* o, BMD* b)
{
	return false;
}

void TestEventMap::MoveBlurEffect(CHARACTER* c, OBJECT* o, BMD* b)
{

}

bool TestEventMap::RenderMonsterVisual(CHARACTER* c, OBJECT* o, BMD* b)
{
	return false;
}

bool TestEventMap::AttackEffectMonster(CHARACTER* c, OBJECT* o, BMD* b)
{
	return false;
}

bool TestEventMap::SetCurrentActionMonster(CHARACTER* c, OBJECT* o)
{
	return false;
}

bool TestEventMap::PlayMonsterSound(OBJECT* o)
{
	return false;
}

void TestEventMap::PlayObjectSound(OBJECT* o)
{

}

#endif