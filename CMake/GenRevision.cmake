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

if(NOT_BUILDDIR)
    set(BUILDDIR ${CMAKE_BINARY_DIR})
endif()

if(GIT_EXECUTABLE)
    # Retrieve repository dirty status
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" diff-index --quiet HEAD --
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        RESULT_VARIABLE isDirty
    )

    # Create a Revision-String that we can use
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" rev-parse --short=12 HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE revHash
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    # Append dirty marker to commit hash
    if(isDirty)
        set(revHash "${revHash}+")
    endif()

    # And grab the commits timestamp
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" show -s --format=%ci
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE revDate
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    # also retrieve branch name
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" symbolic-ref -q --short HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE revBranch
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    #  when ran on CI, repository is put in detached HEAD state, attempt to scan for known local branches
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" for-each-ref --points-at=HEAD refs/heads "--format=%(refname:short)"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE revBranch
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    # if local branch scan didn't find anything, try remote branches
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" for-each-ref --points-at=HEAD refs/remote "--format=%(refname:short)"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE revBranch
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if(NOT revBranch)
        set(revBranch ${revHash})
    endif()

    string(REGEX MATCH "^[^ \t\r\n]+" rev_branch ${revBranch})

    if(NOT revHash)
        set(revDate "2020-01-01 00:00:00 +0000")
        set(revHash "Unknown")
        set(revBranch "Archived")
    endif()
endif()

if(NOT "${revHash_cached}" STREQUAL "${revHash}" OR NOT "${revBranch_cached}" STREQUAL "${revBranch}" OR NOT EXISTS "${BUILDDIR}/RevisionData.hpp")
configure_file(
    "${CMAKE_SOURCE_DIR}/RevisionData.hpp.in.cmake"
    "${BUILDDIR}/RevisionData.hpp"
    @ONLY
  )

    set(revHash_cached "${revHash}" CACHE INTERNAL "Cached commit-hash")
    set(revBranch_cached "${revBranch}" CACHE INTERNAL "Cached branch name")
endif()
