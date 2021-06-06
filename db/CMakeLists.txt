cmake_minimum_required(VERSION 3.5)

project(test_redis)

set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(. SOURCE)

aux_source_directory(. SOURCE)

find_library(REDIS hiredis)

add_executable(test_redis tests/test_redis.cc ${SOURCE})

target_link_libraries(test_redis ${REDIS})


