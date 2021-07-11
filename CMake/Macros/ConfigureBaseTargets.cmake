add_library(project-waaagh-compile-option-interface INTERFACE)
add_library(project-waaagh-feature-interface INTERFACE)
add_library(project-waaagh-warning-interface INTERFACE)

# An interface used for all ohter interfaces
add_library(project-waaagh-default-interface INTERFACE)
target_link_libraries(project-waaagh-default-interface
    INTERFACE
        project-waaagh-compile-option-interface
        project-waaagh-feature-interface)

add_library(project-waaagh-dependency-interface INTERFACE)
target_link_libraries(project-waaagh-dependency-interface
    INTERFACE 
        project-waaagh-default-interface
        project-waaagh-warning-interface)

add_library(project-waaagh-no-warning-interface INTERFACE)


add_library(project-waaagh-core-interface INTERFACE)
target_link_libraries(project-waaagh-core-interface
    INTERFACE 
        project-waaagh-default-interface 
        project-waaagh-warning-interface)