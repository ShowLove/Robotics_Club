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

		
# Use Boost
set(Boost_ADDITIONAL_VERSIONS 
			"1.47" 
			"1.47.0"
			"1.48"
			"1.48.0")
find_package(Boost COMPONENTS thread date_time REQUIRED)
if(NOT Boost_FOUND)
	if(NOT ${Boost_INCLUDE_DIRS})
		set(BOOST_ROOT "C:/boost" CACHE PATH "Boost root path directory")
	endif()
	if(NOT Boost_LIBRARY_DIRS)
		set(BOOST_LIBRARYDIR "C:/boost/lib" CACHE PATH "Boost library path directory")
	endif()
endif()

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

#message(${Boost_LIBRARIES})
if(WIN32)
	option(COPY_BOOST_BINARIES "Copy boost DLL files to local path" 1)
	#file(GLOB Boost_BINARIES ${Boost_LIBRARY_DIRS}/*.dll)
	if(COPY_BOOST_BINARIES)
		message(STATUS "Copying Boost DLL Files")
	endif()
	foreach(LIB ${Boost_LIBRARIES})
		if(NOT ${LIB} MATCHES "optimized" AND NOT ${LIB} MATCHES "debug")
			set(BIN ${LIB})
			string(REPLACE ".lib" ".dll" BIN ${BIN})
			string(REPLACE "libboost" "boost" BIN ${BIN})
			if(EXISTS ${BIN})
				set(Boost_BINARIES ${Boost_BINARIES} ${BIN})
				if(COPY_BOOST_BINARIES)
					message(STATUS "\t${BIN}")
					
					execute_process(COMMAND ${CMAKE_COMMAND} -E copy
										${BIN} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
										RESULT_VARIABLE COPY_RESULT
										ERROR_VARIABLE ERROR)
					if(NOT COPY_RESULT EQUAL 0)
								MESSAGE(STATUS "Copy Boost - ${ERROR}")
					endif()
				endif()
			endif()
		endif()
	endforeach()
endif()

# message(STATUS "Found Boost = ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")
if(NOT WIN32)
	set(EXT_LIBS ${Boost_LIBRARIES})
endif()


# Set the name of the JAUS library.
set(JAUS_NAME core)
# No other JAUS dependencies for core.
set(JAUS_INTERNAL_DEPENDENCIES "")
# Set the name to use for the DLL Export Preprocessor
# definition for WIN32
set(DLL_EXPORT_NAME JAUS_CORE_DLL_EXPORT)
# Set name of this project, and dependencies needed.
# Names are different between MSVC and other build systems, so
# in MSVC add prefix to name(s) as needed.
set(LIB_PROJECT_NAME jaus${JAUS_NAME})
# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(LIB_NAME ${LIB_PREFIX}${LIB_PROJECT_NAME})
# Source/Directories to sort.
set(SOURCE_GROUPS control discovery events liveness management time transport)

# Add include directories for header files.
include_directories(../../../include/
					../../../ext
                                        ../../../ext/tstl
					${TinyXML_INCLUDE_DIRS}
					${CxUtils_INCLUDE_DIRS})

# Set variable name of input directories for code.
set(LIB_INCLUDE_DIR ../../../include/jaus/${JAUS_NAME})
set(LIB_SRC_DIR ../../../src/jaus/${JAUS_NAME})

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
endif()

# Add paths to directories containing external dependencies we need.
link_directories(../../../lib)

