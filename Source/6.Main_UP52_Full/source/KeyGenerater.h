
#pragma once

CONST int MAX_KEY_GENERATER_FILTER	= 10;

class CKeyGenerater
{
public:
	CKeyGenerater();
	virtual ~CKeyGenerater();

#ifdef USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE
	DWORD		GenerateSeedValue();
#endif
	DWORD		GenerateKeyValue(DWORD dwKeyValue);
	bool		CheckKeyValue(DWORD* dwOldKeyValue, DWORD dwReceiveKeyValue);

#ifdef USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE
	void		_SimulationKeyGenerate();
#endif
};

extern CKeyGenerater g_KeyGenerater;
