cmake_minimum_required(VERSION 2.8)
project(http2)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin)

find_package(Boost)
message ( STATUS "${BOOST_ROOT}")
if( NOT Boost_FOUND)
    message ( FATAL_ERROR "Boost not found")
endif()

if(UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

add_subdirectory(base)
add_subdirectory(net)
add_subdirectory(http)
add_subdirectory(http2)