# Get header files.
file(GLOB_RECURSE LIB_HEADERS ${LIB_INCLUDE_DIR}/*.h)
# Get src files.
file(GLOB_RECURSE LIB_SOURCES ${LIB_SRC_DIR}/*.cpp)


# Build a library
add_library(${LIB_NAME} SHARED ${LIB_SOURCES} ${LIB_HEADERS})
if(WIN32)
	# Add a define for building a DLL in windows.
	set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "-D${DLL_EXPORT_NAME}")
else()
	# Link symbols
        set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS -rdynamic)
endif(WIN32)

# Add an _d when building debug version
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")

set(JAUS_INTERNAL_DEPENDENCIES 
		${CxUtils_DEPENDENCY}
		${TinyXML_DEPENDENCY})
# Set build dependencies.  These projects will get built
# before this file.
add_dependencies(${LIB_NAME} ${JAUS_INTERNAL_DEPENDENCIES})


# Set any external libraries to link against.
set(EXT_LIBS ${EXT_LIBS}
		${CxUtils_LIBRARIES}
		${TinyXML_LIBRARIES})

if(NOT WIN32)
    set(EXT_LIBS ${EXT_LIBS} rt)
endif()

# Link against dependencies.
target_link_libraries(${LIB_NAME} 
						${CMAKE_REQUIRED_LIBRARIES}
						${EXT_LIBS})

# Standard libraries to link against
target_link_libraries(${LIB_NAME} ${EXT_LIBS})

###########################################
##### NODE MANAGER APPLICATION
###########################################

# Set variable name of input directories for code.
set(GUI_INCLUDE_DIR ../../../src/programs/gui/)
set(GUI_SOURCE_DIR ../../../src/programs/gui/)

#  If wxWidgets is found, build wxWidget applications.
if(MSVC)
	set(wxWidgets_CONFIGURATION mswu)
	set(wxWidgets_USE_REL_AND_DBG 1)
endif(MSVC)

find_package(wxWidgets COMPONENTS net core base adv)

if(MSVC)
	file(GLOB_RECURSE RESOURCE_FILES ../../../resources/*.rc)
	#message(STATUS ${RESOURCE_FILES})
endif(MSVC)

if(wxWidgets_FOUND)
		include(${wxWidgets_USE_FILE})
		include_directories(${GUI_SOURCE_DIR}
							${GUI_INCLUDE_DIR})

		# Dialog Files
		set(JAUSApps 	
				NodeManager)
		
		foreach(App ${JAUSApps})
			# Application
			file(GLOB_RECURSE GUI_SOURCES ${GUI_SOURCE_DIR}/${App}/*.cpp)
			file(GLOB_RECURSE GUI_HEADERS ${GUI_INCLUDE_DIR}/${App}/*.h)
			
			if(MSVC)
				set(GUI_SOURCES ${GUI_SOURCES} ${RESOURCE_FILES})
			endif(MSVC)
			
			add_executable(${App} WIN32
					${GUI_SOURCES}
					${GUI_HEADERS}
					)
			target_link_libraries(${App} ${CMAKE_REQUIRED_LIBRARIES})
			# Additional dependecies.		
			target_link_libraries(${App} 
						${wxWidgets_LIBRARIES}
						${EXT_LIBS}
						debug ${LIB_PREFIX}${LIB_PROJECT_NAME}_d
						optimized ${LIB_PREFIX}${LIB_PROJECT_NAME})
			# Setup dependencies and build order.
			add_dependencies(${App} ${JAUS_INTERNAL_DEPENDENCIES} ${LIB_NAME})	
			
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
		
		file(COPY 
			../../../bin/icons 
			DESTINATION	${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
			
endif(wxWidgets_FOUND)

# CPack Installer Generation

install(TARGETS ${LIB_NAME}
			DESTINATION ${BIN_INSTALL_DEST}
			ARCHIVE DESTINATION ${LIB_INSTALL_DEST}
			COMPONENT SDK)
install(TARGETS ${LIB_NAME}
		DESTINATION ${BIN_INSTALL_DEST}
		RUNTIME DESTINATION ${BIN_INSTALL_DEST}
		COMPONENT SDK)
install(TARGETS ${LIB_NAME}
		DESTINATION ${BIN_INSTALL_DEST}
		LIBRARY DESTINATION ${BIN_INSTALL_DEST}
		COMPONENT SDK)
		
install(DIRECTORY 
		${LIB_INCLUDE_DIR}
		DESTINATION ${INCLUDE_INSTALL_DEST}/jaus
		COMPONENT SDK
		FILES_MATCHING PATTERN "*.h" 
		PATTERN ".svn" EXCLUDE)

if(WIN32)
    install(DIRECTORY 
		    ../../../docs/
		    DESTINATION docs/../docs/JAUS++
		    COMPONENT SDK
		    PATTERN ".svn" EXCLUDE
		    )
endif(WIN32)

install(DIRECTORY 
		../../../include
		../../../src
		../../../ext
		../../../docs
		DESTINATION src/active-ist/libraries/jaus++/trunk/
		COMPONENT SourceCode
		PATTERN ".svn" EXCLUDE
		)
install(DIRECTORY 
		../../../bin/icons
		../../../bin/images
		../../../bin/logs
		#../../../bin/logs
		../../../bin/settings
		../../../bin/video
		DESTINATION src/active-ist/libraries/jaus++/trunk/bin
		COMPONENT SourceCode
		PATTERN ".svn" EXCLUDE)
		
if(WIN32)
	install(FILES
				${Boost_BINARIES}
				DESTINATION bin
				COMPONENT SDK)
    install(DIRECTORY 
		    ../../../bin/icons
		    ../../../bin/images
		    ../../../bin/logs
		    #../../../bin/logs
		    ../../../bin/settings
		    ../../../bin/video
		    DESTINATION bin
		    COMPONENT SDK
		    PATTERN ".svn" EXCLUDE)
else()
        install(DIRECTORY 
		    ../../../bin/icons
		    ../../../bin/images
		    ../../../bin/logs
		    #../../../bin/logs
		    ../../../bin/settings
		    ../../../bin/video
		    DESTINATION bin/active
		    COMPONENT SDK
		    PATTERN ".svn" EXCLUDE)
endif(WIN32)

install(DIRECTORY 
		../../../build/cmake/lib${LIB_PROJECT_NAME}
		DESTINATION src/active-ist/libraries/jaus++/trunk/build/cmake
		COMPONENT SourceCode
		PATTERN ".svn" EXCLUDE)


# Make Source Groups, this makes sub folders in Visual Studio

foreach(GRP ${SOURCE_GROUPS})
	file(GLOB_RECURSE HEADER_GROUP ${LIB_INCLUDE_DIR}/${GRP}/*.h)
	file(GLOB_RECURSE SRC_GROUP ${LIB_SRC_DIR}/${GRP}/*.cpp)
	SOURCE_GROUP("Header Files\\${GRP}" FILES ${HEADER_GROUP})
	SOURCE_GROUP("Source Files\\${GRP}" FILES ${SRC_GROUP})
endforeach(GRP)

