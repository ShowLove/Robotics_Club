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
				../../../../../
				../../../../../../)
include(${ACTIVE_MACROS_ROOT_DIR}/SetOutputDirectories.cmake)
include(${ACTIVE_MACROS_ROOT_DIR}/FindACTIVE.cmake)

# Set the name to use for the DLL Export Preprocessor
# definition for WIN32
set(DLL_EXPORT_NAME CX_UTILS_DLL_EXPORTS)
# Create a solution/workspace name and a specific library/binary name.
set(PROJECT_NAME cxutils)
# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(LIB_NAME ${LIB_PREFIX}${PROJECT_NAME})

# Create project with the previously defined name.
project (${PROJECT_NAME})

# Find packages and libraries required
include(FindTurboJPEG.cmake)
include(FindLibPNG.cmake)

if(UNIX)
	find_package(X11)
	find_path(XTST_DIR extensions/XTest.h keysym.h /usr/include/X11 /usr/local/include/X11)
	find_library(XTST_LIBRARY NAMES Xtst PATH /usr/lib /usr/local/lib)
	find_path(PTHREAD_DIR pthread.h /usr/include/ /usr/local/include/)
	find_library(PTHREAD_LIBRARY NAMES pthread PATH /usr/lib /usr/local/lib)
endif(UNIX)

# Add include directories for header files.
include_directories(../../../include)
link_directories(${LIBRARY_OUTPUT_PATH})
# Set the main folders containing headers and source files.
set(LIB_INCLUDE_DIR ../../../include/cxutils)
set(LIB_SRC_DIR ../../../src/cxutils)

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

# Add paths to directories containing external dependencies we need.
link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Get header files.
file(GLOB_RECURSE HEADERS ${LIB_INCLUDE_DIR}/*.h)
# Get src files.
file(GLOB_RECURSE SOURCES ${LIB_SRC_DIR}/*.cpp)

# Build a shared library
add_library(${LIB_NAME} SHARED ${SOURCES} ${HEADERS})
# Add an _d when building debug version
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")

# Set build dependencies.  These projects will get built
# before this file.
if(TurboJPEG_SET_DEPENDENCY)
	add_dependencies(${LIB_NAME} ${TurboJPEG_DEPENDENCY})
endif(TurboJPEG_SET_DEPENDENCY)

if(PNG_SET_DEPENDENCY)
	add_dependencies(${LIB_NAME} ${PNG_DEPENDENCY})
endif(PNG_SET_DEPENDENCY)

if(WIN32)
	# Add a define for building a DLL for CxUtils
	set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "-D${DLL_EXPORT_NAME}")
	set(EXT_LIBS winmm wsock32)
else()
	# Link symbols
	set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS -rdynamic)
	set(EXT_LIBS X11 Xtst pthread)
endif(WIN32)

target_link_libraries(${LIB_NAME} 
						${CMAKE_REQUIRED_LIBRARIES}
						${TurboJPEG_LIBRARIES}
						${PNG_LIBRARIES}
						${EXT_LIBS})

# Installer and CPack Information

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
		DESTINATION ${INCLUDE_INSTALL_DEST}
		COMPONENT SDK
		FILES_MATCHING PATTERN "*.h" 
		PATTERN ".svn" EXCLUDE
		)

if(WIN32)
    install(DIRECTORY 
		    ../../../docs/
		    DESTINATION docs/../docs/CxUtils
		    COMPONENT SDK
		    PATTERN ".svn" EXCLUDE
		    )
endif(WIN32)

install(DIRECTORY 
		../../../include
		../../../src
		../../../ext
		DESTINATION src/active-ist/libraries/cxutils/trunk/
		COMPONENT SourceCode
		PATTERN ".svn" EXCLUDE
		)

install(DIRECTORY 
		../../../bin/
		DESTINATION src/active-ist/libraries/cxutils/trunk/bin
		COMPONENT SourceCode
		FILES_MATCHING PATTERN "*.jpg" 
		PATTERN ".svn" EXCLUDE)

file(GLOB FIND_MODULES_AND_MACROS ../../../../../*.cmake)

install(FILES 
		${FIND_MODULES_AND_MACROS}
		DESTINATION src/active-ist/libraries
		COMPONENT SourceCode
		)

install(DIRECTORY 
		../../../build/cmake/libcxutils
		DESTINATION src/active-ist/libraries/cxutils/trunk/build/cmake
		COMPONENT SourceCode
		PATTERN ".svn" EXCLUDE)

# Make Source Groups, this makes sub folders in Visual Studio

# Source/Directories to sort.
set(SOURCE_GROUPS images ipc math networking plugin)
foreach(GRP ${SOURCE_GROUPS})
	file(GLOB_RECURSE HEADER_GROUP ${LIB_INCLUDE_DIR}/${GRP}/*.h)
	file(GLOB_RECURSE SRC_GROUP ${LIB_SRC_DIR}/${GRP}/*.cpp)
	SOURCE_GROUP("Header Files\\${GRP}" FILES ${HEADER_GROUP})
	SOURCE_GROUP("Source Files\\${GRP}" FILES ${SRC_GROUP})
endforeach(GRP)
	
