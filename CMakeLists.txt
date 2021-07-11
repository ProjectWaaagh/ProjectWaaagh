# This File is part of the ProjectWaaagh.
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# Minimum required CMAKE_VERSION >= 3.16
cmake_minimum_required(VERSION 3.16)

# Set Project Name
project(ProjectWaaagh)


# CMake Policies

# Set CMake Macro Directory
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Macros")
list(APPEND CMAKE_MODULE_PATH
C:/Users/monop/.shizo/Development/vcpkg/scripts/cmake)

# Set vcpkg Directory
INCLUDE_DIRECTORIES(C:/Users/monop/.shizo/Development/vcpkg/installed/x86-windows/include)
LINK_DIRECTORIES(C:/Users/monop/.shizo/Development/vcpkg/installed/x86-windows/lib)


include(CheckCXXSourceRuns)
include(CheckIncludeFiles)

include(ConfigureBaseTargets)
include(CheckPlatform)

include(AutoCollect)
include(GroupSources)

find_package(MySQL)


add_subdirectory(Sources)