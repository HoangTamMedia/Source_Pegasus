#include "stdafx.h"
#include "GameMain.h"
#include "BattleSoccerManager.h"
#include "CommandManager.h"
#include "DSProtocol.h"
#include "ESProtocol.h"
#include "HackCheck.h"
#include "JSProtocol.h"
#include "MasterSkillTree.h"
#include "MonsterAI.h"
#include "MonsterManager.h"
#include "ObjectManager.h"
#include "Protect.h"
#include "QueueTimer.h"
#include "ServerInfo.h"
#include "SocketManagerUdp.h"
#include "User.h"
#include "Util.h"
#include "CustomAttack.h"
#include "OfflineMode.h"
#include "FakeOnline.h"
CConnection gJoinServerConnection;
CConnection gDataServerConnection;

void GameMainInit(HWND hwnd) // OK
{
	FILE* debugFile = OpenDebugLog();
	if(debugFile != NULL)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		fprintf(debugFile, "[DEBUG] [%04d-%02d-%02d %02d:%02d:%02d.%03d] GameMainInit iniciado\n", 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		fflush(debugFile);
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Criando Mutex: %s\n", gServerInfo.m_ServerMutex);
		fflush(debugFile);
	}
	HANDLE mutexHandle = CreateMutex(0,0,gServerInfo.m_ServerMutex);
	DWORD mutexError = GetLastError();
	if(mutexHandle == 0 || mutexError == ERROR_ALREADY_EXISTS)
	{
		if(debugFile != NULL)
		{
			fprintf(debugFile, "[DEBUG] ERRO: Mutex já existe ou falhou! Error: %d\n", mutexError);
			fclose(debugFile);
		}
		ErrorMessageBox("Could not open GameServer");
		return;
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Mutex criado com sucesso\n");
		fflush(debugFile);
	}

	PROTECT_START

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando gObjInit()...\n");
		fflush(debugFile);
	}
	gObjInit();
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] gObjInit() concluído\n");
		fflush(debugFile);
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando InitHackCheck()...\n");
		fflush(debugFile);
	}
	InitHackCheck();
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] InitHackCheck() concluído\n");
		fflush(debugFile);
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando InitBattleSoccer()...\n");
		fflush(debugFile);
	}
	InitBattleSoccer();
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] InitBattleSoccer() concluído\n");
		fflush(debugFile);
	}

	gGameServerLogOut = 0;

	gGameServerDisconnect = 0;

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando gServerInfo.ReadInit()...\n");
		fflush(debugFile);
	}
	gServerInfo.ReadInit();
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] gServerInfo.ReadInit() concluído\n");
		fflush(debugFile);
	}

	//gCommandManager.Init();

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando gObjSetExperienceTable()...\n");
		fflush(debugFile);
	}
	gObjSetExperienceTable();
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] gObjSetExperienceTable() concluído\n");
		fflush(debugFile);
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando gMonsterManager.SetMonsterData()...\n");
		fflush(debugFile);
	}
	gMonsterManager.SetMonsterData();
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] gMonsterManager.SetMonsterData() concluído\n");
		fflush(debugFile);
	}

	#if(GAMESERVER_UPDATE>=401)

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando gMasterSkillTree.SetMasterLevelExperienceTable()...\n");
		fflush(debugFile);
	}
	gMasterSkillTree.SetMasterLevelExperienceTable();
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] gMasterSkillTree.SetMasterLevelExperienceTable() concluído\n");
		fflush(debugFile);
	}

	#endif

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando gJoinServerConnection.Init()...\n");
		fflush(debugFile);
	}
	gJoinServerConnection.Init(hwnd,JoinServerProtocolCore);
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] gJoinServerConnection.Init() concluído\n");
		fflush(debugFile);
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando gDataServerConnection.Init()...\n");
		fflush(debugFile);
	}
	gDataServerConnection.Init(hwnd,DataServerProtocolCore);
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] gDataServerConnection.Init() concluído\n");
		fflush(debugFile);
	}

	PROTECT_FINAL

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] GameMainInit concluído com sucesso\n");
		fclose(debugFile);
	}
}

