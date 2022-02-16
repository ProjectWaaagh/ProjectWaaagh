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

list(APPEND PRIVATE_SOURCES ${sources_windows})
#[[
if (WIN32)
    if (MSVC)
        list(APPEND PRIVATE_SOURCES LoginServer.rc)
    endif()
endif()
#]]
if (USE_COREPCH)
    set(PRIVATE_PCH_HEADER PrecompiledHeaders/LoginServerPCH.hpp)
endif()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(LoginServer
    ${PRIVATE_SOURCES}
)

if (NOT WIN32)
    set_target_properties(LoginServer PROPERTIES
    COMPILE_DEFINITIONS _PROJECTWAAAGH_LOGINSERVER_CONFIG="${CONF_DIR}/LoginServer.conf"
)
endif()

target_link_libraries(LoginServer
    PRIVATE
        projectWaaagh-core-interface
    PUBLIC
        Shared)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC_INCLUDES
    # Exclude
        ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders)

target_include_directories(LoginServer
    PUBLIC
        ${PUBLIC_INCLUDES}
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(LoginServer
    PROPERTIES
        FOLDER
            "Servers")

if (COPY_CONF AND WIN32)
    if ("${CMAKE_MAKE_PROGRAM}" MATCHES "MSBuild")
        add_custom_command(TARGET LoginServer
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/LoginServer.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
        )       
    elseif (MINGW)
        add_custom_command(TARGET LoginServer
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/LoginServer.conf.dist ${CMAKE_BINARY_DIR}/bin/
        )
    endif()
endif()

if (WIN32)
    if ("${CMAKE_MAKE_PROGRAM}" MATCHES "MSBuild")
        add_custom_command(TARGET LoginServer
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/LoginServer.cert.pem ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/LoginServer.key.pem ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
        )
    elseif (MINGW)
        add_custom_command(TARGET LoginServer
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/LoginServer.cert.pem ${CMAKE_BINARY_DIR}/bin/
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/LoginServer.key.pem ${CMAKE_BINARY_DIR}/bin/
        )
    endif()
endif()

if (UNIX)
    install(TARGETS LoginServer DESTINATION bin)
    if (COPY_CONF)
        install(FILES LoginServer.conf.dist DESTINATION ${CONF_DIR})
    endif()
    install(FILES LoginServer.cert.pem LoginServer.key.pem DESTINATION bin)
elseif (WIN32)
    install(TARGETS LoginServer DESTINATION "${CMAKE_INSTALL_PREFIX}")
    if (COPY_CONF)
        install(FILES LoginServer.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
    endif()
    install(FILES LoginServer.cert.pem LoginServer.key.pem DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

# Generate precompiled header
if (USE_COREPCH)
    add_cxx_pch(LoginServer ${PRIVATE_PCH_HEADER})
endif()