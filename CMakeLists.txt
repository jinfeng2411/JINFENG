cmake_minimum_required(VERSION 3.5)

project(JINFENG)

set(CMAKE_CXX_FLAGS "-std=c++11")

aux_source_directory(src SOURCE)

include_directories(src)

find_library(PTHREAD pthread)
find_library(YAMLCPP yaml-cpp)

add_executable(test_logging tests/test_logging.cc ${SOURCE})
add_executable(test_thread tests/test_thread.cc ${SOURCE})
add_executable(test_threadPool tests/test_threadPool.cc ${SOURCE})
add_executable(test_eventloop tests/test_eventloop.cc ${SOURCE})
add_executable(test_tcpServer tests/test_tcpServer.cc ${SOURCE})
#add_executable(test_buffer tests/test_buffer.cc ${SOURCE})
add_executable(test_tcpClient tests/test_tcpClient.cc ${SOURCE})
add_executable(test_eventLoopThreadPool tests/test_eventLoopThreadPool.cc ${SOURCE})
#add_executable(test_config tests/test_config.cc ${SOURCE})

target_link_libraries(test_logging ${PTHREAD} ${YAMLCPP})
target_link_libraries(test_thread ${PTHREAD} ${YAMLCPP})
target_link_libraries(test_threadPool ${PTHREAD} ${YAMLCPP})
target_link_libraries(test_eventloop ${PTHREAD} ${YAMLCPP})
target_link_libraries(test_tcpServer ${PTHREAD} ${YAMLCPP})
#target_link_libraries(test_buffer ${PTHREAD} ${TAMLCPP})
target_link_libraries(test_tcpClient ${PTHREAD} ${YAMLCPP})
target_link_libraries(test_eventLoopThreadPool ${PTHREAD} ${YAMLCPP})
#target_link_libraries(test_config ${PTHREAD} ${YAMLCPP})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)



