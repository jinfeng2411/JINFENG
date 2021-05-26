cmake_minimum_required(VERSION 3.5)

project(query)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(Protobuf REQUIRED)

if(PROTOBUF_FOUND)
	message("Protobuf found")
else()
	message(FATAL_ERROR "cannot find Protobuf")
endif()

include_directories(. SOURCE)
include_directories(../src SOURCE)

aux_source_directory(. SOURCE)
aux_source_directory(../src SOURCE)

add_executable(test_queryServer tests/test_queryServer.cc ${SOURCE})
add_executable(test_queryClient tests/test_queryClient.cc ${SOURCE})

target_link_libraries(test_queryServer ${PTHREAD} ${PROTOBUF_LIBRARIES})
target_link_libraries(test_queryClient ${PTHREAD} ${PROTOBUF_LIBRARIES})


