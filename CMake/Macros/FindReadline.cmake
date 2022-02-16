find_path(READLINE_INCLUDE_DIR
  NAMES
    readline/readline.h
  HINTS
    ${READLINE_ROOT_DIR}
    ENV READLINE_ROOT_DIR
  PATH_SUFFIXES
    include)

find_library(READLINE_LIBRARY
  NAMES
    readline
  HINTS
    ${READLINE_ROOT_DIR}
    ENV READLINE_ROOT_DIR
  PATH_SUFFIXES
    lib)

if(READLINE_INCLUDE_DIR AND EXISTS "${READLINE_INCLUDE_DIR}/readline/readline.h")
  file(STRINGS "${READLINE_INCLUDE_DIR}/readline/readline.h" readline_major
    REGEX "^#[\t ]*define[\t ]+RL_VERSION_MAJOR[\t ]+([0-9])+.*")
  file(STRINGS "${READLINE_INCLUDE_DIR}/readline/readline.h" readline_minor
    REGEX "^#[\t ]*define[\t ]+RL_VERSION_MINOR[\t ]+([0-9])+.*")
  if (readline_major AND readline_minor)
    string(REGEX REPLACE "^.*RL_VERSION_MAJOR[\t ]+([0-9])+.*$"
           "\\1" readline_major "${readline_major}")
    string(REGEX REPLACE "^.*RL_VERSION_MINOR[\t ]+([0-9])+.*$"
           "\\1" readline_minor "${readline_minor}")
    set(READLINE_VERSION "${readline_major}.${readline_minor}")
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Readline
  REQUIRED_VARS
    READLINE_LIBRARY
    READLINE_INCLUDE_DIR
  VERSION_VAR
    READLINE_VERSION
)

mark_as_advanced(READLINE_FOUND READLINE_LIBRARY READLINE_INCLUDE_DIR)

if(READLINE_FOUND)
  message(STATUS "Found Readline library: ${READLINE_LIBRARY}")
  message(STATUS "Found Readline headers: ${READLINE_INCLUDE_DIR}")

  if (NOT TARGET Readline::Readline)
    add_library(Readline::Readline UNKNOWN IMPORTED)
    set_target_properties(Readline::Readline
      PROPERTIES
        IMPORTED_LOCATION
          "${READLINE_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES
          "${READLINE_INCLUDE_DIR}")
  endif()
endif()