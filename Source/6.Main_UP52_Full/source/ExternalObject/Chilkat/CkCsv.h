
#ifndef _CkCsv_H
#define _CkCsv_H

#pragma once

#include "CkString.h"
class CkByteData;
#include "CkObject.h"

#pragma pack (push, 8)

class CkCsv  : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkCsv(const CkCsv &);
	CkCsv &operator=(const CkCsv &);

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const;

	CkCsv(void *impl);

	CkCsv();
	virtual ~CkCsv();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool SaveLastError(const char *filename);
    void LastErrorXml(CkString &str);
    void LastErrorHtml(CkString &str);
    void LastErrorText(CkString &str);

    const char *lastErrorText(void);
    const char *lastErrorXml(void);
    const char *lastErrorHtml(void);

	bool LoadFile(const char *filename);

	bool SaveFile(const char *filename);

	bool SaveFile2(const char *filename, const char *charset);

	bool LoadFile2(const char *filename, const char *charset);

	bool GetCell(int row, int col, CkString &outStr);
	const char *getCell(int row, int col);

	int get_NumRows(void);

	bool SetCell(int row, int col, const char *content);

	int GetNumCols(int row);

	void get_Delimiter(CkString &str);
	const char *delimiter(void);
	void put_Delimiter(const char *newVal);

	bool get_Crlf(void);
	void put_Crlf(bool newVal);

	bool get_HasColumnNames(void);
	void put_HasColumnNames(bool newVal);

	int get_NumColumns(void);

	int GetIndex(const char *columnName);

	bool GetColumnName(int index, CkString &outStr);
	const char *getColumnName(int index);

	bool LoadFromString(const char *csvData);

	bool SaveToString(CkString &outStr);
	const char *saveToString(void);

	bool SetColumnName(int index, const char *columnName);

};
#pragma pack (pop)

#endif
