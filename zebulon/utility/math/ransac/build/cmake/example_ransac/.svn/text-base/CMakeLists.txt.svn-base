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

link_directories(../../../lib/ )
# Program name.
set(PROGRAM_NAME
            example_ransac)
# Set variable name of input directories for code.
set(INCLUDE_DIR ../../../include/ransac/ .)
set(SRC_DIR ../../../src/ransac/)
set(DEPENDENCIES
        ${LIB_PREFIX}ransac)

# Add include paths required for this project
include_directories(../../../include/ransac
                    ../../../src/ransac)
IF(USE_MULTITHREADING)
    SET(EXAMPLE_SRC ${SRC_DIR}mainMT.cxx)
    #INCLUDE_DIRECTORIES(
    #        ${MULTI_THREADING_SOURCE_DIR}
    #        ${MULTI_THREADING_BINARY_DIR})  
    #LINK_DIRECTORIES(${MULTI_THREADING_BINARY_DIR})
    #LINK_LIBRARIES (MultiThreading ${LIB_NAME})
    include_directories(../../../include/ransac/multiThreading)
    #MESSAGE("${MULTI_THREADING_SOURCE_DIR}")
ELSE()
    SET(EXAMPLE_SRC ${SRC_DIR}main.cxx)
   # LINK_LIBRARIES (${LIB_NAME})
ENDIF()


ADD_EXECUTABLE(${PROGRAM_NAME} ${EXAMPLE_SRC})
add_dependencies(${PROGRAM_NAME} ${DEPENDENCIES})
TARGET_LINK_LIBRARIES(example_ransac ${DEPENDENCIES})
set_target_properties(example_ransac PROPERTIES DEBUG_POSTFIX "_d")

