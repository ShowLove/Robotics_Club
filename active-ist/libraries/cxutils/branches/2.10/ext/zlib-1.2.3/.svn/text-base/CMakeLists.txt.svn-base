# We must specify a minimum version, current min is 2.8
IF(WIN32)
	# Require 2.8.2 for Debug/Release configurations in VS
	cmake_minimum_required (VERSION 2.8.2) 
ELSE()
	cmake_minimum_required (VERSION 2.8)
ENDIF(WIN32)

# Use path relative to cmake build dir
if(COMMAND cmake_policy)
	if(POLICY CMP0015)
		cmake_policy(SET CMP0015 NEW)
	endif(POLICY CMP0015)
endif(COMMAND cmake_policy)

# Script to setup output directories.
include(../../../../SetOutputDirectories.cmake)

# Create a solution/workspace name and a specific library/binary name.
set(ZLIB_PROJECT_NAME zlib)
if(MSVC)
	set(ZLIB_LIB_NAME lib${ZLIB_PROJECT_NAME})
else()
	set(ZLIB_LIB_NAME ${ZLIB_PROJECT_NAME})
endif(MSVC)

# Name the project (for dependancy)
project (${ZLIB_PROJECT_NAME})

# Add include directories for header files.
include_directories(./)
set(ZLIB_SRC_FOLDER ./)

if(MSVC)
add_definitions(/D_CRT_SECURE_NO_WARNINGS /wd4996 /DUNICODE /D_UNICODE)
endif(MSVC)

if(UNIX)
# Ensure using gcc for our code
set(CMAKE_C_COMPILER gcc)
# Add fPIC to support 64 bit building issues
add_definitions(-fPIC)
endif(UNIX)

set (ZLIB_SRC_FILES 
${ZLIB_SRC_FOLDER}/zutil.c
${ZLIB_SRC_FOLDER}/uncompr.c
${ZLIB_SRC_FOLDER}/trees.c
${ZLIB_SRC_FOLDER}/minigzip.c
${ZLIB_SRC_FOLDER}/inftrees.c
${ZLIB_SRC_FOLDER}/inflate.c
${ZLIB_SRC_FOLDER}/inffast.c
${ZLIB_SRC_FOLDER}/infback.c
${ZLIB_SRC_FOLDER}/gzio.c
${ZLIB_SRC_FOLDER}/deflate.c
${ZLIB_SRC_FOLDER}/crc32.c
${ZLIB_SRC_FOLDER}/compress.c
${ZLIB_SRC_FOLDER}/adler32.c
)

# Build a static library
add_library(${ZLIB_LIB_NAME} STATIC ${ZLIB_SRC_FILES})

# Other libraries to link with (e.g. any dependencies).
target_link_libraries(${ZLIB_LIB_NAME} ${CMAKE_REQUIRED_LIBRARIES})
# Add an _d when building debug version
set_target_properties(${ZLIB_LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")
