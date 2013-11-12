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

if(WIN32)
    set(LIB_PREFIX "lib")
else()
    set(LIB_PREFIX "")
endif()

set(EXE_NAME example)

include_directories(../../../include/)

link_directories(../../../lib/)

file(GLOB_RECURSE EXE_INC ../../../include/${EXE_NAME}/*.h)
file(GLOB_RECURSE EXE_SRC ../../../src/${EXE_NAME}/*.cpp)

add_executable(${EXE_NAME} ${EXE_INC} ${EXE_SRC})

# Add an _d when building debug version
set_target_properties(${EXE_NAME} PROPERTIES DEBUG_POSTFIX "_d")

add_dependencies(${EXE_NAME} ${LIB_PREFIX}pipetree)
target_link_libraries(${EXE_NAME} ${LIB_PREFIX}pipetree ${OpenCV_LIBS})
