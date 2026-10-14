#include "stdafx.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Util.h"
#include "Message.h"
#include "User.h"
#include "Path.h"
#include "ServerInfo.h"
#include "MemScript.h"
#include "CustomRanking.h"
#include "Notice.h"

CCustomRanking gCustomRanking;

void CCustomRanking::Load(char* path)
{
	FILE* debugFile = OpenDebugLog();
	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] CCustomRanking::Load iniciado - Path: %s\n", path);
		fflush(debugFile);
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Criando CMemScript...\n");
		fflush(debugFile);
	}
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		if(debugFile != NULL)
		{
			fprintf(debugFile, "[DEBUG] ERRO: Falha ao alocar CMemScript!\n");
			fclose(debugFile);
		}
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] CMemScript criado. Chamando SetBuffer...\n");
		fflush(debugFile);
	}
	if(lpMemScript->SetBuffer(path) == 0)
	{
		if(debugFile != NULL)
		{
			fprintf(debugFile, "[DEBUG] ERRO: SetBuffer falhou! Erro: %s\n", lpMemScript->GetLastError());
			fclose(debugFile);
		}
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] SetBuffer OK. Inicializando arrays...\n");
		fflush(debugFile);
	}
	for(int n=0;n < MAX_RANK;n++)
	{
		this->r_Data[n];
	}

	this->m_count = 0;

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Iniciando loop de leitura do arquivo...\n");
		fflush(debugFile);
	}
	int loopCount = 0;
	int maxLoops = 100000; // Proteção contra loop infinito
	try
	{
		while(true)
		{
			loopCount++;
			if(loopCount > maxLoops)
			{
				if(debugFile != NULL)
				{
					fprintf(debugFile, "[DEBUG] ERRO: Loop infinito detectado! Mais de %d iterações!\n", maxLoops);
					fflush(debugFile);
				}
				break;
			}

			if(debugFile != NULL && loopCount % 1000 == 0)
			{
				fprintf(debugFile, "[DEBUG] Loop: %d iterações...\n", loopCount);
				fflush(debugFile);
			}

			if(lpMemScript->GetToken() == TOKEN_END)
			{
				if(debugFile != NULL)
				{
					fprintf(debugFile, "[DEBUG] TOKEN_END recebido. Saindo do loop externo. Total de iterações: %d\n", loopCount);
					fflush(debugFile);
				}
				break;
			}

			int section = lpMemScript->GetNumber();

			int innerLoopCount = 0;
			int maxInnerLoops = 10000;
			while(true)
			{
				innerLoopCount++;
				if(innerLoopCount > maxInnerLoops)
				{
					if(debugFile != NULL)
					{
						fprintf(debugFile, "[DEBUG] ERRO: Loop interno infinito detectado! Section: %d, Iterações: %d\n", section, innerLoopCount);
						fflush(debugFile);
					}
					break;
				}

				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						if(debugFile != NULL && innerLoopCount > 100)
						{
							fprintf(debugFile, "[DEBUG] 'end' encontrado no loop interno após %d iterações\n", innerLoopCount);
							fflush(debugFile);
						}
						break;
					}

					int index = lpMemScript->GetNumber();

					strcpy_s(this->r_Data[index].Name, lpMemScript->GetAsString());

					strcpy_s(this->r_Data[index].Col1, lpMemScript->GetAsString());

					strcpy_s(this->r_Data[index].Col2, lpMemScript->GetAsString());

					strcpy_s(this->r_Data[index].Col3, lpMemScript->GetAsString());

					strcpy_s(this->r_Data[index].Col4, lpMemScript->GetAsString());

					this->m_count++;
				}
				else
				{
					// Se section != 0, precisa sair do loop interno
					if(debugFile != NULL && innerLoopCount == 1)
					{
						fprintf(debugFile, "[DEBUG] Section != 0 (%d), saindo do loop interno\n", section);
						fflush(debugFile);
					}
					break;
				}
			}
		}
	}
	catch(...)
	{
		if(debugFile != NULL)
		{
			fprintf(debugFile, "[DEBUG] ERRO: Exceção capturada! Erro: %s\n", lpMemScript->GetLastError());
			fflush(debugFile);
		}
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] Deletando lpMemScript...\n");
		fflush(debugFile);
	}
	delete lpMemScript;

	if(debugFile != NULL)
	{
		fprintf(debugFile, "[DEBUG] CCustomRanking::Load concluido. Total de registros: %d, Total de iterações: %d\n", this->m_count, loopCount);
		fclose(debugFile);
	}

	//LogAdd(LOG_BLUE, "[ServerInfo] CustomRanking Info loaded successfully");
}

