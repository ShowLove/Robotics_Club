#
#
#

cmake_minimum_required (VERSION 2.8)

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../bin/)
#set(EXECUTABLE_OUTPUT_PATH_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../../../bin/)
#set(EXECUTABLE_OUTPUT_PATH_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../../../bin/)
#set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/)
#set(LIBRARY_OUTPUT_PATH_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/)
#set(LIBRARY_OUTPUT_PATH_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/)

include(../../../../../../SetOutputDirectories.cmake)

set(LIB_NAME pipetree)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

SET(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS thread REQUIRED)

include_directories(../../../include/
                    ${Boost_INCLUDE_DIR})

file(GLOB_RECURSE LIB_INC ../../../include/${LIB_NAME}/*.h)
file(GLOB_RECURSE LIB_HPP ../../../include/${LIB_NAME}/*.hpp)
file(GLOB_RECURSE LIB_SRC ../../../src/${LIB_NAME}/*.cpp)

add_library(${LIB_NAME} ${LIB_INC} ${LIB_HPP} ${LIB_SRC})

# Add an _d when building debug version
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")

target_link_libraries(${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${OpenCV_LIBS})
message(STATUS "OpenCV Libs: ${OpenCV_LIBS}")

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions("-g -Wall -std=gnu++0x") #-std=c++0x
    message(STATUS "Setting flags for C++11/C++0x Support")
endif()

