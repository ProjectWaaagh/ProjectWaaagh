#[[ 

                ProjectWaaagh - MMORPG Framework
            --------------------------------------

    ~ Authors:  ProjectWaaagh Contributors
    ~ Email:    info@project-waaagh.net
    ~ Webiste:  https://www.project-waaagh.net

    ~ Copyright: 
        The code of AlpakaCore is shipped as it is without any form of warranty,
        and - except for third party libraries - licensed under the GPL 2.0,
        which you can read from the file "COPYING"

#]]

# check what platform we're on (64-bit or 32-bit), and create a simpler test than CMAKE_SIZEOF_VOID_P
if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PLATFORM 64)
    MESSAGE(STATUS "Detected 64-Bit Platform")
else()
    set(PLATFORM 32)
    MESSAGE(STATUS "Detected 32-Bit Platform")
endif()

if(WIN32)
  include("${CMAKE_SOURCE_DIR}/CMake/Platform/Windows/Settings.cmake")
elseif(UNIX)
  include("${CMAKE_SOURCE_DIR}/CMake/Platform/Unix/Settings.cmake")
endif()