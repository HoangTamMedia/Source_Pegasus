#include "stdafx.h"
#include "Resource.h"
#include "BloodCastle.h"
#include "CastleDeep.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "Crywolf.h"
#include "CustomArena.h"
#include "CustomEventDrop.h"
#include "CustomOnlineLottery.h"
#include "CustomQuiz.h"
#include "DevilSquare.h"
#include "EventTvT.h"
#include "GameServer.h"
#include "GameMain.h"
#include "IllusionTemple.h"
#include "InvasionManager.h"
#include "JSProtocol.h"
#include "Message.h"
#include "MiniDump.h"
#include "Notice.h"
#include "Protect.h"
#include "QueueTimer.h"
#include "ServerDisplayer.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "SocketManagerUdp.h"
#include "ThemidaSDK.h"
#include "Util.h"
#include "ReiDoMU.h"
#include "IpManager.h"
#include "Log.h"
#include "CustomAttack.h"
#include "CustomStore.h"
#include "OfflineMode.h"
#include "FakeOnline.h"
#include "Path.h"
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HINSTANCE hInst;
HWND hWnd;
int Conectar = 0;


int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) // OK
{
	// DEBUG: Log de início
	FILE* debugFile = OpenDebugLog();
	if(debugFile != NULL)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		fprintf(debugFile, "[DEBUG] [%04d-%02d-%02d %02d:%02d:%02d.%03d] WinMain iniciado\n", 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		fflush(debugFile);
	}

	/*if(gProtect.ReadMainFile("..\\Data\\Hack\\keyword.enc") == 0)
	{
		MessageBox(0,"Licencia not found or invalid!","Error",MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}*/

	VM_START

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] VM_START concluído\n");
		fflush(debugFile);
	}

	CMiniDump::Start();

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] CMiniDump::Start() concluído\n");
		fflush(debugFile);
	}

	LoadString(hInstance,IDS_APP_TITLE,szTitle,MAX_LOADSTRING);
	LoadString(hInstance,IDC_GAMESERVER,szWindowClass,MAX_LOADSTRING);

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] LoadString concluído\n");
		fflush(debugFile);
	}

	ATOM regResult = MyRegisterClass(hInstance);
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] MyRegisterClass retornou: %d\n", regResult);
		fflush(debugFile);
	}

	if(InitInstance(hInstance,nCmdShow) == 0)
	{
		if(debugFile != NULL)
		{
			fprintf(debugFile, "[DEBUG] ERRO: InitInstance retornou 0 - Saindo\n");
			fclose(debugFile);
		}
		return 0;
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] InitInstance concluído com sucesso\n");
		fflush(debugFile);
	}

	SetLargeRand();

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] SetLargeRand concluído\n");
		fflush(debugFile);
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando ReadStartupInfo...\n");
		fflush(debugFile);
	}
	gServerInfo.ReadStartupInfo("GameServerInfo",".\\Data\\GameServerInfo - Common.dat");
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] ReadStartupInfo concluído\n");
		fflush(debugFile);
	}

	#if(PROTECT_STATE==1)
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando StartAuth...\n");
		fflush(debugFile);
	}

	#if(GAMESERVER_UPDATE>=801)
		gProtect.StartAuth(AUTH_SERVER_TYPE_S8_GAME_SERVER);
	#elif(GAMESERVER_UPDATE>=601)
		//gProtect.StartAuth(AUTH_SERVER_TYPE_S6_GAME_SERVER);
	#elif(GAMESERVER_UPDATE>=401)
		gProtect.StartAuth(AUTH_SERVER_TYPE_S4_GAME_SERVER);
	#else
		gProtect.StartAuth(AUTH_SERVER_TYPE_S2_GAME_SERVER);
	#endif

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] StartAuth concluído\n");
		fflush(debugFile);
	}

	#endif

	char buff[256];

	wsprintf(buff,"[%s] %s (ON: %d) %s",GAMESERVER_VERSION,gServerInfo.m_ServerName, gObjTotalUser,GAMESERVER_CLIENT);

	SetWindowText(hWnd,buff);

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] SetWindowText concluído\n");
		fflush(debugFile);
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando gServerDisplayer.Init...\n");
		fflush(debugFile);
	}
	gServerDisplayer.Init(hWnd);
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] gServerDisplayer.Init concluído\n");
		fflush(debugFile);
	}

	WSADATA wsa;

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando WSAStartup...\n");
		fflush(debugFile);
	}
	int wsaResult = WSAStartup(MAKEWORD(2,2),&wsa);
	if(wsaResult == 0)
	{
		if(debugFile != NULL)
		{
			fprintf(debugFile, "[DEBUG] WSAStartup OK, iniciando gSocketManager.Start na porta %d...\n", gServerInfo.m_ServerPort);
			fflush(debugFile);
		}
		int socketStartResult = gSocketManager.Start((WORD)gServerInfo.m_ServerPort);
		if(socketStartResult == 0)
		{
			LogAdd(LOG_RED,"Could not start GameServer");
			if(debugFile != NULL)
			{
				fprintf(debugFile, "[DEBUG] ERRO: gSocketManager.Start falhou!\n");
				fflush(debugFile);
			}
		}
		else
		{
			if(debugFile != NULL)
			{
				fprintf(debugFile, "[DEBUG] gSocketManager.Start OK, iniciando GameMainInit...\n");
				fflush(debugFile);
			}
			GameMainInit(hWnd);
			if(debugFile != NULL)
			{
				fprintf(debugFile, "[DEBUG] GameMainInit concluído\n");
				fflush(debugFile);
			}

			if(debugFile != NULL)
			{
				fprintf(debugFile, "[DEBUG] Conectando JoinServer...\n");
				fflush(debugFile);
			}
			JoinServerConnect(WM_JOIN_SERVER_MSG_PROC);
			if(debugFile != NULL)
			{
				fprintf(debugFile, "[DEBUG] JoinServerConnect concluído\n");
				fflush(debugFile);
			}

			if(debugFile != NULL)
			{
				fprintf(debugFile, "[DEBUG] Conectando DataServer...\n");
				fflush(debugFile);
			}
			DataServerConnect(WM_DATA_SERVER_MSG_PROC);
			if(debugFile != NULL)
			{
				fprintf(debugFile, "[DEBUG] DataServerConnect concluído\n");
				fflush(debugFile);
			}

			if(debugFile != NULL)
			{
				fprintf(debugFile, "[DEBUG] Conectando UDP Socket...\n");
				fflush(debugFile);
			}
			gSocketManagerUdp.Connect(gServerInfo.m_ConnectServerAddress,(WORD)gServerInfo.m_ConnectServerPort);
			if(debugFile != NULL)
			{
				fprintf(debugFile, "[DEBUG] UDP Socket conectado\n");
				fflush(debugFile);
			}

			if(debugFile != NULL)
			{
				fprintf(debugFile, "[DEBUG] Configurando timers...\n");
				fflush(debugFile);
			}
			SetTimer(hWnd,WM_TIMER_1000,1000,0);

			SetTimer(hWnd,WM_TIMER_10000,10000,0);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_MOVE,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_AI,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_AI_MOVE,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_EVENT,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_VIEWPORT,1000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_FIRST,1000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_CLOSE,1000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MATH_AUTHENTICATOR,10000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_ACCOUNT_LEVEL,60000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_PICK_COMMAND,6000,&QueueTimerCallback);
			if(debugFile != NULL)
			{
				fprintf(debugFile, "[DEBUG] Timers configurados\n");
				fflush(debugFile);
			}
		}
	}
	else
	{
		LogAdd(LOG_RED,"WSAStartup() failed with error: %d",WSAGetLastError());
		if(debugFile != NULL)
		{
			fprintf(debugFile, "[DEBUG] ERRO: WSAStartup falhou com erro: %d\n", WSAGetLastError());
			fflush(debugFile);
		}
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] PaintAllInfo e PaintName...\n");
		fflush(debugFile);
	}
	gServerDisplayer.PaintAllInfo();

	gServerDisplayer.PaintName();

	SetTimer(hWnd,WM_TIMER_2000,2000,0);

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando loop de mensagens...\n");
		fflush(debugFile);
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance,(LPCTSTR)IDC_GAMESERVER);

	MSG msg;

	// Tratamento de exceções no loop principal
	__try
	{
		int messageCount = 0;
		while(GetMessage(&msg,0,0,0) != 0)
		{
			messageCount++;
			if(debugFile != NULL && (msg.message == WM_QUIT || messageCount % 1000 == 0))
			{
				if(msg.message == WM_QUIT)
				{
					fprintf(debugFile, "[DEBUG] WM_QUIT recebido! Mensagem: 0x%04X, wParam: %d, lParam: %d. Saindo do loop...\n", 
						msg.message, msg.wParam, msg.lParam);
					fflush(debugFile);
				}
				else if(messageCount % 1000 == 0)
				{
					fprintf(debugFile, "[DEBUG] Loop de mensagens rodando... %d mensagens processadas\n", messageCount);
					fflush(debugFile);
				}
			}
			if(TranslateAccelerator(msg.hwnd,hAccelTable,&msg) == 0)
			{
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}
		}
		if(debugFile != NULL)
		{
			fprintf(debugFile, "[DEBUG] Loop de mensagens finalizado. GetMessage retornou 0. msg.message: 0x%04X\n", msg.message);
			fflush(debugFile);
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		if(debugFile != NULL)
		{
			DWORD exceptionCode = GetExceptionCode();
			fprintf(debugFile, "[DEBUG] ERRO CRÍTICO: Exceção capturada no loop principal! Código: 0x%08X\n", exceptionCode);
			fflush(debugFile);
		}
		LogAdd(LOG_RED, "[DEBUG] Exceção crítica capturada no loop principal! Código: 0x%08X", GetExceptionCode());
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Loop de mensagens finalizado. Limpando...\n");
		fflush(debugFile);
	}

	CMiniDump::Clean();

	VM_END

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] WinMain finalizando. Retorno: %d\n", msg.wParam);
		fclose(debugFile);
	}

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) // OK
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance,(LPCTSTR)IDI_GAMESERVER);
	wcex.hCursor = LoadCursor(0,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = (LPCSTR)IDC_GAMESERVER;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance,(LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance,int nCmdShow) // OK
{
	FILE* debugFile = fopen("GameServer_Debug.log", "a");
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] InitInstance iniciado\n");
		fflush(debugFile);
	}

	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass,szTitle,WS_OVERLAPPEDWINDOW | WS_THICKFRAME,CW_USEDEFAULT,0,980,750,0,0,hInstance,0);

	if(hWnd == 0)
	{
		if(debugFile != NULL)
		{
			DWORD error = GetLastError();
			fprintf(debugFile, "[DEBUG] ERRO: CreateWindow falhou! Error: %d\n", error);
			fclose(debugFile);
		}
		return 0;
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] CreateWindow OK, hWnd: %p\n", hWnd);
		fflush(debugFile);
	}

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] InitInstance concluído com sucesso\n");
		fclose(debugFile);
	}
	return 1;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	// DEBUG: Log de mensagens críticas
	if(message == WM_CLOSE || message == WM_DESTROY || message == WM_QUIT)
	{
		FILE* debugFile = OpenDebugLog();
		if(debugFile != NULL)
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
			fprintf(debugFile, "[DEBUG] [%04d-%02d-%02d %02d:%02d:%02d.%03d] WndProc recebeu mensagem: ", 
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
			if(message == WM_CLOSE) fprintf(debugFile, "WM_CLOSE");
			else if(message == WM_DESTROY) fprintf(debugFile, "WM_DESTROY");
			else if(message == WM_QUIT) fprintf(debugFile, "WM_QUIT");
			fprintf(debugFile, " (wParam: %d, lParam: %d)\n", wParam, lParam);
			fflush(debugFile);
			fclose(debugFile);
		}
	}

	__try
	{
		const char ClassName[] = "MainWindowClass";

		HWND hWndStatusBar;

		switch(message)
	{

		case WM_CREATE:

		{

            hWndStatusBar = CreateWindowEx(

            0,

            STATUSCLASSNAME,

            NULL,

            WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|CCS_BOTTOM,

            0,

            0,

            0,

            0,

            hWnd,

            (HMENU)IDC_STATUSBAR,

            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),

            NULL);

            int iStatusWidths[] = {190,270,360,450,580, -1};

            char text[256];

            SendMessage(hWndStatusBar, SB_SETPARTS, 6, (LPARAM)iStatusWidths);

			wsprintf(text, "Gameserver %s - Update %d ", GAMESERVER_NAME, UPDATE_GAMING);

            SendMessage(hWndStatusBar, SB_SETTEXT, 0,(LPARAM)text);

			wsprintf(text, "OffStore: %d", gObjOffStore);

            SendMessage(hWndStatusBar, SB_SETTEXT, 1,(LPARAM)text);

			wsprintf(text, "OffAttack: %d", gObjOffAttack);

            SendMessage(hWndStatusBar, SB_SETTEXT, 2,(LPARAM)text);

			wsprintf(text, "Bots Buffer: %d", gObjTotalBot);

            SendMessage(hWndStatusBar, SB_SETTEXT, 3,(LPARAM)text);

			wsprintf(text, "Monsters: %d/%d", gObjTotalMonster,MAX_OBJECT_MONSTER);

            SendMessage(hWndStatusBar, SB_SETTEXT, 4,(LPARAM)text);

			SendMessage(hWndStatusBar, SB_SETTEXT, 5,(LPARAM)NULL);

            ShowWindow(hWndStatusBar, SW_SHOW);

		}
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_ONLINEUSERS:
					DialogBox(hInst,(LPCTSTR)IDD_ONLINEUSER,hWnd,(DLGPROC)UserOnline);
					break;
				case IDM_ABOUT:
					DialogBox(hInst,(LPCTSTR)IDD_ABOUTBOX,hWnd,(DLGPROC)About);
					break;
				case IDM_EXIT:
					if(MessageBox(0,"Are you sure to terminate GameServer?","Ask terminate server",MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						DestroyWindow(hWnd);
					}
					break;
				case IDM_FILE_ALLUSERLOGOUT:
					gObjAllLogOut();
					break;
				case IDM_FILE_ALLUSERDISCONNECT:
					gObjAllDisconnect();
					break;
				case IDM_FILE_1MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 60;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(487));
					}
					break;
				case IDM_FILE_3MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 180;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(488));
					}
					break;
				case IDM_FILE_5MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 300;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(489));
					}
					break;
				case IDM_RELOAD_RELOADCASHSHOP:
					gServerInfo.ReadCashShopInfo();
					break;
				case IDM_RELOAD_RELOADCHAOSMIX:
					gServerInfo.ReadChaosMixInfo();
					break;
				case IDM_RELOAD_RELOADCHARACTER:
					gServerInfo.ReadCharacterInfo();
					break;
				case IDM_RELOAD_RELOADCOMMAND:
					gServerInfo.ReadCommandInfo();
					break;
				case IDM_RELOAD_RELOADCOMMON:
					gServerInfo.ReadCommonInfo();
					break;
				case IDM_RELOAD_RELOADCUSTOM:
					gServerInfo.ReadCustomInfo();
					break;
				case IDM_RELOAD_RELOADEVENT:
					gServerInfo.ReadEventInfo();
					break;
				case IDM_RELOAD_RELOADEVENTITEMBAG:
					gServerInfo.ReadEventItemBagInfo();
					break;
				case IDM_RELOAD_RELOADHACK:
					gServerInfo.ReadHackInfo();
					break;
				case IDM_RELOAD_RELOADITEM:
					gServerInfo.ReadItemInfo();
					break;
				case IDM_RELOAD_RELOADMONSTER:
					gServerInfo.ReloadMonsterInfo();
					break;
				case IDM_RELOAD_RELOADMOVE:
					gServerInfo.ReadMoveInfo();
					break;
				case IDM_RELOAD_RELOADQUEST:
					gServerInfo.ReadQuestInfo();
					break;
				case IDM_RELOAD_RELOADSHOP:
					gServerInfo.ReadShopInfo();
					break;
				case IDM_RELOAD_RELOADSKILL:
					gServerInfo.ReadSkillInfo();
					break;
				case IDM_RELOAD_RELOADUTIL:
					gServerInfo.ReadUtilInfo();
					break;
				case IDM_RELOAD_RELOADBOTS: //MC bots
					gServerInfo.ReloadBotInfo(); //MC bots
					break;
				case IDM_RELOAD_RELOADALL:
					gServerInfo.ReloadAll();
					break;
				case IDM_STARTONLINELOTTERY_LORENCIA:
					gCustomOnlineLottery.StartNow(0);
					break;
				case IDM_STARTONLINELOTTERY_DEVIAS:
					gCustomOnlineLottery.StartNow(2);
					break;
				case IDM_STARTONLINELOTTERY_NORIA:
					gCustomOnlineLottery.StartNow(3);
					break;
				case IDM_STARTONLINELOTTERY_ELBELAND:
					gCustomOnlineLottery.StartNow(51);
					break;
				case IDM_STARTBC:
					gBloodCastle.StartBC();
					break;
				case IDM_STARTDS:
					gDevilSquare.StartDS();
					break;
				case IDM_STARTCC:
					gChaosCastle.StartCC();
					break;
				case IDM_STARTIT:
					gIllusionTemple.StartIT();
					break;
				case IDM_STARTQUIZ:
					gCustomQuiz.StartQuiz();
					break;
				case IDM_STARTDROP:
					gCustomEventDrop.StartDrop();
					break;
				case IDM_STARTKING:
					gReiDoMU.StartKing();
					break;
				case IDM_STARTTVT:
					gTvTEvent.StartTvT();
					break;
				case IDM_INVASION0:
					gInvasionManager.StartInvasion(0);
					break;
				case IDM_INVASION1:
					gInvasionManager.StartInvasion(1);
					break;
				case IDM_INVASION2:
					gInvasionManager.StartInvasion(2);
					break;
				case IDM_INVASION3:
					gInvasionManager.StartInvasion(3);
					break;
				case IDM_INVASION4:
					gInvasionManager.StartInvasion(4);
					break;
				case IDM_INVASION5:
					gInvasionManager.StartInvasion(5);
					break;
				case IDM_INVASION6:
					gInvasionManager.StartInvasion(6);
					break;
				case IDM_INVASION7:
					gInvasionManager.StartInvasion(7);
					break;
				case IDM_INVASION8:
					gInvasionManager.StartInvasion(8);
					break;
				case IDM_INVASION9:
					gInvasionManager.StartInvasion(9);
					break;
				case IDM_INVASION10:
					gInvasionManager.StartInvasion(10);
					break;
				case IDM_INVASION11:
					gInvasionManager.StartInvasion(11);
					break;
				case IDM_INVASION12:
					gInvasionManager.StartInvasion(12);
					break;
				case IDM_INVASION13:
					gInvasionManager.StartInvasion(13);
					break;
				case IDM_CA0:
					gCustomArena.StartCustomArena(0);
					break;
				case IDM_CA1:
					gCustomArena.StartCustomArena(1);
					break;
				case IDM_CA2:
					gCustomArena.StartCustomArena(2);
					break;
				case IDM_CA3:
					gCustomArena.StartCustomArena(3);
					break;
				case IDM_CA4:
					gCustomArena.StartCustomArena(4);
					break;
				case IDM_CA5:
					gCustomArena.StartCustomArena(5);
					break;
				case IDM_CA6:
					gCustomArena.StartCustomArena(6);
					break;
				case IDM_CA7:
					gCustomArena.StartCustomArena(7);
					break;
				case IDM_CA8:
					gCustomArena.StartCustomArena(8);
					break;
				case IDM_CA9:
					gCustomArena.StartCustomArena(9);
					break;
				case IDM_CA10:
					gCustomArena.StartCustomArena(10);
					break;
				case IDM_CA11:
					gCustomArena.StartCustomArena(11);
					break;
				case IDM_CA12:
					gCustomArena.StartCustomArena(12);
					break;
				case IDM_CA13:
					gCustomArena.StartCustomArena(13);
					break;
				case IDM_STARTCS:
					#if(GAMESERVER_TYPE==1)
					gCastleSiege.StartCS();
					#endif
					break;
				case IDM_STARTCW:
					gCrywolf.StartCW();
					break;
				case IDM_STARTLD:
					gCastleDeep.StartLD();
					break;
