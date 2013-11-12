#
#
#

cmake_minimum_required (VERSION 2.8)

set(LIB_NAME pipetree)

project(${LIB_NAME})

if(UNIX)
	# Ensure using gcc for our code
	set(CMAKE_C_COMPILER gcc)
	# Add fPIC to support 64 bit building issues
	add_definitions(-fPIC)
endif(UNIX)

find_package( OpenCV REQUIRED )

add_subdirectory(libpipetree ./cmake.pipetree)

add_subdirectory(example ./cmake.example)
add_subdirectory(unit_test ./cmake.unit_test)