void ConnectServerInfoSend() // OK
{
	PROTECT_START

	SDHP_GAME_SERVER_LIVE_SEND pMsg;

	pMsg.header.set(0x01,sizeof(pMsg));

	pMsg.ServerCode = (WORD)gServerInfo.m_ServerCode;
if( Conectar == 1 )
{
	pMsg.UserTotal = (BYTE)((gObjTotalUser==0)?0:((gObjTotalUser*100) / 100));
}
else
{
	pMsg.UserTotal = (BYTE)((gObjTotalUser==0)?0:((gObjTotalUser*100)/ gServerInfo.m_ServerMaxUserNumber));
}

	pMsg.UserCount = (WORD)gObjTotalUser;

	pMsg.AccountCount = 0;

	pMsg.PCPointCount = 0;
if( Conectar == 1 )
{
	pMsg.MaxUserCount = (WORD) 100; //gServerInfo.m_ServerMaxUserNumber;
}
else
{
	pMsg.MaxUserCount = (WORD) gServerInfo.m_ServerMaxUserNumber;
}
	gSocketManagerUdp.DataSend((BYTE*)&pMsg,pMsg.header.size);

	PROTECT_FINAL
}

bool JoinServerConnect(DWORD wMsg) // OK
{
	if(gJoinServerConnection.Connect(gServerInfo.m_JoinServerAddress,(WORD)gServerInfo.m_JoinServerPort,wMsg) == 0)
	{
		return 0;
	}

	GJServerInfoSend();
	return 1;
}

bool DataServerConnect(DWORD wMsg) // OK
{
	if(gDataServerConnection.Connect(gServerInfo.m_DataServerAddress,(WORD)gServerInfo.m_DataServerPort,wMsg) == 0)
	{
		return 0;
	}

	GDServerInfoSend();
	return 1;
}

bool JoinServerReconnect(HWND hwnd,DWORD wMsg) // OK
{
	if(gJoinServerConnection.CheckState() == 0)
	{
		gJoinServerConnection.Init(hwnd,JoinServerProtocolCore);
		return JoinServerConnect(wMsg);
	}

	return 1;
}

bool DataServerReconnect(HWND hwnd,DWORD wMsg) // OK
{
	if(gDataServerConnection.CheckState() == 0)
	{
		gDataServerConnection.Init(hwnd,DataServerProtocolCore);
		return DataServerConnect(wMsg);
	}

	return 1;
}

void JoinServerMsgProc(WPARAM wParam,LPARAM lParam) // OK
{
	switch(LOWORD(lParam))
	{
		case FD_READ:
			gJoinServerConnection.DataRecv();
			break;
		case FD_WRITE:
			gJoinServerConnection.DataSendEx();
			break;
		case FD_CLOSE:
			gJoinServerConnection.Disconnect();
			gObjAllDisconnect();
			break;
	}
}

void DataServerMsgProc(WPARAM wParam,LPARAM lParam) // OK
{
	switch(LOWORD(lParam))
	{
		case FD_READ:
			gDataServerConnection.DataRecv();
			break;
		case FD_WRITE:
			gDataServerConnection.DataSendEx();
			break;
		case FD_CLOSE:
			gDataServerConnection.Disconnect();
			gObjAllDisconnect();
			break;
	}
}

void CALLBACK QueueTimerCallback(PVOID lpParameter,BOOLEAN TimerOrWaitFired) // OK
{
	PROTECT_START

	static CCriticalSection critical;

	critical.lock();

	switch(((int)lpParameter))
	{
		case QUEUE_TIMER_MONSTER:
			gObjectManager.ObjectMonsterAndMsgProc();
			break;
		case QUEUE_TIMER_MONSTER_MOVE:
			gObjectManager.ObjectMoveProc();
			break;
		case QUEUE_TIMER_MONSTER_AI:
			CMonsterAI::MonsterAIProc();
			break;
		case QUEUE_TIMER_MONSTER_AI_MOVE:
			CMonsterAI::MonsterMoveProc();
			break;
		case QUEUE_TIMER_EVENT:
			gObjEventRunProc();
			break;
		case QUEUE_TIMER_VIEWPORT:
			gObjViewportProc();
#if(OFFLINE_MODE==TRUE)
			for (int n = OBJ_STARTUSERINDEX; n < MAX_OBJECT; n++)
			{
				if (gObj[n].m_OfflineMode == 1)
				{
					g_OfflineMode.PickUP(n);
					g_OfflineMode.regresar(n);

				}
				// Bổ sung thêm gọi hàm FakeOnline
				s_FakeOnline.NhatItem(n);
				s_FakeOnline.QuayLaiToaDoGoc(n);
			}
#endif
			break;

		case QUEUE_TIMER_FIRST:
			gObjFirstProc();
			break;
		case QUEUE_TIMER_CLOSE:
			gObjCloseProc();
			break;
		case QUEUE_TIMER_MATH_AUTHENTICATOR:
			gObjMathAuthenticatorProc();
			break;
		case QUEUE_TIMER_ACCOUNT_LEVEL:
			gObjAccountLevelProc();
			break;
		case QUEUE_TIMER_PICK_COMMAND:
			gObjPickProc();
			break;
	}

	critical.unlock();

	PROTECT_FINAL
}
