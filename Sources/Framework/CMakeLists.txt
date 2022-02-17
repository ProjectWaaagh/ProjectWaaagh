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

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE_SOURCES
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/Debugging
    ${CMAKE_CURRENT_SOURCE_DIR}/Platform
    ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders)

list(APPEND PRIVATE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Debugging/Errors.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Debugging/Errors.hpp)

if(USE_COREPCH)
    set(PRIVATE_PCH_HEADER PrecompiledHeaders/FrameworkPCH.hpp)
endif(USE_COREPCH)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DPRW_API_EXPORT_FRAMEWORK)

add_library(Framework
    ${PRIVATE_SOURCES})


# >>>

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC_INCLUDES
    # Exclude
        ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders)

target_include_directories(Framework
    PUBLIC
        # Provide the binary dir for all child targets
        ${CMAKE_BINARY_DIR}
        ${PUBLIC_INCLUDES}
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Framework
    PRIVATE
        projectWaaagh-core-interface
    PUBLIC
        #argon2
        boost
        fmt
        #g3dlib
        #Detour
        sfmt
        utf8cpp
        openssl
        #valgrind
        threads
        #jemalloc
        )

add_dependencies(Framework RevisionData.hpp)

set_target_properties(Framework
    PROPERTIES
        FOLDER
            "Servers")

if(BUILD_SHARED_LIBS)
    if(UNIX)
        install(TARGETS Framework
            LIBRARY
            DESTINATION lib)
    elseif(WIN32)
        install(TARGETS Framework
            RUNTIME
            DESTINATION "${CMAKE_INSTALL_PREFIX}")
    endif()
endif()

# Generate precompiled header
if(USE_COREPCH)
    add_cxx_pch(Framework ${PRIVATE_PCH_HEADER})
endif()