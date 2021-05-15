cmake_minimum_required(VERSION 3.5)

project(JINFENG)

set(CMAKE_CXX_FLAGS "-std=c++11")

aux_source_directory(src SOURCE)

include_directories(src)

find_library(PTHREAD pthread)

add_executable(test_logging tests/test_logging.cc ${SOURCE})
add_executable(test_thread tests/test_thread.cc ${SOURCE})

target_link_libraries(test_logging ${PTHREAD})
target_link_libraries(test_thread ${PTHREAD})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)



