cmake_minimum_required(VERSION 2.8)

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


SET(MULTI_THREADING_HDRS
    itkMacro.h
    itkIndent.h
    itkTimeStamp.h
    itkSmartPointer.h
    itkSimpleFastMutexLock.h
    itkLightObject.h
    itkWin32Header.h
    itkWindows.h
    itkExceptionObject.h
    itkObjectFactoryBase.h
    itkObjectFactory.h
    itkObject.h
    itkFastMutexLock.h
    itkCreateObjectFunction.h
    itkMutexLock.h
    itkMultiThreader.h
    )

SET(MULTI_THREADING_SRCS
    itkIndent.cxx
    itkTimeStamp.cxx
    itkSimpleFastMutexLock.cxx
    itkExceptionObject.cxx
    itkObjectFactoryBase.cxx
    itkLightObject.cxx
    itkObject.cxx
    itkFastMutexLock.cxx
    itkMutexLock.cxx
    itkMultiThreader.cxx
    )


INCLUDE_DIRECTORIES(
  ${MULTI_THREADING_BINARY_DIR}
  ${MULTI_THREADING_SOURCE_DIR}
)

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
