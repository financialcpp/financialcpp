cmake_minimum_required(VERSION 3.18)

# Name of the project (will be the name of the plugin)
project(financialcpp VERSION 0.0.1
DESCRIPTION "Financialcpp - High performance stock trading platform"
HOMEPAGE_URL "https://financialcpp.com"
)




# this is the logical target of the library
add_library("${PROJECT_NAME}-lib" SHARED
"${CMAKE_SOURCE_DIR}/cpp/lib/src/financial.hpp"
)

set_target_properties("${PROJECT_NAME}-lib" PROPERTIES 
CXX_STANDARD 20
LINKER_LANGUAGE CXX
COMPILE_FLAGS "-pthread"
)

# note: we use SYSTEM PUBLIC instead of PUBLIC to suppress warnings coming from 
# hana lib. cmake will turn this include into a "system" include, and compilers
# do not issue warnings coming from "system" includes
# namely: in c++20, std::is_pod is deprecated but hana uses it
target_include_directories("${PROJECT_NAME}-lib" SYSTEM INTERFACE "${CMAKE_SOURCE_DIR}/cpp/lib/external/hana/include")


# mio
target_include_directories("${PROJECT_NAME}-lib" INTERFACE "${CMAKE_SOURCE_DIR}/cpp/lib/external/mio/include")

# readwriterqueue
target_include_directories("${PROJECT_NAME}-lib" INTERFACE "${CMAKE_SOURCE_DIR}/cpp/lib/external/readwriterqueue/include")

# activetick - Includes
target_include_directories("${PROJECT_NAME}-lib" INTERFACE "${CMAKE_SOURCE_DIR}/cpp/lib/external/activetick/include/ActiveTickServerAPI")
target_include_directories("${PROJECT_NAME}-lib" INTERFACE "${CMAKE_SOURCE_DIR}/cpp/lib/external/activetick/include")

# activetick - Wrapper/Examples - they included some nice examples, I've just edited them a bit
target_include_directories("${PROJECT_NAME}-lib" INTERFACE "${CMAKE_SOURCE_DIR}/cpp/lib/external/activetick/src")


# Build a shared library named after the project
# Keep addon and source separate if we end up splitting the two
add_library(${PROJECT_NAME} SHARED
# Addon sources
"${CMAKE_SOURCE_DIR}/cpp/addon/src/financial.h"
"${CMAKE_SOURCE_DIR}/cpp/addon/src/financial.cpp"

# activetick sources
"${CMAKE_SOURCE_DIR}/cpp/lib/external/activetick/src/Helper.cpp"
"${CMAKE_SOURCE_DIR}/cpp/lib/external/activetick/src/Requestor.cpp"
"${CMAKE_SOURCE_DIR}/cpp/lib/external/activetick/src/Session.cpp"
"${CMAKE_SOURCE_DIR}/cpp/lib/external/activetick/src/Streamer.cpp"
"${CMAKE_SOURCE_DIR}/cpp/lib/external/activetick/src/calendar.cpp"
)

# financialcpp library
target_link_libraries(${PROJECT_NAME} PUBLIC "${PROJECT_NAME}-lib")

# activetick
target_link_libraries(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/cpp/lib/external/activetick/bin/libActiveTickServerAPI.so")

#  stdc++fs pthread

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES 
CXX_STANDARD 20
PREFIX ""
SUFFIX ".node"
)


# These variables are set globally by cmakejs
# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

# Include N-API wrappers
# -p: prints script to stdout 
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})


