#pragma once

#include "User.h"

class CAUTOHP
{
public:
	void Load(char* path);
	void AutoHp(LPOBJ lpObj);
	bool CommandHp(LPOBJ lpObj);
	void MainProc();
private:

};

extern CAUTOHP gAUTOHP;