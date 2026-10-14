#include "stdafx.h"
#include "Util.h"
#include "GameMain.h"
#include "HackCheck.h"
#include "ItemManager.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "ThemidaSDK.h"
#include "Viewport.h"

std::mt19937 seed;
std::uniform_int_distribution<int> dist;
short RoadPathTable[MAX_ROAD_PATH_TABLE] = {-1,-1,0,-1,1,-1,1,0,1,1,0,1,-1,1,-1,0};

FILE* OpenDebugLog() // OK
{
	// Verifica se o debug está ativo
	// A configuração LogDebug é lida em ReadCustomInfo (dentro de ReadInit)
	// Por padrão, m_DebugLog = 0 (desligado)
	// Se LogDebug=1 no arquivo, m_DebugLog será 1
	
	// Se está ligado (1), cria o arquivo de log
	if(gServerInfo.m_DebugLog == 1)
	{
		return fopen("GameServer_Debug.log", "a");
	}
	
	// Se está desligado (0), não cria o arquivo
	// Nota: Antes de ReadCustomInfo ser chamado, m_DebugLog = 0 por padrão
	// então não vai logar no início. Isso é aceitável pois ReadCustomInfo
	// é chamado logo no início do ReadInit, que acontece cedo na inicialização
	return NULL;
}

int SafeGetItem(int index) // OK
{
	return CHECK_ITEM(index);
}

flt GetRoundValue(float value) // OK
{
	float integral;

	if(modf(value,&integral) > 0.5f)
	{
		return ceil(value);
	}

	return floor(value);
}

BYTE GetNewOptionCount(BYTE NewOption) // OK
{
	BYTE count = 0;

	for(int n=0;n < MAX_EXC_OPTION;n++)
	{
		if((NewOption & (1 << n)) != 0)
		{
			count++;
		}
	}

	return count;
}

BYTE GetSocketOptionCount(BYTE SocketOption[5]) // OK
{
	BYTE count = 0;

	for(int n=0;n < MAX_SOCKET_OPTION;n++)
	{
		if(SocketOption[n] != 0xFF)
		{
			count++;
		}
	}

	return count;
}

BYTE GetPathPacketDirPos(int px,int py) // OK
{
	if(px <= -1 && py <= -1)
	{
		return 0;
	}
	else if(px <= -1 && py == 0)
	{
		return 7;
	}
	else if(px <= -1 && py >= 1)
	{
		return 6;
	}
	else if(px == 0 && py <= -1)
	{
		return 1;
	}
	else if(px == 0 && py >= 1)
	{
		return 5;
	}
	else if(px >= 1 && py <= -1)
	{
		return 2;
	}
	else if(px >= 1 && py == 0)
	{
		return 3;
	}
	else if(px >= 1 && py >= 1)
	{
		return 4;
	}

	return 0;
}

void PacketArgumentDecrypt(char* out_buff,char* in_buff,int size) // OK
{
	BYTE XorTable[3] = {0xFC,0xCF,0xAB};

	for(int n=0;n < size;n++)
	{
		out_buff[n] = in_buff[n]^XorTable[n%3];
	}
}

void ErrorMessageBox(char* message,...) // OK
{
	FILE* debugFile = OpenDebugLog();
	if(debugFile != NULL)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		fprintf(debugFile, "[DEBUG] [%04d-%02d-%02d %02d:%02d:%02d.%03d] ErrorMessageBox CHAMADO - Servidor vai FECHAR!\n", 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		fflush(debugFile);
	}

	VM_START

	char buff[256];

	memset(buff,0,sizeof(buff));

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff, sizeof(buff), message, arg);
	va_end(arg);

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Mensagem de erro: %s\n", buff);
		fprintf(debugFile, "[DEBUG] Chamando MessageBox e depois ExitProcess(0)...\n");
		fflush(debugFile);
		fclose(debugFile);
	}

	MessageBoxA(0, buff, "Error", MB_OK | MB_ICONERROR);

	VM_END

	if(debugFile != NULL)
	{
		debugFile = OpenDebugLog();
		if(debugFile != NULL)
		{
			fprintf(debugFile, "[DEBUG] ExitProcess(0) sendo chamado agora - SERVIDOR FECHANDO!\n");
			fflush(debugFile);
			fclose(debugFile);
		}
	}

	ExitProcess(0);
}

void LogAdd(eLogColor color,char* text,...) // OK
{
	tm today;
	time_t ltime;
	time(&ltime);

	if(localtime_s(&today,&ltime) != 0)
	{
		return;
	}

	char time[32];

	if(asctime_s(time,sizeof(time),&today) != 0)
	{
		return;
	}

	char temp[1024];

	va_list arg;
	va_start(arg,text);
	vsprintf_s(temp,text,arg);
	va_end(arg);

	char log[1024];

	sprintf_s(log, sizeof(log), "%.8s %s", &time[11], temp);

	gServerDisplayer.LogAddText(color,log,strlen(log));
}

void LogAddConnect(eLogColor color,char* text,...) // OK
{
	tm today;
	time_t ltime;
	time(&ltime);

	if(localtime_s(&today,&ltime) != 0)
	{
		return;
	}

	char time[32];

	if(asctime_s(time,sizeof(time),&today) != 0)
	{
		return;
	}

	char temp[1024];

	va_list arg;
	va_start(arg,text);
	vsprintf_s(temp,text,arg);
	va_end(arg);

	char log[1024];

	sprintf_s(log, sizeof(log), "%.8s %s", &time[11], temp);

	gServerDisplayer.LogAddTextConnect(color,log,strlen(log));
}

