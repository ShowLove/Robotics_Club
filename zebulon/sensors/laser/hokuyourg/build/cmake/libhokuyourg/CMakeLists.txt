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

#find_package(SDL)

#if(NOT SDL_FOUND)
#    message(STATUS "SDL NOT FOUND!")
#endif()

#include_directories(${SDL_INCLUDE_DIRS})
#link_directories(${SDL_LIBRARY_DIRS})

#set(EXT_LIBS ${SDL_LIBRARIES})

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
        set(EXT_LIBS ${EXT_LIBS} ${Boost_LIBRARIES})
endif()


# Set the name to use for the DLL Export Preprocessor
# definition for WIN32
set(DLL_EXPORT_NAME ZEB_URG_DLL)
# Set name of this project, and dependencies needed.
# Names are different between MSVC and other build systems, so
# in MSVC add prefix to name(s) as needed.
set(LIB_PROJECT_NAME hokuyourg)
# Set build dependencies.  These projects will get built
# before this file.
set(DEPENDENCIES 
		${LIB_PREFIX}laser)
# Source/Directories to sort.
#set(SOURCE_GROUPS messages)
# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(LIB_NAME ${LIB_PREFIX}${LIB_PROJECT_NAME})

# Add include directories for header files.
include_directories(../../../include/
			${CxUtils_INCLUDE_DIRS}
			../../../ext/
			../../../../abstract/include)

# Set variable name of input directories for code.
set(LIB_INCLUDE_DIR ../../../include/${LIB_PROJECT_NAME})
set(LIB_SRC_DIR ../../../src/${LIB_PROJECT_NAME})

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
if(WIN32)

else()

endif()
				
# Build a library
add_library(${LIB_NAME} SHARED ${LIB_SOURCES} ${LIB_HEADERS})

# Solution folders.
if(USE_SOLUTION_FOLDERS)
	SET_PROPERTY(TARGET ${LIB_NAME}  PROPERTY FOLDER "Zebulon")
endif()


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
set(EXT_LIBS ${EXT_LIBS}
                ${CxUtils_LIBRARIES}
		urg
                urg_common
                urg_connection
                urg_connection_sdl
                urg_coordinate
                urg_monitor
                urg_system
                urg_geometry)
		
# Link against dependencies.
target_link_libraries(${LIB_NAME} 
                            ${CMAKE_REQUIRED_LIBRARIES}
                            ${EXT_LIBS})


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

foreach(GRP ${SOURCE_GROUPS})
	file(GLOB_RECURSE HEADER_GROUP ${LIB_INCLUDE_DIR}/${GRP}/*.h)
	file(GLOB_RECURSE SRC_GROUP ${LIB_SRC_DIR}/${GRP}/*.cpp)
	SOURCE_GROUP("Header Files\\${GRP}" FILES ${HEADER_GROUP})
	SOURCE_GROUP("Source Files\\${GRP}" FILES ${SRC_GROUP})
endforeach(GRP)
