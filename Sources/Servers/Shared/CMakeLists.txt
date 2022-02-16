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
    set(PRIVATE_PCH_HEADER PrecompiledHeaders/SharedPCH.h)
endif()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DPRW_API_EXPORT_SHARED)

add_library(Shared
    ${PRIVATE_PCH_SOURCE}
    ${PRIVATE_SOURCES}
)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC_INCLUDES
    # Exclude
        ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders)

target_include_directories(Shared
    PUBLIC
        ${PUBLIC_INCLUDES}
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Shared
    PRIVATE
        projectWaaagh-core-interface
    PUBLIC
        Database
        #rapidjson
        #proto
        #zlib
        #gsoap
        )

set_target_properties(Shared
    PROPERTIES
        FOLDER
            "Servers")

if(BUILD_SHARED_LIBS)
    if(UNIX)
        install(TARGETS Shared
            LIBRARY
            DESTINATION lib)
    elseif(WIN32)
        install(TARGETS Shared
            RUNTIME
            DESTINATION "${CMAKE_INSTALL_PREFIX}")
    endif()
endif()

# Generate precompiled header
if(USE_COREPCH)
    add_cxx_pch(Shared ${PRIVATE_PCH_HEADER})
endif()