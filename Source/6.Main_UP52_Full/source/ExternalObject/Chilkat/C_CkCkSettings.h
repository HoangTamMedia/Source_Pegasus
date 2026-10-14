#ifndef _CkCkSettings_H
#define _CkCkSettings_H
#include "Chilkat_C.h"

HCkCkSettings CkCkSettings_Create(void);
void CkCkSettings_Dispose(HCkCkSettings handle);

static void CkCkSettings_cleanupMemory(HCkCkSettings handle);

static unsigned long CkCkSettings_getTotalSizeProcessHeaps(HCkCkSettings handle);

static void CkCkSettings_initializeMultithreaded(HCkCkSettings handle);
#endif
