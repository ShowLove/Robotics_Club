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
FIND_PATH(ACTIVE_MACROS_ROOT_DIR SetOutputDirectories.cmake
			PATHS
				../../../../../
				../../../../../../)
include(${ACTIVE_MACROS_ROOT_DIR}/SetOutputDirectories.cmake)

# Create a solution/workspace name and a specific library/binary name.
set(CXJPEG_PROJECT_NAME cxjpeg-6b)
if(MSVC)
	set(CXJPEG_LIB_NAME lib${CXJPEG_PROJECT_NAME})
else()
	set(CXJPEG_LIB_NAME ${CXJPEG_PROJECT_NAME})
endif(MSVC)

# Name the project (for dependancy)
project (${CXJPEG_PROJECT_NAME})

if(WIN32)
	# Add include directories for header files.
	include_directories(./msvc)
	# Set name of source folder.
	set(CXJPEG_SRC_FOLDER ./msvc)
	if(MSVC)
		add_definitions("/D_CRT_SECURE_NO_WARNINGS /wd4267" )
	endif(MSVC)
else(UNIX)
	# Add include directories for header files.
	include_directories(./linux)
	# Set name of source folder.
	set(CXJPEG_SRC_FOLDER ./linux)
	# Ensure using gcc for our code
	set(CMAKE_C_COMPILER gcc)
	# Add fPIC to support 64 bit building issues
	add_definitions(-fPIC)
endif(WIN32)

set (CXJPEG_SRC_FILES 
${CXJPEG_SRC_FOLDER}/jcapimin.c
${CXJPEG_SRC_FOLDER}/jcapistd.c
${CXJPEG_SRC_FOLDER}/jctrans.c
${CXJPEG_SRC_FOLDER}/jcparam.c
${CXJPEG_SRC_FOLDER}/jdatadst.c
${CXJPEG_SRC_FOLDER}/jcinit.c
${CXJPEG_SRC_FOLDER}/jcmaster.c
${CXJPEG_SRC_FOLDER}/jcmarker.c
${CXJPEG_SRC_FOLDER}/jcmainct.c
${CXJPEG_SRC_FOLDER}/jcprepct.c
${CXJPEG_SRC_FOLDER}/jccoefct.c
${CXJPEG_SRC_FOLDER}/jccolor.c
${CXJPEG_SRC_FOLDER}/jcsample.c
${CXJPEG_SRC_FOLDER}/jchuff.c
${CXJPEG_SRC_FOLDER}/jcphuff.c
${CXJPEG_SRC_FOLDER}/jcdctmgr.c
${CXJPEG_SRC_FOLDER}/jfdctfst.c
${CXJPEG_SRC_FOLDER}/jfdctflt.c
${CXJPEG_SRC_FOLDER}/jfdctint.c
${CXJPEG_SRC_FOLDER}/jdapimin.c
${CXJPEG_SRC_FOLDER}/jdapistd.c
${CXJPEG_SRC_FOLDER}/jdtrans.c
${CXJPEG_SRC_FOLDER}/jdatasrc.c
${CXJPEG_SRC_FOLDER}/jdmaster.c
${CXJPEG_SRC_FOLDER}/jdinput.c
${CXJPEG_SRC_FOLDER}/jdmarker.c
${CXJPEG_SRC_FOLDER}/jdhuff.c
${CXJPEG_SRC_FOLDER}/jdphuff.c
${CXJPEG_SRC_FOLDER}/jdmainct.c
${CXJPEG_SRC_FOLDER}/jdcoefct.c
${CXJPEG_SRC_FOLDER}/jdpostct.c
${CXJPEG_SRC_FOLDER}/jddctmgr.c
${CXJPEG_SRC_FOLDER}/jidctfst.c
${CXJPEG_SRC_FOLDER}/jidctflt.c
${CXJPEG_SRC_FOLDER}/jidctint.c
${CXJPEG_SRC_FOLDER}/jidctred.c
${CXJPEG_SRC_FOLDER}/jdsample.c
${CXJPEG_SRC_FOLDER}/jdcolor.c
${CXJPEG_SRC_FOLDER}/jquant1.c
${CXJPEG_SRC_FOLDER}/jquant2.c
${CXJPEG_SRC_FOLDER}/jdmerge.c
${CXJPEG_SRC_FOLDER}/jcomapi.c
${CXJPEG_SRC_FOLDER}/jutils.c
${CXJPEG_SRC_FOLDER}/jerror.c
${CXJPEG_SRC_FOLDER}/jmemmgr.c
)

# Add extra file needed for unix build
if(WIN32)
	set(CXJPEG_SRC_FILES ${CXJPEG_SRC_FILES} ${CXJPEG_SRC_FOLDER}/jmemansi.c)
else()
	set(CXJPEG_SRC_FILES ${CXJPEG_SRC_FILES} ${CXJPEG_SRC_FOLDER}/jmemnobs.c)
endif(WIN32)

# Build a static library
add_library(${CXJPEG_LIB_NAME} STATIC ${CXJPEG_SRC_FILES})

# Other libraries to link with (e.g. any dependencies).
target_link_libraries(${CXJPEG_LIB_NAME} ${CMAKE_REQUIRED_LIBRARIES})
# Add an _d when building debug version
set_target_properties(${CXJPEG_LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")
# Solution folders.
if(USE_SOLUTION_FOLDERS)
	SET_PROPERTY(TARGET ${CXJPEG_LIB_NAME} PROPERTY FOLDER "Libraries")
endif()