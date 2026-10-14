
#ifndef __EMBREE_SYSINFO_H__
#define __EMBREE_SYSINFO_H__

#include "platform.h"

namespace embree
{
    enum CPUModel {
        CPU_UNKNOWN,
        CPU_CORE1,
        CPU_CORE2,
        CPU_CORE_NEHALEM,
        CPU_CORE_SANDYBRIDGE
    };

    std::string getExecutableFileName();

    std::string getPlatformName();

    std::string getCPUVendor();

    CPUModel getCPUModel();

    size_t getNumberOfLogicalThreads();

    int getTerminalWidth();
}

#endif