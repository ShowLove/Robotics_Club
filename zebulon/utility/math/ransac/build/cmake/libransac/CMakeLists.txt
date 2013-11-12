#edited by dadams@ist.ucf.edu
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

set(LIB_NAME ransac)

# Set variable name of input directories for code.
set(INCLUDE_DIR ../../../include/ransac/)
set(SRC_DIR ../../../src/ransac/)

SET(RANSAC_BASE_INC
    ${INCLUDE_DIR}RANSAC.h
    ${INCLUDE_DIR}ParameterEstimator.h
    ${INCLUDE_DIR}LineParamEstimator.h
    ${INCLUDE_DIR}Point2D.h
    )

SET(RANSAC_BASE_SRC
    ${SRC_DIR}RANSAC.txx
    ${SRC_DIR}LineParamEstimator.cxx
    ${SRC_DIR}Point2D.cxx
    )

IF(USE_MULTITHREADING)
    SET(RANSAC_LIB_INC ${RANSAC_BASE_INC} ${INCLUDE_DIR}/RANSACMT.h)
    SET(RANSAC_LIB_SRC ${RANSAC_BASE_SRC} ${SRC_DIR}/RANSACMT.txx)
    #INCLUDE_DIRECTORIES(
    #        ${MULTI_THREADING_SOURCE_DIR}
    #        ${MULTI_THREADING_BINARY_DIR})  
    #LINK_DIRECTORIES(${MULTI_THREADING_BINARY_DIR})
    #LINK_LIBRARIES (MultiThreading ${LIB_NAME})
    include_directories(../../../include/ransac/multiThreading .)
    set(DEPENDENCIES
            multiThreading)
ELSE()
    SET(RANSAC_LIB_INC ${RANSAC_BASE_INC})
    SET(RANSAC_LIB_SRC ${RANSAC_BASE_SRC})
    LINK_LIBRARIES (${LIB_NAME})
ENDIF()

# Add include paths required for this project
include_directories(../../../include/ransac
                    ../../../src/ransac)

# Build a library
#MESSAGE("${MULTI_THREADING_SOURCE_DIR}")
add_library(${LIB_NAME} SHARED ${RANSAC_LIB_SRC} ${RANSAC_LIB_INC})
# Add an _d when building debug version
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")

# Solution folders.
if(USE_SOLUTION_FOLDERS)
	SET_PROPERTY(TARGET ${LIB_NAME}  PROPERTY FOLDER "Zebulon")
endif()
		

install(DIRECTORY 
	./
	DESTINATION include/zebulon/ransac 
	COMPONENT SDK
	FILES_MATCHING PATTERN "*.h" 
	PATTERN ".svn" EXCLUDE)
install(DIRECTORY 
	./
	DESTINATION include/zebulon/ransac 
	COMPONENT SDK
	FILES_MATCHING PATTERN "*.txx" 
	PATTERN ".svn" EXCLUDE)
IF(WIN32)
# may need to add ARCHIVE
ELSE()
# Install targets
install(TARGETS ${LIB_NAME} 
	LIBRARY DESTINATION 
	lib/zebulon
	COMPONENT SDK)
ENDIF(WIN32)

