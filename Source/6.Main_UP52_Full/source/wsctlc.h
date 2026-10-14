
#ifndef __WSCTLC_H__
#define __WSCTLC_H__
#include "Protect.h"
#include "./Utilities/Log/ErrorReport.h"
#include "./Utilities/Log/muConsoleDebug.h"

#define MAX_SENDBUF		8192
#define MAX_RECVBUF		8192

typedef struct
{
	int used;
	int len;
	BYTE buf[MAX_RECVBUF];
} STRUCT_RECVSTACK;

class CPacketQueue;

class CWsctlc {

private :
	HWND   m_hWnd;
	BOOL	m_bGame;

	int	   m_iMaxSockets;
	SOCKET m_socket;

	BYTE   m_SendBuf[MAX_SENDBUF];
	int    m_nSendBufLen;

	BYTE   m_RecvBuf[MAX_RECVBUF];
	int    m_nRecvBufLen;

	int		m_LogPrint;
	FILE*   m_logfp;

	CPacketQueue*	m_pPacketQueue;

	BOOL ShutdownConnection(SOCKET sd);

public:

	CWsctlc(void);
	~CWsctlc();

	SOCKET GetSocket();

	BOOL Startup();
	void Cleanup();

	int  Create(HWND hWnd, BOOL bGame = FALSE);
	BOOL Close();
	BOOL Close(SOCKET & socket);

	BOOL Connect(char *ip_addr, unsigned short port, DWORD WinMsgNum);

	int  sSend(SOCKET socket, char *buf, int len);
	__forceinline int  sSend(char *buf, int len)
	{
		int nResult;

		int nLeft = len;
		int nDx=0;
		if( m_socket == INVALID_SOCKET ) return FALSE;

		for( ; ; )
		{
#if(ENCRYPT_STATE==1)
			if (gProtect.CheckSocketPort(m_socket) != 0)
			{
				gProtect.EncryptData((BYTE*)buf + nDx, len - nDx);

			}
#endif
			nResult = send(m_socket, (char*)buf+nDx, len-nDx, 0);
			if( nResult == SOCKET_ERROR )
			{
				if( WSAGetLastError() != WSAEWOULDBLOCK )
				{
					g_ConsoleDebug->Write(MCD_ERROR, "[Send Packet Error] WSAGetLastError() != WSAEWOULDBLOCK");
					g_ConsoleDebug->Write(MCD_ERROR, "[Send Packet Error] WSAGetLastError = %d", WSAGetLastError());
					g_ErrorReport.Write("[Send Packet Error] WSAGetLastError() != WSAEWOULDBLOCK\r\n");
					Close();
					return FALSE;
				}
				else
				{
					if( (m_nSendBufLen+len) > MAX_SENDBUF )
					{
						g_ConsoleDebug->Write(MCD_ERROR, "[Send Packet Error] SendBuffer Overflow");
						g_ConsoleDebug->Write(MCD_ERROR, "[Send Packet Error] WSAGetLastError = %d", WSAGetLastError());
						g_ErrorReport.Write("[Send Packet Error] SendBuffer Overflow\r\n");
						Close();
						return FALSE;
					}
					memcpy( m_SendBuf+m_nSendBufLen, buf, nLeft);
					m_nSendBufLen += nLeft;

					return FALSE;
				}
			}
			else
			{
				if( nResult == 0 )
				{

					break;
				}
				if( m_LogPrint )
				{
					LogHexPrintS((BYTE*)buf, nResult);
				}
			}
			nDx += nResult;
			nLeft -= nResult;
			if( nLeft <= 0 ) break;
		}
		return TRUE;
	}
	int  FDWriteSend();

	int  nRecv();

	BYTE * GetReadMsg();

	void LogPrint( char *szlog, ...);
	void LogHexPrint(BYTE *buf, int size);
	void LogHexPrintS( BYTE *buf, int size);
	void LogPrintOn();
	void LogPrintOff();

};

#endif
