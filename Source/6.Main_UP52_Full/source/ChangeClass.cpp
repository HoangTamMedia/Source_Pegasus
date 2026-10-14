#include "stdafx.h"
#include "ChangeClass.h"
#include "CBInterface.h"
#include <NewUISystem.h>

cChangeClass gChangeClass;

cChangeClass::cChangeClass()
{
	this->m_Price = 0;
}

cChangeClass::~cChangeClass()
{

}

void cChangeClass::RecvData(PMSG_CHANGECLASS_DATA* Data)
{
	this->m_PriceType = Data->PriceType;
	this->m_Price = Data->Price;
	gInterface.Data[eCHANGINGCLASS_MAIN].OnShow ^= 1;
}

void cChangeClass::SendChangeClass(int Type)
{
	gInterface.Data[eCHANGINGCLASS_MAIN].Close();

	XULY_CGPACKET pMsg;
	pMsg.header.set(0xFD,0x05, sizeof(pMsg));
	pMsg.ThaoTac = Type;
	DataSend((LPBYTE)&pMsg, pMsg.header.size);
}

void cChangeClass::drawNewInterfaceClassChange()
{
	if (!gInterface.Data[eCHANGINGCLASS_MAIN].OnShow)
	{
		return;
	}

	// Was 540; the two button columns end around 250, leaving the right half of
	// the frame empty.
	float MainWidth = 360;
	float MainHeight = 245;
	float StartY = ((DisplayHeight - 51) / 2) - (MainHeight / 2) + 1.0f;
	float StartX = (MAX_WIN_WIDTH / 2) - (MainWidth / 2) + 151.0f;

	g_pBCustomMenuInfo->gDrawWindowCustom(&StartX, &StartY, MainWidth, MainHeight, eCHANGINGCLASS_MAIN, "");

	float TextX = StartX + 15;
	float TextY = StartY + 25;
	int CountText = 0;

	if (gChangeClass.m_PriceType == 0)
	{

	}
	else if (gChangeClass.m_PriceType == 1)
	{

	}
	else if (gChangeClass.m_PriceType == 2)
	{

	}
	else if (gChangeClass.m_PriceType == 3)
	{

	}
	else if (gChangeClass.m_PriceType == 4)
	{

	}
	EnableAlphaTest();

	glColor3f(1.0, 1.0, 1.0);
	gInterface.DrawFormat(eYellow, StartX + 30, StartY + 12, 100, 1, "Mudar Classe [F4]");

	gInterface.DrawFormat(eWhite, TextX, TextY + 10, 200, 1, "Requisitos para Mudar de Classe - Por Favor Leia com Aten��o");

	gInterface.DrawFormat(eRed, TextX, TextY + 20, 200, 1, "Pegasus Coin : %d", this->m_Price);

	gInterface.DrawFormat(eWhite, TextX, TextY + 30, 200, 1, "Por Favor Remova Todo o Equipamento ao Mudar de Classe");

	gInterface.DrawFormat(eRed, TextX, TextY + 40, 200, 1, "Pontos Master Ser�o Restaurados Ap�s Mudar de Classe");

	gInterface.DrawFormat(eRed, TextX, TextY + 50, 200, 1, "Deve Completar a Miss�o do Personagem 3");
	DisableAlphaBlend();

	char* buttonName[] = { "Wizard", "Dark Knight", "Elf", "Rage Fight", "Dark Lord", "Summoner", "Magic Gladiator"};
	for (char i = 0; i < 7; i++)
	{

		if (g_pBCustomMenuInfo->DrawButton(StartX + 35 + (i % 2) * (60 / 1.3 + 70), StartY + 100 + i / 2 * (50 / 2.0 + 5), 100, 12, buttonName[i]))
		{

			if(i == 0)
			{
				gChangeClass.SendChangeClass(0);
			}else if(i==1){

				gChangeClass.SendChangeClass(16);
			}else if(i==2){

				gChangeClass.SendChangeClass(32);
			}else if(i==3){

				gChangeClass.SendChangeClass(48);
			}else if(i==4){

				gChangeClass.SendChangeClass(64);
			}else if(i==5){

				gChangeClass.SendChangeClass(80);
			}else if(i==6)
			{

				gChangeClass.SendChangeClass(96);
			}
			gInterface.Data[eCHANGINGCLASS_MAIN].OnShow = false;
		}
	}

}

void cChangeClass::ChangeClass()
{

	if ((GetTickCount() - gInterface.Data[eCHANGINGCLASS_MAIN].EventTick) > 300)
	{
		gInterface.Data[eCHANGINGCLASS_MAIN].EventTick = GetTickCount();

		if (gInterface.Data[eCHANGINGCLASS_MAIN].OnShow)
		{
			gInterface.Data[eCHANGINGCLASS_MAIN].OnShow = 0;
			return;
		}
		XULY_CGPACKET pMsg;
		pMsg.header.set(0xD3, 0x93, sizeof(pMsg));
		pMsg.ThaoTac = 1;
		DataSend((LPBYTE)&pMsg, pMsg.header.size);

	}

}
