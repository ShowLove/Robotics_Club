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

# Name the project
project (RANSAC)

# Set output folders for binaries and libraries.
include(../../../../../SetOutputDirectories.cmake)

#option(USE_MULTITHREADING "Enable MultiThreading" ${MULTITHREADING}) 

IF(USE_MULTITHREADING)
add_subdirectory(multiThreading ./ransac/multiThreading)
ENDIF()

add_subdirectory(example_ransac ./ransac/example_ransac)
add_subdirectory(libransac ./ransac/libransac)

