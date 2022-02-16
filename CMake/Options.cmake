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

option(SERVERS "Build the Servers." 1)
option(TOOLS "Build the Tools." 1)
option(USE_COREPCH "Use Core Precompiled Headers." 0)
option(WITH_DYNAMIC_LINKING "Use Dynamic Library Linking." 0)
if(WITH_DYNAMIC_LINKING AND WITH_DYNAMIC_LINKING_FORCED)
    set(WITH_DYNAMIC_LINKING_FORCED OFF)
endif()
if(WITH_DYNAMIC_LINKING OR WITH_DYNAMIC_LINKING_FORCED)
    set(BUILD_SHARED_LIBS ON)
else()
    set(BUILD_SHARED_LIBS OFF)
endif()
option(WITH_WARNINGS "Show all warnings during compiling" 1)
option(WITH_CORE_DEBUG "Include additional debug-code in core." 1)
option(COPY_CONF "Copy LoginServer and WorldServer .conf.dist to project Dir." 1)