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
include(../../../../../../SetOutputDirectories.cmake)
include(../../../../../../FindACTIVE.cmake)
include(../../../../../../FindOpenCV.cmake)

# Try add CxUtils.
ACTIVE_ADD_SUBDIRECTORY(CxUtils ./libcxutils)
			
if(CxUtils_FOUND)
	if(CxUtils_SET_DEPENDENCY)
		set(DEPENDENCIES 
				${DEPENDENCIES} 
				${CxUtils_DEPENDENCY})
	endif(CxUtils_SET_DEPENDENCY)
	include_directories(${CxUtils_INCLUDE_DIRS})
	set(EXT_LIBS ${EXT_LIBS} ${CxUtils_LIBRARIES})
endif()

set(DEPENDENCIES ${DEPENDENCIES}
	${LIB_PREFIX}cvcapture)

# Setup libraries for linking
set(EXT_LIBS ${EXT_LIBS}
	${LIB_PREFIX}cvcapture)
	
#message(STATUS ${DEPENDENCIES})

if(OpenCV_FOUND)
        set(EXT_LIBS ${EXT_LIBS}
				${OpenCV_LIBRARIES})
	link_directories(${OpenCV_LIBRARY_DIRS})
endif(OpenCV_FOUND)
# If Basler Pylon library found
# include support for it
if(Pylon_FOUND)
        include_directories(${Pylon_INCLUDE_DIRS})
        link_directories(${Pylon_LIBRARY_DIRS})
        #message(STATUS ${Pylon_LIBRARY_DIRS})
        set(LIB_HEADERS ${LIB_HEADERS} ${LIB_INCLUDE_DIR}/imagecapturepylon.h)
        set(LIB_SOURCES ${LIB_SOURCES} ${LIB_SRC_DIR}/imagecapturepylon.cpp)
        set(EXT_LIBS ${EXT_LIBS} ${Pylon_LIBRARIES})
        add_definitions(${Pylon_CXX_FLAGS})
        #message(STATUS ${Pylon_CXX_FLAGS})
        message(STATUS "CvImageCapture - Basler Pylon Library Found")
else(Pylon_FOUND)
        message(STATUS "CvImageCapture - Basler Pylon Library NOT FOUND, skipping support")
endif(Pylon_FOUND)
# List of example programs
set(EXAMPLES 
	example_cvcapture)
			
# Set variable name of input directories for code.
set(EXAMPLE_INCLUDE_DIR ../../../include/examples)
set(EXAMPLE_SRC_DIR ../../../src/examples)

# Add include paths required for this project
include_directories(../../../include/)
			  

foreach(EXAMPLE ${EXAMPLES})
	# Application
	add_executable(${EXAMPLE} ${EXAMPLE_SRC_DIR}/${EXAMPLE}.cpp)
	target_link_libraries(${EXAMPLE} ${CMAKE_REQUIRED_LIBRARIES})
	# Additional dependecies.		
	target_link_libraries(${EXAMPLE}  
				${EXT_LIBS})
	# Setup dependencies and build order.
	add_dependencies(${EXAMPLE} ${DEPENDENCIES})	
	# Add Debug postfix
	set_target_properties(${EXAMPLE} PROPERTIES DEBUG_POSTFIX "_d")
endforeach(EXAMPLE)

# Setup any additional compiler flags.
if(WIN32)
	if(MSVC)
		# Setup to use UNICODE for MSVC, and disable warnings for
		# use of s_* standard C functions.
		add_definitions(-D_CRT_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE)
		# Add support for parallel compilation
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	endif(MSVC)
	if(UNIX AND NOT MINGW)
		# Ensure using gcc for our code
		set(CMAKE_C_COMPILER gcc)
		# Add fPIC to support 64 bit building issues
		add_definitions(-fPIC)
	endif(UNIX AND NOT MINGW)
endif(WIN32)
