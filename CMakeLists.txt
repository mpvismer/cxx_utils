# Generic cmake to build all the cpp_utils

# Apply policies and behaviours as of 3.15 onwards
cmake_minimum_required(VERSION 3.15)

project(
  cxx_utils 
  VERSION 0.0.0
  LANGUAGES CXX) # languages needed to build the project

message("cxx_utils - a code collection")
#("The source path is: ${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_LINKER_LANGUAGE CXX) # Help cmake to figure this out and reduce warnings

# Set paths
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src/")
set(EXAMPLE_DIR "${PROJECT_SOURCE_DIR}/examples/")
set(EXT_DIR "${PROJECT_SOURCE_DIR}/ext/")

include_directories(
  ${SRC_DIR} 
  "${EXT_DIR}/spdlog/include")

add_executable(logging
  "${EXAMPLE_DIR}/logger.cpp"
  "${SRC_DIR}/logger.hpp")
# CMake cannot figure this out - why!?
set_target_properties(logging PROPERTIES LINKER_LANGUAGE CXX)

add_executable(udpserver
  "${EXAMPLE_DIR}/udpserver.cpp"
  "${SRC_DIR}/udpserver.hpp"
  "${SRC_DIR}/utils.hpp"
  "${SRC_DIR}/logger.hpp")  
set_target_properties(udpserver PROPERTIES LINKER_LANGUAGE CXX)

if(WIN32)
  target_sources(udpserver PRIVATE "${SRC_DIR}/utils_win32.cpp")
  target_link_libraries(udpserver ws2_32)
endif(WIN32)

add_executable(realtime
  "${EXAMPLE_DIR}/realtime.cpp"
  "${SRC_DIR}/utils.hpp"
  "${SRC_DIR}/logger.hpp")  
set_target_properties(realtime PROPERTIES LINKER_LANGUAGE CXX)

if(WIN32)
  target_sources(realtime PRIVATE "${SRC_DIR}/utils_win32.cpp")
endif(WIN32)