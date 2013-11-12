# We must specify a minimum version, current min is 2.8
IF(WIN32)
	# Require 2.8.2 for Debug/Release configurations in VS
	cmake_minimum_required (VERSION 2.8.2) 
ELSE()
	cmake_minimum_required (VERSION 2.8)
ENDIF(WIN32)

# Use path relative to cmake build dir
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

# Create a solution/workspace name and a specific library/binary name.
set(PNG_PROJECT_NAME png)
if(MSVC)
	set(PNG_LIB_NAME lib${PNG_PROJECT_NAME})
else()
	set(PNG_LIB_NAME ${PNG_PROJECT_NAME})
endif(MSVC)

# Use path relative to cmake build dir
if(COMMAND cmake_policy)
	if(POLICY CMP0015)
		cmake_policy(SET CMP0015 NEW)
	endif(POLICY CMP0015)
endif(COMMAND cmake_policy)

# Name the project (for dependancy)
project (${PNG_PROJECT_NAME})

# Add include directories for header files.
include_directories(./)
set(PNG_SRC_FOLDER ./)

if(MSVC)
add_definitions(/D_CRT_SECURE_NO_WARNINGS /wd4996 /wd4267 /DUNICODE /D_UNICODE)
endif(MSVC)

if(UNIX AND NOT MINGW)
# Ensure using gcc for our code
set(CMAKE_C_COMPILER gcc)
# Add fPIC to support 64 bit building issues
add_definitions(-fPIC)
endif(UNIX AND NOT MINGW)

set (PNG_SRC_FILES 
${PNG_SRC_FOLDER}/png.c
${PNG_SRC_FOLDER}/pngerror.c
${PNG_SRC_FOLDER}/pngget.c
${PNG_SRC_FOLDER}/pngmem.c
${PNG_SRC_FOLDER}/pngpread.c
${PNG_SRC_FOLDER}/pngread.c
${PNG_SRC_FOLDER}/pngrio.c
${PNG_SRC_FOLDER}/pngrtran.c
${PNG_SRC_FOLDER}/pngrutil.c
${PNG_SRC_FOLDER}/pngset.c
${PNG_SRC_FOLDER}/pngtrans.c
${PNG_SRC_FOLDER}/pngwio.c
${PNG_SRC_FOLDER}/pngwrite.c
${PNG_SRC_FOLDER}/pngwtran.c
${PNG_SRC_FOLDER}/pngwutil.c
)



# Build a static library
add_library(${PNG_LIB_NAME} STATIC ${PNG_SRC_FILES})

# Other libraries to link with (i.e. any dependencies).
include_directories(../zlib-1.2.3/)
add_subdirectory(../zlib-1.2.3/ ./cmake.zlib)
link_directories(../zlib-1.2.3/lib/)


# Maintain build order
add_dependencies(${PNG_LIB_NAME} ${LIB_PREFIX}zlib)
if(WIN32)
target_link_libraries(${PNG_LIB_NAME} 
						debug ${LIB_PREFIX}zlib_d.lib
						optimized ${LIB_PREFIX}zlib.lib)
else()
target_link_libraries(${PNG_LIB_NAME} 
						debug ${LIB_PREFIX}zlib_d
						optimized ${LIB_PREFIX}zlib)
endif()

# Other libraries to link with (e.g. any dependencies).
target_link_libraries(${PNG_LIB_NAME} ${CMAKE_REQUIRED_LIBRARIES})
# Add an _d when building debug version
set_target_properties(${PNG_LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")
# Solution folders.
if(USE_SOLUTION_FOLDERS)
	SET_PROPERTY(TARGET ${PNG_LIB_NAME} PROPERTY FOLDER "Libraries")
endif()