void CCustomRanking::GCReqRankingCount(int Index,PMSG_CUSTOM_RANKING_COUNT_RECV* lpMsg)
{

#if (GAMESERVER_CLIENTE_UPDATE >= 6)

	if(gServerInfo.m_CustomRankingSwitch == 0)
	{
		return;
	}

	PMSG_CUSTOM_RANKING_COUNT_SEND pMsg;

    pMsg.header.set(0xF3,0xE7, sizeof(pMsg)); 

	pMsg.count = (this->m_count > MAX_RANK) ? MAX_RANK : this->m_count;

	DataSend(Index, (LPBYTE)&pMsg, pMsg.header.size);

#endif

}

void CCustomRanking::GCReqRanking(int Index,PMSG_CUSTOM_RANKING_RECV* lpMsg)
{

#if (GAMESERVER_CLIENTE_UPDATE >= 6)

	if(gServerInfo.m_CustomRankingSwitch == 0)
	{
		return;
	}

	if(gObjIsConnected(Index) == false)
	{
		return;
	}

	if (this->m_count <= 0 )
	{
		return;
	}

	if (lpMsg->type < 0 || lpMsg->type >= this->m_count)
	{
		return;
	}

	if (this->m_count == 0)
	{
		return;
	}

	SDHP_CUSTOM_RANKING_SEND pMsg;

    pMsg.header.set(0xF4, sizeof(pMsg)); 

	pMsg.index = Index;

	pMsg.type = lpMsg->type;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);

#endif
}

void CCustomRanking::GDCustomRankingRecv(SDHP_CUSTOM_RANKING_RECV* lpMsg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 6)

	if( lpMsg == NULL )
	{
		return;
	}

	BYTE send[4096];

	PMSG_CUSTOM_RANKING_SEND pMsg;

	pMsg.header.set(0xF3,0xE6,0);

	int size = sizeof(pMsg);

	memcpy(pMsg.rankname, this->r_Data[lpMsg->type].Name, sizeof(pMsg.rankname));
	memcpy(pMsg.col1, this->r_Data[lpMsg->type].Col1, sizeof(pMsg.col1));
	memcpy(pMsg.col2, this->r_Data[lpMsg->type].Col2, sizeof(pMsg.col2));
	memcpy(pMsg.col3, this->r_Data[lpMsg->type].Col3, sizeof(pMsg.col3));
	memcpy(pMsg.col4, this->r_Data[lpMsg->type].Col4, sizeof(pMsg.col4));

	pMsg.count = 0;

	CUSTOM_RANKING_DATA info;

	for (int n = 0; n < lpMsg->count; n++)
	{
		CUSTOM_RANKING_DATA* lpInfo = (CUSTOM_RANKING_DATA*)(((BYTE*)lpMsg) + sizeof(SDHP_CUSTOM_RANKING_RECV) + (sizeof(CUSTOM_RANKING_DATA) * n));
		memcpy(info.Value1, lpInfo->Value1, sizeof(info.Value1));
		memcpy(info.Value2, lpInfo->Value2, sizeof(info.Value2));
		memcpy(info.Value3, lpInfo->Value3, sizeof(info.Value3));
		memcpy(info.Value4, lpInfo->Value4, sizeof(info.Value4));

		pMsg.count++;

		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);
	// ---
	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(lpMsg->index,send,size);

#endif
}