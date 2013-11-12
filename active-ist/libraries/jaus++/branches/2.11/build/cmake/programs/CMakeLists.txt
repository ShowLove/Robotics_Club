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
find_path(ACTIVE_MACROS_ROOT_DIR SetOutputDirectories.cmake
			PATHS
				../../../../
				../../../../../
				../../../../../../)
include(${ACTIVE_MACROS_ROOT_DIR}/SetOutputDirectories.cmake)
include(${ACTIVE_MACROS_ROOT_DIR}/FindACTIVE.cmake)

ACTIVE_ADD_SUBDIRECTORY(CxUtils ./libcxutils)
ACTIVE_ADD_SUBDIRECTORY(TinyXML ./libtinyxml)
#ACTIVE_ADD_SUBDIRECTORY(JAUS ./libjaus)

# Set build dependencies.  These projects will get built
# before this file.
if(CxUtils_SET_DEPENDENCY)
	set(DEPENDENCIES 
			${CxUtils_DEPENDENCY})
endif(CxUtils_SET_DEPENDENCY)

if(JAUS_SET_DEPENDENCY)
	set(DEPENDENCIES 
			${DEPENDENCIES} 
			${JAUS_DEPENDENCY})
endif(JAUS_SET_DEPENDENCY)

# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(EXT_LIBS 
		${DEPENDENCIES}
		${CxUtils_LIBRARIES}
		${JAUS_LIBRARIES})

# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(EXT_LIBS 
	${DEPENDENCIES})
	
# List of example programs
set(CONSOLE_PROGRAMS 
			console_joystick
                        console_keyboard
                        jcopc)
			
# Set variable name of input directories for code.
set(PROGRAM_INCLUDE_DIR ../../../include/programs)
set(PROGRAM_SRC_DIR ../../../src/programs)

# Add include paths required for this project
include_directories(../../../include/
					${TinyXML_INCLUDE_DIRS}
					${CxUtils_INCLUDE_DIRS})

# Add library paths required for this project.
link_directories(../../../lib/)
			  
foreach(CONSOLE_PROGRAM ${CONSOLE_PROGRAMS})
	# Application
	add_executable(${CONSOLE_PROGRAM} ${PROGRAM_SRC_DIR}/${CONSOLE_PROGRAM}.cpp)
	target_link_libraries(${CONSOLE_PROGRAM} ${CMAKE_REQUIRED_LIBRARIES})
	# Additional dependecies.		
	target_link_libraries(${CONSOLE_PROGRAM}  
								${EXT_LIBS})

	# Setup dependencies and build order.
	add_dependencies(${CONSOLE_PROGRAM} ${DEPENDENCIES})	
	# Add Debug postfix
	set_target_properties(${CONSOLE_PROGRAM} PROPERTIES DEBUG_POSTFIX "_d")
	# Setup for installers
    if(WIN32)
	    install(TARGETS ${CONSOLE_PROGRAM}
			    DESTINATION bin
			    RUNTIME DESTINATION bin
			    COMPONENT Programs)
	else()
        install(TARGETS ${CONSOLE_PROGRAM}
			    DESTINATION bin
			    RUNTIME DESTINATION bin/active
			    COMPONENT Programs)
    endif(WIN32)
endforeach(CONSOLE_PROGRAM)

#  If wxWidgets is found, build wxWidget applications.
if(MSVC)
	set(wxWidgets_CONFIGURATION mswu)
	set(wxWidgets_USE_REL_AND_DBG 1)
endif(MSVC)

find_package(wxWidgets COMPONENTS core base xml adv)
 
if(wxWidgets_FOUND)
		include(${wxWidgets_USE_FILE})
		# Application
		add_executable(videoclient WIN32
				${PROGRAM_SRC_DIR}/imagepanel.cpp
				${PROGRAM_SRC_DIR}/videoclientframe.cpp
				${PROGRAM_SRC_DIR}/videoclientapp.cpp
				${PROGRAM_INCLUDE_DIR}/imagepanel.h
				${PROGRAM_INCLUDE_DIR}/videoclientframe.h
				)
		target_link_libraries(videoclient ${CMAKE_REQUIRED_LIBRARIES})
		# Additional dependecies.		
		target_link_libraries(videoclient 
									${wxWidgets_LIBRARIES}
									${EXT_LIBS})
		# Setup dependencies and build order.
		add_dependencies(videoclient ${DEPENDENCIES})	
		# Add Debug postfix
		set_target_properties(videoclient PROPERTIES DEBUG_POSTFIX "_d")
		if(WIN32)
		    install(TARGETS videoclient
				    DESTINATION bin
				    RUNTIME DESTINATION bin
				    COMPONENT Programs)
		else()
            install(TARGETS videoclient
				    DESTINATION bin
				    RUNTIME DESTINATION bin/active
				    COMPONENT Programs)
        endif(WIN32)
endif(wxWidgets_FOUND)

# Setup any additional compiler flags.
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
