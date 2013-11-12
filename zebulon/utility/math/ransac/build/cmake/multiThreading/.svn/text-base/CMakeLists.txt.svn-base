cmake_minimum_required(VERSION 2.8)

include(../../../../../../SetOutputDirectories.cmake)
PROJECT( MULTI_THREADING )

#
#Get the definitions from the FindThreads.cmake file which is part of the
#Cmake distribution. This module determines which thread library is available
#on the system
#
INCLUDE(FindThreads)

# for the gnu compiler a -D_PTHREADS is needed on sun
# for the native compiler a -mt flag is needed on the sun
IF(CMAKE_USE_PTHREADS_INIT)
  IF(CMAKE_SYSTEM MATCHES "SunOS.*")
    IF(CMAKE_COMPILER_IS_GNUCXX)
      SET(MULTI_THREADING_REQUIRED_CXX_FLAGS "${MULTI_THREADING_REQUIRED_CXX_FLAGS} -D_PTHREADS")
    ELSE(CMAKE_COMPILER_IS_GNUCXX)
      SET(MULTI_THREADING_REQUIRED_CXX_FLAGS "${MULTI_THREADING_REQUIRED_CXX_FLAGS} -mt")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
  ENDIF(CMAKE_SYSTEM MATCHES "SunOS.*")
ENDIF(CMAKE_USE_PTHREADS_INIT)

set(INCLUDE_DIR ../../../include/ransac/multiThreading/)
set(SRC_DIR ../../../src/ransac/multiThreading/)
include_directories(../../../include/ransac/multiThreading .)

SET(MULTI_THREADING_HDRS
    ${INCLUDE_DIR}itkMacro.h
    ${INCLUDE_DIR}itkIndent.h
    ${INCLUDE_DIR}itkTimeStamp.h
    ${INCLUDE_DIR}itkSmartPointer.h
    ${INCLUDE_DIR}itkSimpleFastMutexLock.h
    ${INCLUDE_DIR}itkLightObject.h
    ${INCLUDE_DIR}itkWin32Header.h
    ${INCLUDE_DIR}itkWindows.h
    ${INCLUDE_DIR}itkExceptionObject.h
    ${INCLUDE_DIR}itkObjectFactoryBase.h
    ${INCLUDE_DIR}itkObjectFactory.h
    ${INCLUDE_DIR}itkObject.h
    ${INCLUDE_DIR}itkFastMutexLock.h
    ${INCLUDE_DIR}itkCreateObjectFunction.h
    ${INCLUDE_DIR}itkMutexLock.h
    ${INCLUDE_DIR}itkMultiThreader.h
    )

SET(MULTI_THREADING_SRCS
    ${SRC_DIR}itkIndent.cxx
    ${SRC_DIR}itkTimeStamp.cxx
    ${SRC_DIR}itkSimpleFastMutexLock.cxx
    ${SRC_DIR}itkExceptionObject.cxx
    ${SRC_DIR}itkObjectFactoryBase.cxx
    ${SRC_DIR}itkLightObject.cxx
    ${SRC_DIR}itkObject.cxx
    ${SRC_DIR}itkFastMutexLock.cxx
    ${SRC_DIR}itkMutexLock.cxx
    ${SRC_DIR}itkMultiThreader.cxx
    )


INCLUDE_DIRECTORIES(
  ${MULTI_THREADING_BINARY_DIR}
  ${MULTI_THREADING_SOURCE_DIR}
)
#MESSAGE("${MULTI_THREADING_BINARY_DIR}")
ADD_LIBRARY(MultiThreading
  ${MULTI_THREADING_HDRS}
  ${MULTI_THREADING_SRCS}
  )

#
# linker flags/libraries set by the FindThreads module
#
IF(CMAKE_HAVE_THREADS_LIBRARY)
  SET(MULTI_THREADING_REQUIRED_LINK_FLAGS "${CMAKE_THREAD_LIBS_INIT}")
ENDIF(CMAKE_HAVE_THREADS_LIBRARY)

TARGET_LINK_LIBRARIES(MultiThreading
  ${MULTI_THREADING_REQUIRED_LINK_FLAGS}
  )

#
#-----------------------------------------------------------------------------
# Check platform and generate multiThreadingConfigure.h
#

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  SET(ITK_USE_PTHREADS 1)
  SET(ITK_PLATFORM_MACOSX 1)
endif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  SET(ITK_USE_PTHREADS 1)
  SET(ITK_PLATFORM_LINUX 1)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  SET(ITK_USE_WIN32_THREADS 1)
  SET(ITK_PLATFORM_WIN32 1)
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")

if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
  SET(ITK_USE_PTHREADS 1)
  SET(ITK_PLATFORM_QNX 1)
endif(CMAKE_SYSTEM_NAME STREQUAL "QNX")

CONFIGURE_FILE(${MULTI_THREADING_SOURCE_DIR}/MultiThreadingConfigure.h.in
               ${MULTI_THREADING_BINARY_DIR}/itkConfigure.h)
