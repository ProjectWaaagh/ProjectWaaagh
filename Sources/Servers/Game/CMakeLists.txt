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
        ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders)

if(USE_COREPCH)
    set(PRIVATE_PCH_HEADER PrecompiledHeaders/GamePCH.h)
endif()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DPRW_API_EXPORT_GAME)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC_INCLUDES
    # Exclude
        ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders)

# Provide an interface target for the game project to allow
# dependent projects to build meanwhile.
add_library(Game-interface INTERFACE)

target_include_directories(Game-interface
    INTERFACE
        ${PUBLIC_INCLUDES})

target_link_libraries(Game-interface
    INTERFACE
        Shared
        #Detour
        )

add_library(Game
    ${PRIVATE_SOURCES})

target_include_directories(Game
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Game
    PRIVATE
        projectWaaagh-core-interface
        #efsw
    PUBLIC
        Game-interface)

set_target_properties(Game
    PROPERTIES
        FOLDER
            "Servers")

if(BUILD_SHARED_LIBS)
    if(UNIX)
        install(TARGETS Game
            LIBRARY
            DESTINATION lib)
    elseif(WIN32)
        install(TARGETS Game
            RUNTIME
            DESTINATION "${CMAKE_INSTALL_PREFIX}")
    endif()
endif()

# Generate precompiled header
if(USE_COREPCH)
    add_cxx_pch(Game ${PRIVATE_PCH_HEADER})
endif()