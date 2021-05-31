cmake_minimum_required(VERSION 3.5)

project(httpServer)

set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(../src SOURCE)
include_directories(. SOURCE)

aux_source_directory(../src SOURCE)
aux_source_directory(. SOURCE)

find_library(PTHREAD pthread)

add_executable(httpServer tests/test_httpServer.cc ${SOURCE})

target_link_libraries(httpServer ${PTHREAD})


