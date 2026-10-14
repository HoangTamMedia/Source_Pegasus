
#if !defined(AFX_CKSETTINGS_H__62E2D44A_4858_429D_994D_A6EAAD2FFEA1__INCLUDED_)
#define AFX_CKSETTINGS_H__62E2D44A_4858_429D_994D_A6EAAD2FFEA1__INCLUDED_

#pragma once

#pragma pack (push, 8)

class CkSettings
{
public:
	CkSettings();
	virtual ~CkSettings();

	static void initializeMultithreaded(void);

	static void cleanupMemory(void);

	static unsigned long getTotalSizeProcessHeaps(void);

};
#pragma pack (pop)

#endif
