include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Macros/EnsureVersion.cmake)

set(MIN_REQ_GIT_VER "2.30")

find_program(GIT_EXECUTABLE
    NAMES
        git git.cmd
    HINTS
        ENV PATH
    DOC "Full path to git commandline client"
)
MARK_AS_ADVANCED(GIT_EXECUTABLE)

if(NOT GIT_EXECUTABLE)
    message(FATAL_ERROR "
    *** GIT WAS NOT FOUND ON YOUR SYSTEM***
    Did you forget to install a recent version, or setting the path to it?
    Observe that for Revision Hash/Date to work you need at least
    Git-Version: ${MIN_REQ_GIT_VER}")
else()
    message(STATUS "Found Git Binary: ${GIT_EXECUTABLE}")
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" --version
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE _GIT_VERSION
        ERROR_QUIET
    )

    ensure_version("${MIN_REQ_GIT_VER}" "${_GIT_VERSION}" _GIT_VERSION_OK)

    if(NOT _GIT_VERSION_OK)
        message(STATUS "Git Version too old: ${_GIT_VERSION}")
        message(FATAL_ERROR 
        "Git was found but is OUTDATED
        Did you forget to install a recent version, or setting the path to it?
        Observe that for Revision Hash/Date to work you need at least
        Git-Version: ${MIN_REQ_GIT_VER}")
    endif()
endif()