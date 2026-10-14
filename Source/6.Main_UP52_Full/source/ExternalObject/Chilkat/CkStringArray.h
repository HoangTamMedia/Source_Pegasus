
#pragma once

#include "CkString.h"
class CkByteData;

#include "CkObject.h"

#pragma pack (push, 8)

class CkStringArray : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkStringArray(const CkStringArray &c) { }
	CkStringArray &operator=(const CkStringArray &) { return *this; }
	CkStringArray(void *impl) : m_impl(impl) { }

    public:
	void *getImpl(void) const { return m_impl; }
	void inject(void *impl);

	CkStringArray();
	virtual ~CkStringArray();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	void Append(const char *str);

	long get_Count();

	bool LoadFromFile(const char *filename);

	bool SaveToFile(const char *filename);

	bool Pop(CkString &str);

	bool LastString(CkString &str);

	bool RemoveAt(long index);

	void InsertAt(long index, const char *str);

	long Find(const char *str, long firstIndex);

	void Prepend(const char *str);

	bool get_Crlf();
	void put_Crlf(bool newVal);

	bool get_Trim();
	void put_Trim(bool newVal);

	bool get_Unique();
	void put_Unique(bool newVal);

	void Serialize(CkString &encodedStr);

	bool AppendSerialized(const char *encodedStr);

	bool GetString(long index, CkString &str);
	const char *GetString(long index);
        const char *getString(long index);

	void Subtract(CkStringArray &array);

	void Remove(const char *str);

	bool Contains(const char *str);

	void Clear();

	void Sort(bool ascending);

	void SplitAndAppend(const char *str, const char *boundary);

	CkString m_resultString;
        const char *strAt(long index);
        const char *serialize(void);
        const char *pop(void);
	const char *lastStr(void);

	void LoadFromText(const char *str);

	bool SaveNthToFile(int index, const char *filename);

	void SaveToText(CkString &outStr);
	const char *saveToText(void);

	int FindFirstMatch(const char *str, int firstIndex);

	void Union(CkStringArray &array);

	int GetStringLen(int index);

};
#pragma pack (pop)
