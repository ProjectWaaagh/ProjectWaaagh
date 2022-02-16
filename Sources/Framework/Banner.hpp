#ifndef PROJECTWAAAGH_BANNER_HPP
#define PROJECTWAAAGH_BANNER_HPP

#include "Defines.hpp"

namespace ProjectWaaagh::Banner {
	PRW_FRAMEWORK_API void Show(char const* applicationName, void(*log)(char const* text), void(*logExtraInfo)());
}

#endif
