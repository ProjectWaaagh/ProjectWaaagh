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

# An interface library to make the target com available to other targets
add_library(projectWaaagh-compile-option-interface INTERFACE)

# Use -std=c++11 instead of -std=gnu++11
set(CXX_EXTENSIONS OFF)

# An interface library to make the target features available to other targets
add_library(projectWaaagh-feature-interface INTERFACE)

target_compile_features(projectWaaagh-feature-interface
  INTERFACE
    cxx_std_17
    cxx_alias_templates
    cxx_auto_type
    cxx_constexpr
    cxx_decltype
    cxx_decltype_auto
    cxx_final
    cxx_lambdas
    cxx_generic_lambdas
    cxx_variadic_templates
    cxx_defaulted_functions
    cxx_nullptr
    cxx_trailing_return_types
    cxx_return_type_deduction)

# An interface library to make the warnings level available to other targets
# This interface taget is set-up through the platform specific script
add_library(projectWaaagh-warning-interface INTERFACE)

# An interface used for all other interfaces
add_library(projectWaaagh-default-interface INTERFACE)
target_link_libraries(projectWaaagh-default-interface
  INTERFACE
    projectWaaagh-compile-option-interface
    projectWaaagh-feature-interface)

# An interface used for silencing all warnings
add_library(projectWaaagh-no-warning-interface INTERFACE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(projectWaaagh-no-warning-interface
    INTERFACE
      /W0)
else()
  target_compile_options(projectWaaagh-no-warning-interface
    INTERFACE
      -w)
endif()

# An interface library to change the default behaviour
# to hide symbols automatically.
add_library(projectWaaagh-hidden-symbols-interface INTERFACE)

# An interface amalgamation which provides the flags and definitions
# used by the dependency targets.
add_library(projectWaaagh-dependency-interface INTERFACE)
target_link_libraries(projectWaaagh-dependency-interface
  INTERFACE
    projectWaaagh-default-interface
    projectWaaagh-no-warning-interface
    projectWaaagh-hidden-symbols-interface)

# An interface amalgamation which provides the flags and definitions
# used by the core targets.
add_library(projectWaaagh-core-interface INTERFACE)
target_link_libraries(projectWaaagh-core-interface
  INTERFACE
    projectWaaagh-default-interface
    projectWaaagh-warning-interface)