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
    set(PRIVATE_PCH_HEADER PrecompiledHeaders/DatabasePCH.hpp)
endif()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Database
    ${PRIVATE_SOURCES}
)

# Do NOT add any extra include directory unless it does not create unneeded extra dependencies,
# and specially, not add any dependency to neither of these: shared, game, scripts
# This way we ensure that if either a PR does that without modifying this file,
# a compile error will be generated, either this file will be modified so it
# is detected more easily.
# While it is OK to include files from other libs as long as they don't require
# linkage (enums, defines...) it is discouraged to do so unless necessary, as it will pullute
# include_directories leading to further unnoticed dependency aditions
# Linker Depencency requirements: common
CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC_INCLUDES
    # Exclude
        ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders)

include_directories(${MYSQL_INCLUDE_DIR})
target_include_directories(Database
    PUBLIC
        ${PUBLIC_INCLUDES}
        mysql
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DPRW_API_EXPORT_DATABASE)

target_link_libraries(Database
    PRIVATE
        projectWaaagh-core-interface
        mysql
    PUBLIC
        Framework)

set_target_properties(Database
    PROPERTIES
        FOLDER
            "Servers")

if(BUILD_SHARED_LIBS)
    if(UNIX)
        install(TARGETS Database
            LIBRARY
            DESTINATION lib)
    elseif(WIN32)
        install(TARGETS Database
            RUNTIME
            DESTINATION "${CMAKE_INSTALL_PREFIX}")
    endif()
endif()

# Generate precompiled header
if(USE_COREPCH)
    add_cxx_pch(Database ${PRIVATE_PCH_HEADER})
endif()