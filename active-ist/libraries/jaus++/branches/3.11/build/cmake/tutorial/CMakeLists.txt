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
FIND_PATH(ACTIVE_MACROS_ROOT_DIR SetOutputDirectories.cmake
			PATHS
				../../../../
				../../../../../
				../../../../../../)
include(${ACTIVE_MACROS_ROOT_DIR}/SetOutputDirectories.cmake)
include(${ACTIVE_MACROS_ROOT_DIR}/FindACTIVE.cmake)

ACTIVE_ADD_SUBDIRECTORY(CxUtils ./libcxutils)
ACTIVE_ADD_SUBDIRECTORY(TinyXML ./libtinyxml)
ACTIVE_ADD_SUBDIRECTORY(JAUS ./libjaus)

set(DEPENDENCIES
		${CxUtils_DEPENDENCY}
		${TinyXML_DEPENDENCY}
		${JAUS_DEPENDENCY}
		)

set(EXT_LIBS 
	${CxUtils_LIBRARIES}
	${TinyXML_LIBRARIES}
	${JAUS_LIBRARIES})
	

# Set variable name of input directories for code.
set(PROGRAM_INCLUDE_DIR ../../../include/tutorial)
set(PROGRAM_SRC_DIR ../../../src/tutorial)

file(GLOB_RECURSE PROGRAMS ${PROGRAM_SRC_DIR}/*.cpp)

# Add include paths required for this project
include_directories(../../../include/
					../../../ext
					${TinyXML_INCLUDE_DIRS}
					${CxUtils_INCLUDE_DIRS})

# Add library paths required for this project.
link_directories(../../../lib/)
			  
foreach(PROGRAM ${PROGRAMS})
	# Application
	
	# Get program name from the file path
	string(REGEX REPLACE "/" ";" PROGRAM_TOKENS ${PROGRAM})
	list(REVERSE PROGRAM_TOKENS)
	list(GET PROGRAM_TOKENS 0 PROGRAM_NAME)
	string(REGEX REPLACE ".cpp" "" PROGRAM_NAME ${PROGRAM_NAME})
	
	add_executable(${PROGRAM_NAME} ${PROGRAM})
	target_link_libraries(${PROGRAM_NAME} ${CMAKE_REQUIRED_LIBRARIES})
	# Additional dependecies.		
	target_link_libraries(${PROGRAM_NAME}  
								${EXT_LIBS})

	# Setup dependencies and build order.
	add_dependencies(${PROGRAM_NAME} ${DEPENDENCIES})	
	# Add Debug postfix
	set_target_properties(${PROGRAM_NAME} PROPERTIES DEBUG_POSTFIX "_d")
	
	# Solution folders.
	if(USE_SOLUTION_FOLDERS)
		SET_PROPERTY(TARGET ${PROGRAM_NAME} PROPERTY FOLDER "JAUS++ Tutorial")
	endif()

endforeach(PROGRAM)

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
