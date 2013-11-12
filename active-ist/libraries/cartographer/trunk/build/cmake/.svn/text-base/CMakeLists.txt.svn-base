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

# Create a solution/workspace name and a specific library/binary name.
set(CARTOGRAPHER_PROJECT_NAME cartographer)

# Name the project (for dependancy)
project (${CARTOGRAPHER_PROJECT_NAME})

include(../../../../SetOutputDirectories.cmake)
include(../../../../FindACTIVE.cmake)
include(../../../../FindOpenCV.cmake)

# Add external dependecies we have projects for.# Add external dependecies we have projects for.
# Adds the CxUtils CMakeLists.txt file if available.
ACTIVE_ADD_SUBDIRECTORY(CxUtils ./libcxutils)
ACTIVE_ADD_SUBDIRECTORY(TinyXML ./libtinyxml)

add_subdirectory(libcartographer ./libcartographer)
#add_subdirectory(examples ../cmake.cartographer.examples)
if(OpenCV_FOUND)
	#MESSAGE(STATUS "OpenCV Found")
	OPTION(CARTOGRAPHER_USE_OPENCV "Add OPENCV visualization" TRUE)
	if(CARTOGRAPHER_USE_OPENCV)
		add_definitions(-DUSE_OPENCV)
	endif(CARTOGRAPHER_USE_OPENCV)
endif(OpenCV_FOUND)
OPTION(BUILD_CARTOGRAPHER_EXAMPLES "Build tutorial" TRUE)

if(BUILD_CARTOGRAPHER_EXAMPLES)
add_subdirectory(examples ./cartographer.examples)
endif()

