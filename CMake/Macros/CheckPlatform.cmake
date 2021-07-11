# Check Build platform
if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PLATFORM 64)
    message(STATUS "Detected 64-Bit Platform")
else()
    set(PLATFORM 32)
    message(STATUS "Detected 32-Bit PLatform")
endif()

if(WIN32)
    include("${CMAKE_SOURCE_DIR}/CMake/Platform/Windows/Settings.cmake")
elseif(UNIX)
    include("${CMAKE_SOURCE_DIR}/CMake/Platform/Unix/Settings.cmake")
endif()