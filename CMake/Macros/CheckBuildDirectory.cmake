# Forced out-of-source build

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" BUILDING_IN_SOURCE)

if(BUILDING_IN_SOURCE)
    message(FATAL_ERROR "
        *** ATTENTION ***
        ProjectWaaagh requires an out of source build. 
        Remove 'CMakeCache.txt' found in this directory before continuing.
        Create a seperate build directory and run 'cmake -S PATH_TO_SOURCE_DIR -B PATH_TO_BUILD_DIR
    ")
endif()