bool DataSend(int aIndex,BYTE* lpMsg,DWORD size) // OK
{
#if(OFFLINE_MODE == TRUE)
	if (gObj[aIndex].m_OfflineMode == 1)
	{
		return 0;
	}
#endif

	return gSocketManager.DataSend(aIndex,lpMsg,size);
}

void DataSendAll(BYTE* lpMsg,int size) // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) != 0)
		{
			DataSend(n,lpMsg,size);
		}
	}
}

bool DataSendSocket(SOCKET socket,BYTE* lpMsg,DWORD size) // OK
{
	if(socket == INVALID_SOCKET)
	{
		return 0;
	}

	#if(ENCRYPT_STATE==1)

	EncryptData(lpMsg,size);

	#endif

	int count=0,result=0;

	while(size > 0)
	{
		if((result=send(socket,(char*)&lpMsg[count],size,0)) == SOCKET_ERROR)
		{
			if(WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return 0;
			}
		}
		else
		{
			count += result;
			size -= result;
		}
	}

	return 1;
}

void MsgSendV2(LPOBJ lpObj,BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state != VIEWPORT_NONE && lpObj->VpPlayer2[n].type == OBJECT_USER && OBJECT_BOTS)
		{
			DataSend(lpObj->VpPlayer2[n].index,lpMsg,size);
		}
	}
}

void CloseClient(int aIndex) // OK
{
	gSocketManager.Disconnect(aIndex);
}

void PostMessage1(char* name,char* message,char* text) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	sprintf_s(buff, sizeof(buff), message, name, text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {0};

	sprintf_s(buff, sizeof(buff), message, text);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_WHISPER_SEND pMsg;

	pMsg.header.set(0x02,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}

	#endif
}

void PostMessage2(char* name,char* message,char* text) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	sprintf_s(buff, sizeof(buff), message, name, text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {'~'};

	sprintf_s(&buff[1], sizeof(buff) - 1, message, text);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	DataSendAll((BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void PostMessage3(char* name,char* message,char* text) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	sprintf_s(buff, sizeof(buff), message, name, text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {'@'};

	sprintf_s(&buff[1], sizeof(buff) - 1, message, text);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	DataSendAll((BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void PostMessage4(char* name,char* message,char* text) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	sprintf_s(buff, sizeof(buff), message, name, text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {'$'};

	sprintf_s(&buff[1], sizeof(buff) - 1, message, text);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	DataSendAll((BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void PostMessagePK(char* name,char* message,char* text) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	sprintf_s(buff, sizeof(buff), message, name, text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {'~'};

	sprintf_s(&buff[1], sizeof(buff) - 1, message, name, text);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,"[PK]",sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	DataSendAll((BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void PostMessageUserON(char* name,char* message) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	sprintf_s(buff, sizeof(buff), message, name);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {'@'};

	sprintf_s(&buff[1], sizeof(buff) - 1, message, name);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,"[UserON]",sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	DataSendAll((BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void PostMessageNew(char* name,char* message,char* text) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	// Verificar se a mensagem contém os placeholders necessários
	if (message != NULL && strlen(message) > 0)
	{
		// Verificar se contém dois %s (um para name e outro para text)
		const char* first_s = strstr(message, "%s");
		if (first_s != NULL)
		{
			const char* second_s = strstr(first_s + 2, "%s");
			if (second_s != NULL)
			{
				// Tem dois %s, usar o formato da mensagem
				sprintf_s(buff, sizeof(buff), message, name, text);
			}
			else
			{
				// Tem apenas um %s, formatar a mensagem com o nome primeiro, depois adicionar o texto
				char temp[256] = {0};
				sprintf_s(temp, sizeof(temp), message, name);
				sprintf_s(buff, sizeof(buff), "%s: %s", temp, text);
			}
		}
		else
		{
			// Não tem %s, usar formato padrão
			sprintf_s(buff, sizeof(buff), "[POST] %s: %s", name, text);
		}
	}
	else
	{
		// Mensagem vazia ou NULL, usar formato padrão
		sprintf_s(buff, sizeof(buff), "[POST] %s: %s", name, text);
	}

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {0};

	// Verificar se a mensagem contém os placeholders necessários
	if (message != NULL && strlen(message) > 0)
	{
		// Verificar se contém dois %s (um para name e outro para text)
		const char* first_s = strstr(message, "%s");
		if (first_s != NULL)
		{
			const char* second_s = strstr(first_s + 2, "%s");
			if (second_s != NULL)
			{
				// Tem dois %s, usar o formato da mensagem
				sprintf_s(buff, sizeof(buff), message, name, text);
			}
			else
			{
				// Tem apenas um %s, formatar a mensagem com o nome primeiro, depois adicionar o texto
				char temp[256] = {0};
				sprintf_s(temp, sizeof(temp), message, name);
				sprintf_s(buff, sizeof(buff), "%s: %s", temp, text);
			}
		}
		else
		{
			// Não tem %s, usar formato padrão
			sprintf_s(buff, sizeof(buff), "[POST] %s: %s", name, text);
		}
	}
	else
	{
		// Mensagem vazia ou NULL, usar formato padrão
		sprintf_s(buff, sizeof(buff), "[POST] %s: %s", name, text);
	}

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_WHISPER_SEND pMsg;

	pMsg.header.set(0x02,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,"[POST]",sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	DataSendAll((BYTE*)&pMsg,pMsg.header.size);
	#endif
}


void SetLargeRand() // OK
{
	std::random_device rd;
	seed = std::mt19937(rd());
	dist = std::uniform_int_distribution<int>(0,2147483647);
}

long GetLargeRand() // OK
{
	return dist(seed);
}
