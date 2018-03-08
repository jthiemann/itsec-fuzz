cmake_minimum_required(VERSION 2.8)



set( CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set( ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set( INCLUDE_DIRSQT ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE
   ${SRC_DIR}/main.cpp
   ${SRC_DIR}/canSocket.cpp
   ${SRC_DIR}/filter.cpp
   ${SRC_DIR}/testcases.cpp
   ${SRC_DIR}/util.cpp
)



set(INCLUDE_DIRS
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   /usr/local/include
)

set( CMAKE_CXX_FLAGS "-g -W -Wall -std=c++11 -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

include_directories( ${INCLUDE_DIRS})
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


project(gatewayFuzzer)
add_executable(${PROJECT_NAME} ${SOURCE})
