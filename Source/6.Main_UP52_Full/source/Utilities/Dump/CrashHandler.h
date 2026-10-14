#ifndef _CRASHHANDLER_H_
#define _CRASHHANDLER_H_

#if _MSC_VER >= 1000
#pragma once
#endif

#include "CrashReporter.h"
#include <list>

#include "Uploader.h"
#include "Logger.h"

#pragma comment(lib, "DbgHelp")

#include <IPHlpApi.h>
#pragma comment(lib, "IPHlpApi")

#define C_RUNTIME_WARNING		((DWORD   )0x00100000L)
#define C_RUNTIME_ERROR			((DWORD   )0x00100001L)
#define C_RUNTIME_ASSERTION		((DWORD   )0x00100002L)
#define UNEXPECTED_ERROR		((DWORD   )0x00100003L)

typedef struct
{
	HINTERNET* pInternetSession;
	HINTERNET* pHttpConnection;
	LPCTSTR szUrl;
} HTTP_PARM;

class CCrashHandler
{
public:
	const static int BUFFER_SIZE = 256;
	const static int MAX_FILE_COUNT = 5;
	const static DWORD TIME_OUT = 1000;
	const static int MAX_STACKTRACE = 10;
	const TCHAR* STACKTRACE_FILENAME;

	CCrashHandler(
		LPCALLBACK lpfnCallbackBefore,
		LPCALLBACK lpfnCallbackAfter,
		int nProjectID,
		LPCTSTR szClientVersion,
		MINIDUMP_TYPE dumpType,
		LPCTSTR szHttpUrl,
		LPCTSTR szFtpUrl,
		int nPort,
		LPCTSTR szId,
		LPCTSTR szPassword,
		int nUploadCount,
		bool bCumulative,
		bool bDeleteDumpfile,
		bool bStackTrace,
		LPCTSTR szDumpfile,
		bool bWriteCrashLog);
	~CCrashHandler();

	LONG							HandleException(PEXCEPTION_POINTERS pExInfo, PVOID pExceptionAddress, DWORD dwExceptionCode);
	BOOL							AddFile(LPCTSTR szFilename);
	void							WriteCrashLog(LPCTSTR szFormat, ...);

private:
	void							Destroyer();
	void							GetExceptionCode(DWORD dwExceptionCode, TCHAR strExceptionCode[]);
	LONG							ReportException(LPCTSTR szUrl);
	LONG							Submit(LPCTSTR szUrl, char szBuffer[]);
	LONG							GenerateErrorReport(PEXCEPTION_POINTERS pExInfo, PVOID pExceptionAddress);
	bool							IsFile(LPCTSTR szFilename);
	void							SetParameter(TCHAR szParameters[], LPCTSTR szKey, LPCTSTR szValue);
	void							GetMacAddress(TCHAR szParameters[], bool bHyphen=TRUE);
	bool							StackTrace(PEXCEPTION_POINTERS pExInfo);

	LPCALLBACK						m_lpfnCallbackBefore;
	LPCALLBACK						m_lpfnCallbackAfter;
	int								m_nProjectId;
	TCHAR							m_szClientVersion[BUFFER_SIZE];
	_MINIDUMP_TYPE					m_dumpType;
	TCHAR							m_szHttpUrl[BUFFER_SIZE];
	bool							m_bCumulative;
	bool							m_bDeleteDumpfile;
	bool							m_bStackTrace;

	LPTOP_LEVEL_EXCEPTION_FILTER	m_oldFilter;
	_invalid_parameter_handler		m_oldHandler;
	unexpected_handler				m_oldUnexpectedHandler;
	terminate_handler				m_oldTerminateHandler;
	_CRT_REPORT_HOOK				m_oldReportFunction;

	CLogger*						m_pLogger;
	std::list< LPCTSTR >			m_lstFile;

	bool							m_bCatchException;
	bool							m_bUpload;
	bool							m_bReportSuccess;

	TCHAR							m_szDumpfile[BUFFER_SIZE];
	TCHAR							m_szMacAddress[BUFFER_SIZE];
	TCHAR							m_szDate[BUFFER_SIZE];
	TCHAR							m_szTime[BUFFER_SIZE];

	int								m_nFileCount;
};

#endif