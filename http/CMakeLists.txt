cmake_minimum_required(VERSION 2.8)
project(http)

set(BIN_NAME http)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${ROOT_DIR}/base/include
	${ROOT_DIR}/net/include
    ${Boost_INCLUDE_DIRS}
)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HttpReply.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/HttpRequestHandler.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/HttpRequestParser.cpp
)

# add for see in solution
if(WIN32)
set(SOURCE_FILES
	${SOURCE_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/HttpReply.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/HttpRequestHandler.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/HttpRequestParser.hpp
)
endif()

add_library(${BIN_NAME}
    ${SOURCE_FILES}
)
