
#ifndef _EXCEPTION_HANDLER_H_
#define _EXCEPTION_HANDLER_H_

#include "sysinfo.h"
#include "callstackdmp.h"

#pragma warning(disable : 4786 4551)
#include <string>

#pragma comment(lib, "Version.lib")

#define _CALLSTACK_DUMP leaf::CCallStackDump

namespace leaf {

	typedef bool (*EXCEPTION_CALLBACK)(_EXCEPTION_POINTERS* pExceptionInfo);

	bool AttachExceptionHandler(const std::string& dmpfile, bool bSecondChance = false);
	bool AttachExceptionHandler(EXCEPTION_CALLBACK pfCallback, bool bSecondChance = false);
	bool DetachExceptionHandler();

	bool IsContinueExceptionToSecondChance();

	bool SaveExceptionDumpFile(const std::string& filename, CONTEXT* pContext, _EXCEPTION_POINTERS* pExceptionInfo = NULL);

#pragma pack(push, 1)

	typedef struct _DMPPROCESSINFOHEADER {
		DWORD	ProcessId;
		CHAR	szImageName[256];
		DWORD	CheckSum32;
		DWORD	ThreadCount;
		BYTE	IsExistFixedFileInfo;
		DWORD	NumOfModules;
	} DMPPROCESSINFOHEADER, * LPDMPPROCESSINFOHEADER;

	typedef struct _DMPMODULEINFO {
		CHAR	szModuleBaseName[128];
		CHAR	szModulePath[256];
		LPVOID	lpBaseAddr;
		DWORD	SizeOfModule;
	} DMPMODULEINFO, * LPDMPMODULEINFO;

	typedef struct _DMPEXCEPTIONINFOHEADER {
		DWORD	ExceptionCode;
		DWORD	ExceptionAddress;
		CHAR	ExceptionCodeString[128];
	} DMPEXCEPTIONINFOHEADER, * LPDMPEXCEPTIONINFOHEADER;

	typedef struct _DMPCALLSTACKFRAME {
		DWORD	FrameAddr;
		DWORD	ReturnAddr;
		DWORD	Parameter[16];
	} DMPCALLSTACKFRAME, * LPDMPCALLSTACKFRAME;

	typedef struct _DMPFILEHEADER {
		BYTE	Sign[4];
		BYTE	Version;
		DWORD	HeaderSize;

		DWORD	CreationTimeStamp;

		CHAR	OSInfoString[128];
		CHAR	CPUInfoString[128];
		MEMORYSTATUS	MemStatus;

		DMPPROCESSINFOHEADER	ProcessInfo;

		DMPEXCEPTIONINFOHEADER	ExceptionInfo;

		CONTEXT	ContextRecord;

		DWORD	CallStackDepth;
	} DMPFILEHEADER, * LPDMPFILEHEADER;

#pragma pack(pop)

	class CExceptionHandler {
		LPTOP_LEVEL_EXCEPTION_FILTER	m_pfPrevExceptionFilter;
		EXCEPTION_CALLBACK				m_pfExceptionCallback;

		std::string	m_filename;
		bool		m_bSecondChance;

	public:
		~CExceptionHandler();

		bool AttachExceptionHandler(const std::string& filename, bool bSecondChance);
		bool AttachExceptionHandler(EXCEPTION_CALLBACK pfCallback, bool bSecondChance);
		bool DetachExceptionHandler();
		bool IsEnableSecondChance() const;

		const std::string& GetDmpFileName() const;
		bool SaveDmpFile(const std::string& filename, CONTEXT* pContext, _EXCEPTION_POINTERS* pExceptionInfo);

		bool IsEnableCallback() const;
		bool CallExceptionCallback(_EXCEPTION_POINTERS* pExceptionInfo);

		static CExceptionHandler* GetObjPtr();
		static LONG __stdcall OnException(_EXCEPTION_POINTERS* pExceptionInfo);

	private:
		CExceptionHandler();

		void SetProcessInfoHeader(DMPPROCESSINFOHEADER* pProcessInfoHeader, DMPMODULEINFO** ppModuleInfo, int MaxModules, VS_FIXEDFILEINFO& ImageFileInfo);
		void SetExceptionInfoHeader(DMPEXCEPTIONINFOHEADER* pExceptionInfoHeader, _EXCEPTION_POINTERS* pExceptionInfo);
		void GetExceptionCodeString(IN PEXCEPTION_RECORD pExceptionRecord, OUT std::string& type);
	};

	class CDmpFileLoader {
		typedef std::vector<DMPCALLSTACKFRAME*> t_framevect;
		typedef std::vector<DMPMODULEINFO*>	t_modulevect;

		DMPFILEHEADER		m_DmpFileHeader;
		VS_FIXEDFILEINFO	m_FixedImageFileInfo;

		t_framevect		m_listStackFrame;
		t_modulevect	m_listModule;

		void Init();
	public:
		CDmpFileLoader();
		virtual ~CDmpFileLoader();

		bool Create(const std::string& dmpfile);
		void Release();

		DWORD GetCreationTimeStamp() const;

		const char* GetOSInfoString() const;
		const char* GetCPUInfoString() const;
		const MEMORYSTATUS& GetMemoryStatus() const;

		DWORD GetProcessId() const;
		DWORD GetCheckSum32() const;
		DWORD GetThreadCount() const;
		const char* GetImageName() const;
		const VS_FIXEDFILEINFO* GetFixedImageFileInfo() const;

		size_t GetNumOfModules() const;
		const DMPMODULEINFO* GetModuleInfo(int index) const;

		const CONTEXT& GetContextRecord() const;
		DWORD GetExceptionCode() const;
		DWORD GetExceptionAddress() const;
		const char* GetExceptionCodeString() const;

		size_t GetStackDepth() const;
		const DMPCALLSTACKFRAME* GetCallStackFrame(int index) const;
	};

}

#endif