#if USE_FAKE_ONLINE == TRUE
					//==========FakeOnline	
				case ID_FAKEONLINE_RELOADDATA:

					s_FakeOnline.LoadFakeData(gPath.GetFullPath("Util\\Obj.xml"));

					break;
				case ID_FAKEONLINE_ADDFAKEONLINE:
				{
					if (gJoinServerConnection.CheckState() != 0 && gDataServerConnection.CheckState() != 0)
					{
						s_FakeOnline.RestoreFakeOnline();
					}

				}
				break;
				case ID_FAKEONLINE_DELFAKEONLINE:
				{
					for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
					{
						if (gObjIsConnectedGP(n) != 0 && gObj[n].IsFakeOnline)
						{
							s_FakeOnline.OnAttackAlreadyConnected(&gObj[n]);
							//LogAdd(LOG_TEST, "[ FakeOnline ] Disconnect Account");
						}
					}
				}
				break;
#endif
				default:
					return DefWindowProc(hWnd,message,wParam,lParam);
			}
			break;
		case WM_CLOSE:
		{
			FILE* debugFile = OpenDebugLog();
			if(debugFile != NULL)
			{
				SYSTEMTIME st;
				GetLocalTime(&st);
				fprintf(debugFile, "[DEBUG] [%04d-%02d-%02d %02d:%02d:%02d.%03d] WM_CLOSE recebido\n", 
					st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
				fflush(debugFile);
			}
			if (MessageBox(0, "Close GameServer?", "GameServer", MB_OKCANCEL) == IDOK)
			{
				if(debugFile != NULL)
				{
					fprintf(debugFile, "[DEBUG] Usuário confirmou fechamento. Desconectando usuários...\n");
					fflush(debugFile);
				}
				gObjAllLogOut();
				gObjAllDisconnect();
				if(debugFile != NULL)
				{
					fprintf(debugFile, "[DEBUG] Chamando DestroyWindow...\n");
					fflush(debugFile);
					fclose(debugFile);
				}
				DestroyWindow(hWnd);
			}
			else
			{
				if(debugFile != NULL)
				{
					fprintf(debugFile, "[DEBUG] Usuário cancelou fechamento\n");
					fclose(debugFile);
				}
			}
			break;
		}
		case WM_TIMER:
			switch(wParam)
			{
				case WM_TIMER_1000:
					GJServerUserInfoSend();
					ConnectServerInfoSend();
					break;
				case WM_TIMER_2000:
					gObjCountProc();
					gServerDisplayer.Run();
					break;
				case WM_TIMER_10000:
					JoinServerReconnect(hWnd,WM_JOIN_SERVER_MSG_PROC);
					DataServerReconnect(hWnd,WM_DATA_SERVER_MSG_PROC);
					break;
			}
			break;
		case WM_JOIN_SERVER_MSG_PROC:
			JoinServerMsgProc(wParam,lParam);
			break;
		case WM_DATA_SERVER_MSG_PROC:
			DataServerMsgProc(wParam,lParam);
			break;
		case WM_DESTROY:
		{
			FILE* debugFile = OpenDebugLog();
			if(debugFile != NULL)
			{
				SYSTEMTIME st;
				GetLocalTime(&st);
				fprintf(debugFile, "[DEBUG] [%04d-%02d-%02d %02d:%02d:%02d.%03d] WM_DESTROY recebido. Chamando PostQuitMessage(0)\n", 
					st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
				fflush(debugFile);
				fclose(debugFile);
			}
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hWnd,message,wParam,lParam);
		}

		return 0;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		FILE* debugFile = OpenDebugLog();
		if(debugFile != NULL)
		{
			DWORD exceptionCode = GetExceptionCode();
			fprintf(debugFile, "[DEBUG] ERRO CRÍTICO: Exceção em WndProc! Mensagem: 0x%04X, Código: 0x%08X\n", message, exceptionCode);
			fflush(debugFile);
			fclose(debugFile);
		}
		LogAdd(LOG_RED, "[DEBUG] Exceção crítica em WndProc! Mensagem: 0x%04X, Código: 0x%08X", message, GetExceptionCode());
		return DefWindowProc(hWnd,message,wParam,lParam);
	}
}

