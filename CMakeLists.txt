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
    "${CMAKE_SOURCE_DIR}/CMake/Macros")

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)

include(GroupSources)
include(AutoCollect)

add_subdirectory(Sources)