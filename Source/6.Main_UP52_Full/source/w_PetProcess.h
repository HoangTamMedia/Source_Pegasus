
#pragma once

#include "w_BasePet.h"
#include "w_PetAction.h"

#define PC4_ELF			1
#define PC4_TEST		2
#define PC4_SATAN		3
#define XMAS_RUDOLPH	4
#ifdef PJH_ADD_PANDA_PET
#define PANDA			5
#endif
#define	UNICORN			6
#define SKELETON		7

BoostSmartPointer(PetInfo);
class PetInfo
{
public:
	static PetInfoPtr Make();
	virtual ~PetInfo();

private:
	PetInfo();
	void Destroy();

public:
	float GetScale() { return m_scale; }
	void SetScale( float scale = 0.0f ) { m_scale = scale; }

	int GetBlendMesh() { return m_blendMesh; }
	void SetBlendMesh( int blendMesh = -1 ) { m_blendMesh = blendMesh; }

	float* GetSpeeds() { return m_speeds; }
	int* GetActions() { return m_actions; }
	void SetActions(int count, int *actions, float *speeds);

	int GetActionsCount() { return m_count; }

private:

	int m_blendMesh;
	float m_scale;

	int *m_actions;
	float *m_speeds;

	int m_count;
};

BoostSmartPointer(PetProcess);

class PetProcess
{
public:
	typedef std::list< BoostSmart_Ptr(PetObject) > PetList;
	typedef std::map< int, BoostSmart_Ptr(PetAction) > ActionMap;
	typedef std::map< int, BoostSmart_Ptr(PetInfo) > InfoMap;

public:
	static PetProcessPtr Make();
	virtual ~PetProcess();

private:
	void Register( BoostSmart_Ptr( PetObject ) pPet );
	void UnRegister( CHARACTER *Owner, int itemType, bool isUnregistAll = false );

	BoostWeak_Ptr(PetAction) Find( int key );

	void Init();
	void Destroy();
	PetProcess();

public:
	bool LoadData();
	bool IsPet( int itemType );
	bool CreatePet( int itemType, int modelType, vec3_t Position, CHARACTER *Owner, int SubType=0, int LinkBone=0 );
	void DeletePet( CHARACTER *Owner, int itemType = -1, bool allDelete = false );
	void UpdatePets();
	void RenderPets();
	void SetCommandPet( CHARACTER *Owner, int targetKey, PetObject::ActionType cmdType );

private:
	PetList m_petsList;
	ActionMap m_petsAction;
	InfoMap m_petsInfo;
};

extern PetProcessPtr g_petProcess;
extern PetProcess& ThePetProcess();
