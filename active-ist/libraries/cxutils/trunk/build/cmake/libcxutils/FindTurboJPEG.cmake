# Macros/Scripts for locating JPEG SDK Files
# This macro determines what version of JPEG is installed on the
# the system or available and configures the appropriate options
# for CxUtils.

# Clear return values in case the module is loaded more than once.
#SET(TurboJPEG_CXX_FLAGS "")	# No JPEG Files located yet
SET(TurboJPEG_BINARIES "")	# Binary files

# Don't run multiple times.
IF(NOT TurboJPEG_FOUND)
	IF(WIN32)
		# Use precompiled binary of libjpeg-turbo.
		IF(MSVC90 OR MSVC10) 
			SET(TurboJPEG_SET_DEPENDENCY FALSE)
			SET(TurboJPEG_INCLUDE_DIRS 
						../../../ext/libjpeg-turbo-1.0.1
						../../../ext/libjpeg-turbo-1.0.1/win)
			IF(MSVC90)
				SET(TurboJPEG_LIBRARIES libjpegturbo.lib)
			ELSE()
				SET(TurboJPEG_LIBRARIES libjpegturbo_vs10.lib)
			ENDIF(MSVC90)
			IF(CMAKE_CL_64) # Get version for architecture type
				FIND_PATH(TurboJPEG_LIBRARY_DIRS ${TurboJPEG_LIBRARIES} ../../../ext/libjpeg-turbo-1.0.1/libs/x64)
			ELSE()
				FIND_PATH(TurboJPEG_LIBRARY_DIRS ${TurboJPEG_LIBRARIES} ../../../ext/libjpeg-turbo-1.0.1/libs/x32)
			ENDIF(CMAKE_CL_64)
			IF(TurboJPEG_LIBRARY_DIRS)
				SET(TurboJPEG_LIBRARIES  ${TurboJPEG_LIBRARY_DIRS}/${TurboJPEG_LIBRARIES})
				SET(TurboJPEG_FOUND TRUE)
				SET(TurboJPEG_SET_DEPENDENCY FALSE)
				SET(TurboJPEG_DEPENDENCY "")
				MESSAGE(STATUS "JPEG Turbo Found")
			ELSE()
				SET(TurboJPEG_INCLUDE_DIRS 
						../../../ext/jpeg-6b/msvc)
				ADD_SUBDIRECTORY(../../../ext/jpeg-6b/ ./cmake.jpeg-6b/)
				SET(TurboJPEG_LIBRARY_DIRS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}) 
				IF(MSVC)
					SET(TurboJPEG_LIBRARIES libcxjpeg-6b)
					SET(TurboJPEG_DEPENDENCY libcxjpeg-6b)
				ELSE()
					SET(TurboJPEG_LIBRARIES cxjpeg-6b)
					SET(TurboJPEG_DEPENDENCY cxjpeg-6b)
				ENDIF(MSVC)
			ENDIF(TurboJPEG_LIBRARY_DIRS)
		# Build version of jpeg-6b that uses BGR ordering instead of RGB
		ELSE()     
			SET(TurboJPEG_INCLUDE_DIRS 
						../../../ext/jpeg-6b/msvc)
			ADD_SUBDIRECTORY(../../../ext/jpeg-6b/ ./cmake.jpeg-6b/)
			SET(TurboJPEG_LIBRARY_DIRS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}) 
			IF(MSVC)
				SET(TurboJPEG_LIBRARIES libcxjpeg-6b)
				SET(TurboJPEG_DEPENDENCY libcxjpeg-6b)
			ELSE()
				SET(TurboJPEG_LIBRARIES cxjpeg-6b)
				SET(TurboJPEG_DEPENDENCY cxjpeg-6b)
			ENDIF(MSVC)
		ENDIF(MSVC90 OR MSVC10)
	ELSE(UNIX)

		FIND_PATH(TurboJPEG_INCLUDE_DIRS 
					NAMES
					turbojpeg.h 
					PATHS
					/opt/libjpeg-turbo/include)

		FIND_PATH(TurboJPEG_LIBRARY_DIRS 
						NAMES
						libjpeg.so 
						PATHS
						/opt/libjpeg-turbo/lib64
						/opt/libjpeg-turbo/lib )

		#SET(TurboJPEG_LIBRARIES ${TurboJPEG_LIBRARY_DIRS}/libjpeg.so)
		IF(MSVC AND NOT CMAKE_BUILD_TYPE MATCHES "Debug" AND TurboJPEG_INCLUDE_DIRS AND TurboJPEG_LIBRARY_DIRS)
			SET(TurboJPEG_LIBRARIES "${TurboJPEG_LIBRARY_DIRS}/libjpeg.so")
			MESSAGE(STATUS ${TurboJPEG_LIBRARIES})
			SET(TurboJPEG_SET_DEPENDENCY FALSE)
			SET(TurboJPEG_CXX_FLAGS -DJPEG_TURBO_DIR="${TurboJPEG_INCLUDE_DIRS}/jpeglib.h")
			SET(TurboJPEG_FOUND TRUE)
			SET(TurboJPEG_SET_DEPENDENCY FALSE)
			SET(TurboJPEG_DEPENDENCY "")
			#MESSAGE(STATUS "JPEG Turbo Found")
		ELSE()
			SET(TurboJPEG_INCLUDE_DIRS 
						../../../ext/jpeg-6b/)
			
			ADD_SUBDIRECTORY(../../../ext/jpeg-6b/ ./cmake.jpeg-6b/)
			SET(TurboJPEG_LIBRARY_DIRS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}) 
			SET(TurboJPEG_LIBRARIES cxjpeg-6b)
			SET(TurboJPEG_DEPENDENCY cxjpeg-6b)
		ENDIF()
	ENDIF(WIN32)

	INCLUDE_DIRECTORIES(${TurboJPEG_INCLUDE_DIRS})
	#MESSAGE(STATUS ${TurboJPEG_LIBRARY_DIRS})
	LINK_DIRECTORIES(${TurboJPEG_LIBRARY_DIRS})
	IF(TurboJPEG_CXX_FLAGS)
		ADD_DEFINITIONS(${TurboJPEG_CXX_FLAGS})
	ENDIF()
ELSE()
	MESSAGE(STATUS "JPEG Turbo Found Already Found")
ENDIF()


