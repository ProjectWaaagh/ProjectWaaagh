//
// Created by bachm on 13.02.2022.
//

#ifndef PROJECTWAAAGH_DURATIONS_HPP
#define PROJECTWAAAGH_DURATIONS_HPP
#include <chrono>

/// Milliseconds shorthand typedef.
typedef std::chrono::milliseconds Milliseconds;

/// Seconds shorthand typedef.
typedef std::chrono::seconds Seconds;

/// Minutes shorthand typedef.
typedef std::chrono::minutes Minutes;

/// Hours shorthand typedef.
typedef std::chrono::hours Hours;

/// time_point shorthand typedefs
typedef std::chrono::steady_clock::time_point TimePoint;
typedef std::chrono::system_clock::time_point SystemTimePoint;

/// Makes std::chrono_literals globally available.
using namespace std::chrono_literals;

constexpr std::chrono::hours operator""_days(unsigned long long days)
{
    return std::chrono::hours(days * 24h);
}
#endif //PROJECTWAAAGH_DURATIONS_HPP
