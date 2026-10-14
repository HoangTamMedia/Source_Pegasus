#pragma once

#include <memory>

class CpuUsage {
public:
    static CpuUsage* Instance();

    double GetUsage();

private:
    CpuUsage();
    ~CpuUsage();

    class Impl;
    std::unique_ptr<Impl> pImpl;
};
