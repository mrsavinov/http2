cmake_minimum_required(VERSION 2.8)
project(http2-lib)

set(BIN_NAME http2-lib)
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${ROOT_DIR}/base/include
	${ROOT_DIR}/net/include
	${ROOT_DIR}/http/include
)

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

set(SOURCE_FILES
	src/Http2RequestHandler.cpp
)

# add for see in solution
if(WIN32)
set(SOURCE_FILES
	${SOURCE_FILES}
    include/Http2Types.hpp
	include/Http2RequestHandler.hpp
)
endif()

add_library(${BIN_NAME}
    ${SOURCE_FILES}
)