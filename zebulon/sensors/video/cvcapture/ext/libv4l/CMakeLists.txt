# We must specify a minimum version, current min is 2.8
IF(WIN32)
	# Require 2.8.2 for Debug/Release configurations in VS
	cmake_minimum_required (VERSION 2.8.2) 
ELSE()
	cmake_minimum_required (VERSION 2.8)
ENDIF(WIN32)

# This policy makes sure that all relative 
# paths are relative to this CMakeLists.txt file.
if(COMMAND cmake_policy)
	if(POLICY CMP0015)
		cmake_policy(SET CMP0015 NEW)
	endif(POLICY CMP0015)
endif(COMMAND cmake_policy)

# Script to setup output directories.
include(../../../../../SetOutputDirectories.cmake)

# Set the name to use for the DLL Export Preprocessor
# definition for WIN32
set(DLL_EXPORT_NAME "")
# Set name of this project, and dependencies needed.
# Names are different between MSVC and other build systems, so
# in MSVC add prefix to name(s) as needed.
set(LIB_PROJECT_NAME v4l)
# Source/Directories to sort.
set(SOURCE_GROUPS 	libv4l 
				  	linux
					example)
# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(LIB_NAME ${LIB_PREFIX}${LIB_PROJECT_NAME})

# Add include directories for header files.
include_directories(./ ./linux ./example)

# Set variable name of input directories for code.
set(LIB_INCLUDE_DIR ./)
set(LIB_SRC_DIR ./)

# Add paths to directories containing external dependencies we need.
link_directories(./)

# Setup any additional compiler flags.

if(UNIX)
	# Ensure using gcc for our code
	set(CMAKE_C_COMPILER gcc)
	# Add fPIC to support 64 bit building issues
	add_definitions(-fPIC -DVER_MAJ=0 -DVER_MIN=7 -DVER_REL=0 )
endif(UNIX)

# Get header files.
file(GLOB HEADERS ${LIB_INCLUDE_DIR}/*.h)
set(LIB_HEADERS ${HEADERS})
file(GLOB HEADERS ${LIB_INCLUDE_DIR}/linux/*.h)
set(LIB_HEADERS ${LIB_HEADERS} ${HEADERS})
# Get src files.
file(GLOB SOURCES ${LIB_SRC_DIR}/*.c)
set(LIB_SOURCES ${SOURCES})
file(GLOB SOURCES ${LIB_SRC_DIR}/linux/*.c)
set(LIB_SOURCES ${LIB_SOURCES} ${SOURCES})

foreach(ITEM ${LIB_SOURCES})
	if(${ITEM} MATCHES "test.c")
		LIST(REMOVE_ITEM LIB_SOURCES ${ITEM})
	endif()
	if(${ITEM} MATCHES "main.c")
		LIST(REMOVE_ITEM LIB_SOURCES ${ITEM})
	endif()
endforeach(ITEM)


# Build a library
add_library(${LIB_NAME} SHARED ${LIB_SOURCES} ${LIB_HEADERS})
if(WIN32)
	# Not Supported
else()
	# Link symbols
	set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE C)
	set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS -rdynamic)
endif(WIN32)

# Add an _d when building debug version
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")

# Link against dependencies.
target_link_libraries(${LIB_NAME} 
						${CMAKE_REQUIRED_LIBRARIES})


# Make Source Groups, this makes sub folders in Visual Studio
foreach(GRP ${SOURCE_GROUPS})
	file(GLOB_RECURSE HEADER_GROUP ${LIB_INCLUDE_DIR}/${GRP}/*.h)
	file(GLOB_RECURSE SRC_GROUP ${LIB_SRC_DIR}/${GRP}/*.cpp)
	SOURCE_GROUP("Header Files\\${GRP}" FILES ${HEADER_GROUP})
	SOURCE_GROUP("Source Files\\${GRP}" FILES ${SRC_GROUP})
endforeach(GRP)
