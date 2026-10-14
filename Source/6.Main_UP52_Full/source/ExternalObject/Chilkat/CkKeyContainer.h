
#ifndef _CKKEYCONTAINER_H
#define _CKKEYCONTAINER_H

#pragma once

class CkString;

#include "CkObject.h"
#include "CkString.h"
#include "CkByteData.h"

class CkPrivateKey;
class CkPublicKey;
class CkStringArray;

#pragma pack (push, 8)

class CkKeyContainer : public CkObject
{
    private:
	void *m_impl;
	bool m_utf8;

	CkKeyContainer(const CkKeyContainer &) { }
	CkKeyContainer &operator=(const CkKeyContainer &) { return *this; }
	CkKeyContainer(void *impl) : m_impl(impl) { }

    public:

	CkKeyContainer();
	virtual ~CkKeyContainer();

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

	bool get_IsOpen(void);

	bool get_IsMachineKeyset(void);

	void get_ContainerName(CkString &name);

	void GenerateUuid(CkString &guid);

	bool CreateContainer(const char *name, bool machineKeyset);
	bool OpenContainer(const char *name, bool needPrivateKeyAccess, bool machineKeyset);
	bool DeleteContainer(void);
	void CloseContainer(void);

	bool GenerateKeyPair(bool bKeyExchangePair, int keyLengthInBits);

	CkPrivateKey *GetPrivateKey(bool bKeyExchangePair);

	CkPublicKey *GetPublicKey(bool bKeyExchangePair);

	bool ImportPublicKey(CkPublicKey &key, bool bKeyExchangePair);
	bool ImportPrivateKey(CkPrivateKey &key, bool bKeyExchangePair);

	bool FetchContainerNames(bool bMachineKeyset);

	int GetNumContainers(bool bMachineKeyset);
	void GetContainerName(bool bMachineKeyset, int index, CkString &name);

	const char *getContainerName(bool bMachineKeyset, int index);
	const char *containerName(void);
	const char *generateUuid(void);

	bool SaveLastError(const char *filename);
	void LastErrorXml(CkString &str);
	void LastErrorHtml(CkString &str);
	void LastErrorText(CkString &str);

	CkString m_resultString;

	const char *lastErrorText(void);
	const char *lastErrorXml(void);
	const char *lastErrorHtml(void);

};
#pragma pack (pop)

#endif
