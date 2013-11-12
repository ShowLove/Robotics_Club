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

# Set build dependencies.  These projects will get built
# before this file.
set(DEPENDENCIES 
		${LIB_PREFIX}microstrain
		${LIB_PREFIX}compass)
if(CxUtils_SET_DEPENDENCY)
	set(DEPENDENCIES 
			${DEPENDENCIES} 
			${CxUtils_DEPENDENCY})
endif(CxUtils_SET_DEPENDENCY)

# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(EXT_LIBS 
	${DEPENDENCIES}
	${CxUtils_LIBRARIES})
	
# List of example programs
set(EXAMPLES 
			example_microstrain)
			
# Set variable name of input directories for code.
set(EXAMPLE_INCLUDE_DIR ../../../include/examples)
set(EXAMPLE_SRC_DIR ../../../src/examples)

# Add include paths required for this project
include_directories(../../../include/
					../../../../abstract/include)

# Add library paths required for this project.
link_directories(../../../lib/)
			  
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
