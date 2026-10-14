
#if !defined(AFX_CKOBJECTARRAY_H__10278D9D_3CD5_4AC8_A322_CE62A7DF95B6__INCLUDED_)
#define AFX_CKOBJECTARRAY_H__10278D9D_3CD5_4AC8_A322_CE62A7DF95B6__INCLUDED_

#pragma once

#include "CkString.h"
#include "CkObject.h"

#pragma pack (push, 8)

class CkObjectArray  : public CkObject
{
    private:
	void *m_impl;

	CkObjectArray(const CkObjectArray &a) { }

    public:
	CkObjectArray();
	virtual ~CkObjectArray();

	CkObject *GetAt(int index) const;
	CkObject *GetLast(void) const;

	void InsertAt(int index, CkObject *obj);
	void RemoveAt(int index);

	void DeleteAllObjects(void);

	void ClearWithoutDeleting(void);

	void Append(CkObject *obj);
	CkObject *Pop(void);

	int get_Size(void) const;

};
#pragma pack (pop)

#endif
