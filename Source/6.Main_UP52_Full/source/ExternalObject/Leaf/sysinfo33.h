
#ifndef _SYSINFO_H_
#define _SYSINFO_H_

#include "regkey.h"

#pragma warning(disable : 4786)
#include <string>

namespace leaf {

	bool GetOSInfoString(OUT std::string& osinfo);
	void GetCPUInfoString(OUT std::string& cpuinfo);
}

#endif