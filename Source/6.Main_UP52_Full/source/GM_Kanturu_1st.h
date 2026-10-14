
#ifndef _GM_KANTURU_1ST_H_
#define _GM_KANTURU_1ST_H_

namespace M37Kanturu1st
{
	bool IsKanturu1st();

	bool CreateKanturu1stObject(OBJECT* pObject);
	bool MoveKanturu1stObject(OBJECT* pObject);
	bool RenderKanturu1stObjectVisual(OBJECT* pObject, BMD* pModel);
	bool RenderKanturu1stObjectMesh(OBJECT* o, BMD* b, bool ExtraMon = 0);
	void RenderKanturu1stAfterObjectMesh(OBJECT* o, BMD* b);

	CHARACTER* CreateKanturu1stMonster(int iType, int PosX, int PosY, int Key);
	bool SetCurrentActionKanturu1stMonster(CHARACTER* c, OBJECT* o);
	bool AttackEffectKanturu1stMonster(CHARACTER* c, OBJECT* o, BMD* b);
	bool MoveKanturu1stMonsterVisual(CHARACTER* c,OBJECT* o, BMD* b);
	void MoveKanturu1stBlurEffect(CHARACTER* c, OBJECT* o, BMD* b);
	bool RenderKanturu1stMonsterObjectMesh(OBJECT* o, BMD* b,int ExtraMon);
	bool RenderKanturu1stMonsterVisual(CHARACTER* c, OBJECT* o, BMD* b);
};

#endif