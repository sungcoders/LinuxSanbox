#include "UtilsTick.h"

std::chrono::steady_clock::time_point UtilsTick::s_startTime;

UtilsTick::UtilsTick()
{
    s_startTime = std::chrono::steady_clock::now();
}

UtilsTick::~UtilsTick()
{
}

void UtilsTick::resetTick(void)
{
    s_startTime = std::chrono::steady_clock::now();
}

double UtilsTick::getSecTick(void)
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = now - s_startTime;
    return diff.count();
}

long long UtilsTick::getMsTick(void)
{
    auto now = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - s_startTime);
    return diff.count();
}
