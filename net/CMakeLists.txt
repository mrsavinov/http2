cmake_minimum_required(VERSION 2.8)
project(net)

set(BIN_NAME net)


set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${ROOT_DIR}/base/include
    ${Boost_INCLUDE_DIRS}
)
LINK_DIRECTORIES(
	${Boost_LIBRARY_DIRS}
)

set(SOURCE_FILES
    src/Connection.cpp
	src/ConnectionManager.cpp
	src/Server.cpp
)

# add for see in solution
if(WIN32)
set(SOURCE_FILES
	${SOURCE_FILES}
    include/Connection.hpp
	include/ConnectionManager.hpp
	include/RequestHandler.hpp
	include/Server.hpp
	include/Socket.hpp
	include/NetTypes.hpp
	#include/RequestHandlerFactory.hpp
)
endif()

add_library(${BIN_NAME}
    ${SOURCE_FILES}
)
