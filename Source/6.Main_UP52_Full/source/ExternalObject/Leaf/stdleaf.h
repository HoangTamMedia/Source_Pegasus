
#ifndef _STDLEAF_H_
#define _STDLEAF_H_

#pragma once

#include <windows.h>

#pragma warning(disable : 4786)
#include <string>

#ifndef _QWORD_DEFINED_
#define _QWORD_DEFINED_

typedef __int64	QWORD;

#ifndef MAKEQWORD
#define MAKEQWORD(low, high) \
	((QWORD)( ((QWORD) ((DWORD) (high))) << 32 | ((DWORD) (low))))
#endif

#ifndef LODWORD
#define LODWORD(qw) \
	((DWORD)(qw))
#endif

#ifndef HIDWORD
#define HIDWORD(qw) \
	((DWORD)(((QWORD)(qw) >> 32) & 0xFFFFFFFF))
#endif

#endif

#ifndef MAKEDOWRD
#define MAKEDWORD(low, high) \
	((DWORD)( ((DWORD) ((WORD) (high))) << 16 | ((WORD) (low))))
#endif

#ifndef MAKE_STRING
#define MAKE_STRING(name) "" #name
#endif

namespace leaf {

	bool CreateDirectoryIncSub(const std::string& path);
	bool DeleteDirectoryIncSub(const std::string& path);

	bool GetFileSizeQW(const HANDLE hFile, QWORD &qwSize);

	void GetAbsolutePath(IN const std::string& path, OUT std::string& abspath);
	void GetAbsoluteFilePath(IN const std::string& path, OUT std::string& abspath, OUT std::string& filename);

	void SplitFileName(IN const std::string& filepath, OUT std::string& filename, bool bIncludeExt = true);
	void SplitDirectoryPath(IN const std::string& filepath, OUT std::string& dir);
	void SplitExt(IN const std::string& filepath, OUT std::string& ext, bool bIncludeDot = false);

	void ExtractDirectoryName(IN const std::string& path, OUT std::string& dirname);
	void ExchangeExt(IN const std::string& in_filepath, IN const std::string& ext, OUT std::string& out_filepath);

	bool CompareFilePath(IN const std::string& path1, IN const std::string& path2, size_t size);
	void AppendFilePath(IN const std::string& dir, IN const std::string& to_append, OUT std::string& out_path);

	bool GetFileCreationTime(IN const std::string& path, OUT SYSTEMTIME& systime, bool bLocalTime = true);
	bool GetFileLastModifiedTime(IN const std::string& path, OUT SYSTEMTIME& systime, bool bLocalTime = true);
	bool GetFileLastAccessedTime(IN const std::string& path, OUT SYSTEMTIME& systime, bool bLocalTime = true);

	const char* FormatString(const char* pFormat, ...);
	void GetTimeString(OUT std::string& str);
}

#endif