LRESULT CALLBACK About(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_INITDIALOG:
			return 1;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg,LOWORD(wParam));
				return 1;
			}
			break;
	}

	return 0;
}

LRESULT CALLBACK UserOnline(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	
	switch(message)
	{
		case WM_INITDIALOG:
					

			hWndComboBox = GetDlgItem(hDlg, IDC_LIST1);

            if( !hWndComboBox )
            {
                MessageBox(hDlg,
                           "Could not create the combo box",
                           "Failed Control Creation",
                           MB_OK);
                return FALSE;
            }


			for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
			{
				if(gObj[n].Connected >= OBJECT_LOGGED && gObj[n].Type == OBJECT_USER)
				{
					    char fulltext[30]; 
						wsprintf(fulltext,"%s (%s)",gObj[n].Account,gObj[n].Name);

						int pos = SendMessage(hWndComboBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCTSTR)fulltext));
						SendMessage(hWndComboBox, LB_SETITEMDATA, pos, (LPARAM) gObj[n].Account);
				}
			}

			return 1;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg,LOWORD(wParam));
				return 1;
			}

			switch(LOWORD(wParam))
			{
			case IDC_BUTTONDC:

					int itemIndex = (int) SendMessage(hWndComboBox, LB_GETCURSEL, (WPARAM)0, (LPARAM) 0);
					
				    if (itemIndex == LB_ERR)
					{
						return 0;
					}

					// Getdata
					char* s = (char*)SendMessage(hWndComboBox, LB_GETITEMDATA, itemIndex, 0);

					for(int n = OBJECT_START_USER; n < MAX_OBJECT ; n++)
					{
						if(gObj[n].Connected >= OBJECT_LOGGED && strcmp(gObj[n].Account,s) == 0 )
						{
							LPOBJ lpObj = &gObj[n];

							gObjUserKill(lpObj->Index);

							gCustomAttack.OnAttackAlreadyConnected(lpObj);

							gCustomStore.OnPShopAlreadyConnected(lpObj);

							g_OfflineMode.OnHelperpAlreadyConnected(lpObj);

							CloseClient(lpObj->Index);

							MessageBox(hDlg, "Account disconnect sucefully", "Confirm", MB_OK);
							break;
						}
					}

				return 1;
			}
			break;
	}

	return 0;
}
