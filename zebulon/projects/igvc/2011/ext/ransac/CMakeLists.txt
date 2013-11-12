#edited by dadams@ist.ucf.edu

cmake_minimum_required(VERSION 2.8)

PROJECT( RANSAC )

set(LIB_NAME ransac)

INCLUDE_DIRECTORIES (
  ${RANSAC_EXAMPLE_SOURCE_DIR}
)
OPTION(USE_MULTITHREADING "Use Multithreading" TRUE)

SET(RANSAC_BASE_INC
    RANSAC.h
    ParameterEstimator.h
    LineParamEstimator.h
    Point2D.h
    )

SET(RANSAC_BASE_SRC
    RANSAC.txx
    LineParamEstimator.cxx
    Point2D.cxx
    )

IF(USE_MULTITHREADING)
    SET(EXAMPLE_SRC mainMT.cxx)
    ADD_SUBDIRECTORY(multiThreading)
    SET(RANSAC_LIB_INC ${RANSAC_BASE_INC} RANSACMT.h)
    SET(RANSAC_LIB_SRC ${RANSAC_BASE_SRC} RANSACMT.txx)
    INCLUDE_DIRECTORIES(
            ${MULTI_THREADING_SOURCE_DIR}
            ${MULTI_THREADING_BINARY_DIR})  
    LINK_DIRECTORIES(${MULTI_THREADING_BINARY_DIR})
    LINK_LIBRARIES (MultiThreading ${LIB_NAME})
ELSE()
    SET(EXAMPLE_SRC main.cxx)
    SET(RANSAC_LIB_INC ${RANSAC_BASE_INC} RANSAC.h)
    SET(RANSAC_LIB_SRC ${RANSAC_BASE_SRC} RANSAC.txx)
    LINK_LIBRARIES (${LIB_NAME})

ENDIF()

# Build a library
add_library(${LIB_NAME} SHARED ${RANSAC_LIB_SRC} ${RANSAC_LIB_INC})
# Add an _d when building debug version
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")

ADD_EXECUTABLE(example_ransac ${EXAMPLE_SRC})
TARGET_LINK_LIBRARIES(example_ransac ${LINK_LIBRARIES})
set_target_properties(example_ransac PROPERTIES DEBUG_POSTFIX "_d")

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
# Install targets
install(TARGETS ${LIB_NAME} 
	LIBRARY DESTINATION 
	lib/zebulon
	COMPONENT SDK)


