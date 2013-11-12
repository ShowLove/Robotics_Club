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
include(../FindPylon.cmake)

# Set the name to use for the DLL Export Preprocessor
# definition for WIN32
#set(DLL_EXPORT_NAME ZEB_THERMAL_DLL)
# Set name of this project, and dependencies needed.
# Names are different between MSVC and other build systems, so
# in MSVC add prefix to name(s) as needed.
set(LIB_PROJECT_NAME pyloncapture)
# Set build dependencies.  These projects will get built
# before this file.
#set(DEPENDENCIES 
#		${LIB_PREFIX}XXX)
# Source/Directories to sort.
#set(SOURCE_GROUPS messages)
# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(LIB_NAME ${LIB_PREFIX}${LIB_PROJECT_NAME})

# Set variable name of input directories for code.
set(LIB_INCLUDE_DIR ../../../include/asvc/${LIB_PROJECT_NAME})
set(LIB_SRC_DIR ../../../src/asvc/${LIB_PROJECT_NAME})

# Add include directories for header files.
include_directories(../../../include/ ${CxUtils_INCLUDE_DIRS} ${LIB_INCLUDE_DIR} )
if(OpenCV_FOUND)
        set(EXT_LIBS ${EXT_LIBS}
                                ${OpenCV_LIBRARIES})
        link_directories(${OpenCV_LIBRARY_DIRS})
endif(OpenCV_FOUND)
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

foreach(V ${LIB_HEADERS})
#MESSAGE(STATUS ${V})
endforeach()

# Build a library
add_library(${LIB_NAME} SHARED ${LIB_SOURCES} ${LIB_HEADERS})

# Solution folders.
if(USE_SOLUTION_FOLDERS)
	SET_PROPERTY(TARGET ${LIB_NAME}  PROPERTY FOLDER "Zebulon")
endif()
		
if(WIN32)
	# Add a define for building a DLL in windows.
	set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "-D${DLL_EXPORT_NAME}=__declspec(dllexport)")
else()
	# Link symbols
	set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS -rdynamic)
endif(WIN32)

# Add an _d when building debug version
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")
# Set build dependencies.  These projects will get built
# before this file.
if(CxUtils_SET_DEPENDENCY)
	add_dependencies(${LIB_NAME} ${CxUtils_DEPENDENCY})
endif(CxUtils_SET_DEPENDENCY)

if(DEPENDENCIES)
	add_dependencies(${LIB_NAME} ${DEPENDENCIES})
	target_link_libraries(${LIB_NAME} ${DEPENDENCIES})
endif(DEPENDENCIES)
# Set any external libraries to link against.
set(EXT_LIBS    ${EXT_LIBS}
		${CxUtils_LIBRARIES})


if(Pylon_FOUND)
	set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS ${Pylon_LINKER_FLAGS})
endif(Pylon_FOUND)
# If Basler Pylon library found
# include support for it
if(Pylon_FOUND)
        #set(ENV{PYLON_ROOT} /opt/pylon)
        #set(ENV{GENICAM_ROOT_V2_1} /opt/pylon/genicam)
        include_directories(${Pylon_INCLUDE_DIRS})
        link_directories(${Pylon_LIBRARY_DIRS})
        #message(STATUS ${Pylon_LIBRARY_DIRS})
        set(LIB_HEADERS ${LIB_HEADERS} ${LIB_INCLUDE_DIR}/imagecapturepylon.h)
        set(LIB_SOURCES ${LIB_SOURCES} ${LIB_SRC_DIR}/imagecapturepylon.cpp)
        set(EXT_LIBS ${EXT_LIBS} ${Pylon_LIBRARIES})
        message(STATUS "Pylon Dirs: ${Pylon_LIBRARY_DIRS}")
        message(STATUS "Pylon Libs: ${Pylon_LIBRARIES}")
        add_definitions(${Pylon_CXX_FLAGS})
        #message(STATUS ${Pylon_CXX_FLAGS})
        message(STATUS "Basler Pylon Library Found")
else(Pylon_FOUND)
        message(STATUS "Basler Pylon Library NOT FOUND, skipping support")
endif(Pylon_FOUND)

# Link against dependencies.
target_link_libraries(${LIB_NAME} 
						${CMAKE_REQUIRED_LIBRARIES}
						${EXT_LIBS})

#message(STATUS "-----------------------"${Pylon_LINKER_FLAGS})
# CPack Installer Generation
if(WIN32)

install(TARGETS ${LIB_NAME}
        DESTINATION bin
        ARCHIVE DESTINATION lib
        COMPONENT SDK)
install(TARGETS ${LIB_NAME}
		DESTINATION bin
		RUNTIME DESTINATION bin
		COMPONENT SDK)
install(TARGETS ${LIB_NAME}
		DESTINATION bin
		LIBRARY DESTINATION bin
		COMPONENT SDK)
install(DIRECTORY 
		${LIB_INCLUDE_DIR}
		DESTINATION include/ 
		COMPONENT SDK
		FILES_MATCHING PATTERN "*.h" 
		PATTERN ".svn" EXCLUDE)
elseif(UNIX)
	# Where to install headers
	install(DIRECTORY 
			${LIB_INCLUDE_DIR}
			DESTINATION include/zebulon/
			COMPONENT SDK
			FILES_MATCHING PATTERN "*.h" 
			PATTERN ".svn" EXCLUDE)
	# Where to install targets
	install(TARGETS 
			${LIB_NAME} 
			LIBRARY DESTINATION 
			lib/zebulon
			COMPONENT SDK)
endif(WIN32)

# Make Source Groups, this makes sub folders in Visual Studio

file(GLOB HEADER_GROUP ${LIB_INCLUDE_DIR}/messages/*.h)
file(GLOB SRC_GROUP ${LIB_SRC_DIR}/messages/*.cpp)
SOURCE_GROUP("Header Files\\messages" FILES ${HEADER_GROUP})
SOURCE_GROUP("Source Files\\messages" FILES ${SRC_GROUP})


file(GLOB HEADER_GROUP ${LIB_INCLUDE_DIR}/messages/custom/*.h)
file(GLOB SRC_GROUP ${LIB_SRC_DIR}/messages/custom/*.cpp)
SOURCE_GROUP("Header Files\\messages\\custom" FILES ${HEADER_GROUP})
SOURCE_GROUP("Source Files\\messages\\custom" FILES ${SRC_GROUP})

