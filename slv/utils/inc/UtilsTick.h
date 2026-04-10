#ifndef UTILS_TICK_H
#define UTILS_TICK_H

#include <chrono>

class UtilsTick
{
    public:
    UtilsTick();
    ~UtilsTick();

    static void resetTick();
    static double getSecTick();
    static long long getMsTick();

private:
    static std::chrono::steady_clock::time_point s_startTime;
};


#endif // UTILS_TICK_H
