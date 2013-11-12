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

# Set build dependencies.  These projects will get built
# before this file.
set(DEPENDENCIES
		${CxUtils_DEPENDENCY}
		${TinyXML_DEPENDENCY}
		${JAUS_DEPENDENCY}
		)

# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(EXT_LIBS 
	${CxUtils_LIBRARIES}
	${TinyXML_LIBRARIES}
	${JAUS_LIBRARIES})

			
# Set variable name of input directories for code.
set(PROGRAM_INCLUDE_DIR ../../../include/programs)
set(PROGRAM_SRC_DIR ../../../src/programs)
set(GUI_INCLUDE_DIR ../../../src/programs/gui/)
set(GUI_SOURCE_DIR ../../../src/programs/gui/)

if(MSVC)
	file(GLOB_RECURSE RESOURCE_FILES ../../../resources/*.rc)
	#message(STATUS ${RESOURCE_FILES})
endif(MSVC)

# Add include paths required for this project
include_directories(../../../include/
					${TinyXML_INCLUDE_DIRS}
					${CxUtils_INCLUDE_DIRS}
					${GUI_INCLUDE_DIR})

# Add library paths required for this project.
link_directories(../../../lib/)

file(GLOB_RECURSE CONSOLE_PROGRAMS ${PROGRAM_SRC_DIR}/console/*.cpp)
			  
foreach(CONSOLE_PROGRAM ${CONSOLE_PROGRAMS})
	# Application
	
	# Get program name from the file path
	string(REGEX REPLACE "/" ";" PROGRAM_TOKENS ${CONSOLE_PROGRAM})
	list(REVERSE PROGRAM_TOKENS)
	list(GET PROGRAM_TOKENS 0 PROGRAM_NAME)
	string(REGEX REPLACE ".cpp" "" PROGRAM_NAME ${PROGRAM_NAME})
	
	if(MSVC)
		set(CONSOLE_PROGRAM ${CONSOLE_PROGRAM} ${RESOURCE_FILES})
	endif(MSVC)
			
	add_executable(${PROGRAM_NAME} ${CONSOLE_PROGRAM})
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
		SET_PROPERTY(TARGET ${PROGRAM_NAME} PROPERTY FOLDER "JAUS++ Programs")
	endif()
	
	# Setup for installers
    if(WIN32)
	    install(TARGETS ${PROGRAM_NAME}
			    DESTINATION bin
			    RUNTIME DESTINATION bin
			    COMPONENT Programs)
	else()
        install(TARGETS ${PROGRAM_NAME}
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

find_package(wxWidgets COMPONENTS net gl core base xml adv)

if(wxWidgets_FOUND)
		
		include(${wxWidgets_USE_FILE})
		include_directories(${GUI_SOURCE_DIR}
							${GUI_INCLUDE_DIR})

		# Dialog Files
		set(JAUSApps 	
				#NodeManager
				)
		
		foreach(App ${JAUSApps})
			# Application
			file(GLOB_RECURSE GUI_SOURCES ${GUI_SOURCE_DIR}/${App}/*.cpp)
			file(GLOB_RECURSE GUI_HEADERS ${GUI_INCLUDE_DIR}/${App}/*.h)
			
			add_executable(${App} WIN32
					${GUI_SOURCES}
					${GUI_HEADERS}
					)
			target_link_libraries(${App} ${CMAKE_REQUIRED_LIBRARIES})
			# Additional dependecies.		
			target_link_libraries(${App} 
										${wxWidgets_LIBRARIES}
										${EXT_LIBS})
			# Setup dependencies and build order.
			add_dependencies(${App} ${DEPENDENCIES} ${PhysioSync_PLUGINS})	
			
			# Add Debug postfix
			set_target_properties(${App} PROPERTIES DEBUG_POSTFIX "_d")
			# Solution folders.
			if(USE_SOLUTION_FOLDERS)
				SET_PROPERTY(TARGET ${App} PROPERTY FOLDER "JAUS++ Programs")
			endif()

			install(TARGETS ${App}
					DESTINATION bin
					RUNTIME DESTINATION bin
					COMPONENT Programs)
		endforeach()
				
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
