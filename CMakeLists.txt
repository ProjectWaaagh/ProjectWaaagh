cmake_minimum_required(VERSION 3.16)

# Project Name (must be done AFTER settings configuration types)
project(ProjectWaaagh)


# CMake Policies settings

# CMake macro-directory
list(APPEND CMAKE_MODULE_PATH
        "${CMAKE_SOURCE_DIR}/CMake/Macros")

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)


# Set default build options
include(CMake/Options.cmake)

include(ConfigureTargets)
include(CheckPlatform)

include(GroupSources)
include(AutoCollect)

find_package(PCHSupport)
find_package(MySQL)

if(NOT WITHOUT_GIT)
    find_package(Git)
endif()

# Find revision ID and Hash of the sourceTree
include(CMake/GenRevision.cmake)

# Print out the configuration results before continuing
include(CMake/ShowOptions.cmake)

# Add dependencies
add_subdirectory(Dependencies)

# Add ProjectWaaagh sources
add_subdirectory(Sources)