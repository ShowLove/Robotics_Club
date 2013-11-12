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

# Use Boost
set(Boost_ADDITIONAL_VERSIONS 
			"1.47" 
			"1.47.0"
			"1.48"
			"1.48.0")
find_package(Boost COMPONENTS thread REQUIRED)
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
if(NOT WIN32)
	set(EXT_LIBS ${Boost_LIBRARIES})
endif()


# Set the name of the JAUS library.
set(JAUS_NAME extras)
# Other JAUS dependencies
set(JAUS_INTERNAL_DEPENDENCIES 
			${CxUtils_DEPENDENCY}
			${TinyXML_DEPENDENCY}
			${LIB_PREFIX}jauscore
			${LIB_PREFIX}jausmobility)
# Set the name to use for the DLL Export Preprocessor
# definition for WIN32
set(DLL_EXPORT_NAME JAUS_EXTRAS_DLL_EXPORT)
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
set(SOURCE_GROUPS audio command controllers mcu rangesensor video mobility)

# Add include directories for header files.
include_directories(../../../include/
					${TinyXML_INCLUDE_DIRS}
					${CxUtils_INCLUDE_DIRS})

# Set variable name of input directories for code.
set(LIB_INCLUDE_DIR ../../../include/jaus/${JAUS_NAME})
set(LIB_SRC_DIR ../../../src/jaus/${JAUS_NAME})

# Setup any additional compiler flags.
if(WIN32)
	if(MSVC)
		# Setup to use UNICODE for MSVC, and disable warnings for
		# use of s_* standard C functions.
		add_definitions(-D_CRT_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE)
		# Add support for parallel compilation
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	elseif(UNIX AND NOT MINGW)
		# Ensure using gcc for our code
		set(CMAKE_C_COMPILER gcc)
		# Add fPIC to support 64 bit building issues
		add_definitions(-fPIC)
	endif()
endif(WIN32)

# Add paths to directories containing external dependencies we need.
link_directories(../../../lib)

# Get header files.
file(GLOB_RECURSE LIB_HEADERS ${LIB_INCLUDE_DIR}/*.h)
# Get src files.
file(GLOB_RECURSE LIB_SOURCES ${LIB_SRC_DIR}/*.cpp)


# Build a library
add_library(${LIB_NAME} SHARED ${LIB_SOURCES} ${LIB_HEADERS})
IF(WIN32)
	# Add a define for building a DLL in windows.
	set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "-D${DLL_EXPORT_NAME}")
ELSE()
	# Link symbols
	set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS -rdynamic)
ENDIF(WIN32)

# Add an _d when building debug version
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")

# Set build dependencies.  These projects will get built
# before this file.
add_dependencies(${LIB_NAME} ${JAUS_INTERNAL_DEPENDENCIES})

# Set any external libraries to link against.
set(EXT_LIBS ${EXT_LIBS}
		${CxUtils_LIBRARIES}
		${TinyXML_LIBRARIES}
		debug ${LIB_PREFIX}jauscore_d${ACTIVE_ARCHIVE_EXTENSION}
		optimized ${LIB_PREFIX}jauscore${ACTIVE_ARCHIVE_EXTENSION}
		debug ${LIB_PREFIX}jausmobility_d${ACTIVE_ARCHIVE_EXTENSION}
		optimized ${LIB_PREFIX}jausmobility${ACTIVE_ARCHIVE_EXTENSION})
		
# Link against dependencies.
target_link_libraries(${LIB_NAME} 
						${CMAKE_REQUIRED_LIBRARIES}
						${EXT_LIBS})

# Standard libraries to link against
target_link_libraries(${LIB_NAME} ${EXT_LIBS})


# CPack Installer Generation
# install(TARGETS ${LIB_NAME} ${LIB_NAME} ${LIB_NAME}
		# RUNTIME DESTINATION ${BIN_INSTALL_DEST}
		# LIBRARY DESTINATION ${LIB_INSTALL_DEST}
		# ARCHIVE DESTINATION ${LIB_INSTALL_DEST}
		# COMPONENT SDK)
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
