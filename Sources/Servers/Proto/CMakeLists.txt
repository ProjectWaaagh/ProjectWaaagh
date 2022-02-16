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

if (USE_COREPCH)
    set(PRIVATE_PCH_HEADER PrecompiledHeaders/ProtoPCH.h)
endif()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DPRW_API_EXPORT_PROTO)

add_library(Proto
    ${PRIVATE_SOURCES}
)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC_INCLUDES
    # Exclude
        ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders
        ${CMAKE_CURRENT_SOURCE_DIR}/Client/API
        ${CMAKE_CURRENT_SOURCE_DIR}/Client/global_extensions)

target_include_directories(Proto
    PUBLIC
        ${PUBLIC_INCLUDES}
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Proto
    PRIVATE
        projectWaaagh-default-interface
        projectWaaagh-no-warning-interface
    PUBLIC
        Framework
        #Protobuf
        )
#[[
if (MSVC)
    target_compile_definitions(Protobuf
PRIVATE
-D_SCL_SECURE_NO_WARNINGS)
endif()
#]]

set_target_properties(Proto
    PROPERTIES
        FOLDER
            "Servers")

if( BUILD_SHARED_LIBS )
    if( UNIX )
        install(TARGETS Proto
            LIBRARY
            DESTINATION lib)
    elseif( WIN32 )
        install(TARGETS Proto
            RUNTIME
            DESTINATION "${CMAKE_INSTALL_PREFIX}")
    endif()
endif()

# Generate precompiled header
if (USE_COREPCH)
    add_cxx_pch(Proto ${PRIVATE_PCH_HEADER})
endif()