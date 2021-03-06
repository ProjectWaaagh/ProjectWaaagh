//
// Created by bachm on 13.02.2022.
//

#ifndef PROJECTWAAAGH_TIMER_HPP
#define PROJECTWAAAGH_TIMER_HPP

#include "Defines.hpp"
#include "Durations.hpp"
inline TimePoint GetApplicationStartTime()
{
    using namespace std::chrono;

    static const steady_clock::time_point ApplicationStartTime = steady_clock::now();

    return ApplicationStartTime;
}
inline uint32 getMSTime()
{
    using namespace std::chrono;

    return uint32(duration_cast<milliseconds>(steady_clock::now() - GetApplicationStartTime()).count());
}

inline uint32 getMSTimeDiff(uint32 oldMSTime, uint32 newMSTime)
{
    // getMSTime() have limited data range and this is case when it overflow in this tick
    if (oldMSTime > newMSTime)
        return (0xFFFFFFFF - oldMSTime) + newMSTime;
    else
        return newMSTime - oldMSTime;
}

inline uint32 getMSTimeDiff(uint32 oldMSTime, TimePoint newTime)
{
    using namespace std::chrono;

    uint32 newMSTime = uint32(duration_cast<milliseconds>(newTime - GetApplicationStartTime()).count());
    return getMSTimeDiff(oldMSTime, newMSTime);
}

inline uint32 GetMSTimeDiffToNow(uint32 oldMSTime)
{
    return getMSTimeDiff(oldMSTime, getMSTime());
}


#endif //PROJECTWAAAGH_TIMER_HPP
