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

# Set output folders for binaries and libraries.
include(../../../../../../SetOutputDirectories.cmake)

# Set build dependencies.  These projects will get built
# before this file.
set(DEPENDENCIES 
            ${LIB_PREFIX}baseline
            ${LIB_PREFIX}statemachine
            ${LIB_PREFIX}ransac
            ${CxUtils_DEPENDENCY}
            ${TinyXML_DEPENDENCY}
            ${JAUS_DEPENDENCY}
            ${Cartographer_DEPENDENCY}
            )

IF(Zebulon_SET_DEPENDENCY)
    set(DEPENDENCIES
            ${DEPENDENCIES}
            ${Zebulon_DEPENDENCIES})
ENDIF(Zebulon_SET_DEPENDENCY)

# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(EXT_LIBS 
        ${DEPENDENCIES})

# Set variable name of input directories for code.
set(INCLUDE_DIR ../../../include/igvc/)
set(SRC_DIR ../../../src/igvc/)

# Add include paths required for this project
include_directories(../../../include/
                    ../../../include/igvc
                    ../../../ext
                    ${Zebulon_INCLUDE_DIRS}
                    ${ACTIVE_INCLUDE_DIRS})

# Add library paths required for this project.
link_directories(../../../lib/ 
                ${Zebulon_LIBARARY_DIRS}
                ${ACTIVE_LIBRARY_DIRS})

# Applications

# Get header files.
file(GLOB_RECURSE PROG_HEADERS ${INCLUDE_DIR}/ground/*.h)
# Get src files.
file(GLOB_RECURSE PROG_SOURCES ${SRC_DIR}/ground/*.cpp)

add_library(ground ${PROG_SOURCES} 
                   ${PROG_HEADERS}
                   ${INCLUDE_DIR}/baseline/microcontroller.h
                   ${SRC_DIR}/baseline/microcontroller.cpp)

target_link_libraries(ground ${CMAKE_REQUIRED_LIBRARIES})
# Additional dependecies.
target_link_libraries(ground
            ${EXT_LIBS})

# Setup dependencies and build order.
add_dependencies(ground ${DEPENDENCIES})
# Add Debug postfix
set_target_properties(ground PROPERTIES DEBUG_POSTFIX "_d")

# Setup any additional compiler flags.
if(WIN32)
endif(WIN32)
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
