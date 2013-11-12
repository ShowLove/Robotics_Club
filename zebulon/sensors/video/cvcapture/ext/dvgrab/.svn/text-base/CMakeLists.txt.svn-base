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
set(LIB_PROJECT_NAME dvgrab)
# Source/Directories to sort.
set(SOURCE_GROUPS "")
# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(LIB_NAME ${LIB_PREFIX}${LIB_PROJECT_NAME})

# Add include directories for header files.
include_directories(./)

# Set variable name of input directories for code.
set(LIB_INCLUDE_DIR ./)
set(LIB_SRC_DIR ./)

# Add paths to directories containing external dependencies we need.
link_directories(./)

# Setup any additional compiler flags.

# Get header files.
set(LIB_HEADERS 
		${LIB_INCLUDE_DIR}/affine.h
		${LIB_INCLUDE_DIR}/avi.h
		${LIB_INCLUDE_DIR}/config.h
		${LIB_INCLUDE_DIR}/dvframe.h
		${LIB_INCLUDE_DIR}/dvgrab.h
		${LIB_INCLUDE_DIR}/endian_types.h
		${LIB_INCLUDE_DIR}/error.h
		${LIB_INCLUDE_DIR}/filehandler.h
		${LIB_INCLUDE_DIR}/frame.h
		${LIB_INCLUDE_DIR}/hdvframe.h
		${LIB_INCLUDE_DIR}/iec13818-1.h
		${LIB_INCLUDE_DIR}/iec13818-2.h
		${LIB_INCLUDE_DIR}/ieee1394io.h
		${LIB_INCLUDE_DIR}/io.h
		${LIB_INCLUDE_DIR}/raw1394util.h
		${LIB_INCLUDE_DIR}/riff.h
		${LIB_INCLUDE_DIR}/smiltime.h
		${LIB_INCLUDE_DIR}/srt.h
		${LIB_INCLUDE_DIR}/stringutils.h
		${LIB_INCLUDE_DIR}/v4l2reader.h)
set(LIB_SOURCES
		${LIB_SRC_DIR}/avi.cc
		${LIB_SRC_DIR}/dvframe.cc
		${LIB_SRC_DIR}/error.cc
		${LIB_SRC_DIR}/filehandler.cc
		${LIB_SRC_DIR}/frame.cc
		${LIB_SRC_DIR}/hdvframe.cc
		${LIB_SRC_DIR}/iec13818-1.cc
		${LIB_SRC_DIR}/iec13818-2.cc
		${LIB_SRC_DIR}/ieee1394io.cc
		${LIB_SRC_DIR}/io.c
		${LIB_SRC_DIR}/raw1394util.c
		${LIB_SRC_DIR}/riff.cc
		${LIB_SRC_DIR}/smiltime.cc
		${LIB_SRC_DIR}/srt.cc
		${LIB_SRC_DIR}/stringutils.cc
		${LIB_SRC_DIR}/v4l2reader.cc)

#message(STATUS ${LIB_SOURCES})

# Build a library
add_library(${LIB_NAME} SHARED ${LIB_SOURCES} ${LIB_HEADERS})

if(UNIX)
	# Ensure using gcc for our code
	set(CMAKE_C_COMPILER gcc)
	# Add fPIC to support 64 bit building issues
	add_definitions(-fPIC -DHAVE_LIBDV)
        set(EXTERNAL_1394_LIBRARIES avc1394 dv iec61883 rom1394)

endif(UNIX)

if(WIN32)
	# Not Supported
else()
	# Link symbols
	#set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE C)
	set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS -rdynamic)
endif(WIN32)

# Add an _d when building debug version
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")

# Link against dependencies.
target_link_libraries(${LIB_NAME} ${EXTERNAL_1394_LIBRARIES}
						dv
						avc1394
						iec61883
						rom1394
						dc1394
						${CMAKE_REQUIRED_LIBRARIES})



# Make Source Groups, this makes sub folders in Visual Studio
foreach(GRP ${SOURCE_GROUPS})
	file(GLOB_RECURSE HEADER_GROUP ${LIB_INCLUDE_DIR}/${GRP}/*.h)
	file(GLOB_RECURSE SRC_GROUP ${LIB_SRC_DIR}/${GRP}/*.cpp)
	SOURCE_GROUP("Header Files\\${GRP}" FILES ${HEADER_GROUP})
	SOURCE_GROUP("Source Files\\${GRP}" FILES ${SRC_GROUP})
endforeach(GRP)
