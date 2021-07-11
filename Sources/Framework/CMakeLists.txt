CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DPW_API_EXPORT_FRAMEWORK)

add_library(Framework ${PRIVATE_SOURCES})


CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC_INCLUDES
    # Exclude Directories
    ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders)

target_include_directories(Framework
    PUBLIC
        ${CMAKE_BINARY_DIR}
        ${PUBLIC_INCLUDES}
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Framework 
    PRIVATE
        project-waaagh-core-interface)


set_target_properties(Framework
    PROPERTIES
        FOLDER "Servers")

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

