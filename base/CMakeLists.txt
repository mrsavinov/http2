cmake_minimum_required(VERSION 2.8)
project(base)

set(BIN_NAME base)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(SOURCE_FILES
	src/BaseLogger.cpp
	src/Config.cpp
	src/ConfigReaderWriter.cpp
)

if(WIN32)
	set(SOURCE_FILES
		${SOURCE_FILES}
		include/BaseTypes.hpp
		include/BaseLogger.hpp
		include/Buffer.hpp
		include/Config.hpp
		include/ConfigReaderWriter.hpp
	)
endif()


add_library(${BIN_NAME}
	${SOURCE_FILES}
)