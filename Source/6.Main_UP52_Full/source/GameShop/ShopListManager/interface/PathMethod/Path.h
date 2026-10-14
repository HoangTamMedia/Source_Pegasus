
#pragma once

class Path
{
public:

	static TCHAR*		GetCurrentFullPath(TCHAR* szPath);

	static TCHAR*		GetCurrentDirectory(TCHAR* szPath);

	static TCHAR*		GetCurrentFileName(TCHAR* szPath);

	static TCHAR*		SetDirString(TCHAR * szPath);

	static TCHAR*		ClearDirString(TCHAR * szPath);

	static TCHAR*		GetDirectory(TCHAR * szPath);

	static TCHAR*		GetFileName(TCHAR * szPath);

	static TCHAR*		ChangeSlashToBackSlash(TCHAR * szPath);

	static TCHAR*		ChangeBackSlashToSlash(TCHAR * szPath);

	static BOOL			ReadFileLastLine(TCHAR * szFile, TCHAR * szLastLine);

	static BOOL			WriteNewFile(TCHAR * szFile, TCHAR * szText, INT nTextSize);

	static BOOL			CreateDirectorys(TCHAR * szFilePath, BOOL bIsFile);
};
