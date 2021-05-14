cmake_minimum_required(VERSION 3.5)

project(JINFENG)

aux_source_directory(src SOURCE)

include_directories(src)

add_executable(test_logging tests/test_logging.cc ${SOURCE})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)



