# Macros/Scripts for locating JPEG SDK Files
# This macro determines what version of JPEG is installed on the
# the system or available and configures the appropriate options
# for CxUtils.

#MESSAGE(STATUS "Finding OSG SDK...")

# Clear return values in case the module is loaded more than once.
SET(PNG_FOUND TRUE)
SET(PNG_SET_DEPENDENCY TRUE)	# If true, we are building JPEG, so make dependency.
SET(PNG_DEPENDENCY "")
SET(PNG_LIBRARIES "")	# No JPEG Files located yet
SET(PNG_CXX_FLAGS "")	# No JPEG Files located yet
SET(PNG_BINARIES "")	# Binary files
SET(PNG_INCLUDE_DIRS "")# Include directories
SET(PNG_LIBRARY_DIRS "")# Library directories

IF(WIN32 OR XCODE_VERSION)
	SET(PNG_INCLUDE_DIR 
		../../../ext/zlib-1.2.3 
		../../../ext/libpng-1.2.35)
	ADD_SUBDIRECTORY(../../../ext/libpng-1.2.35/ ./cmake.libpng)
	SET(PNG_LIBRARY_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
	IF(MSVC)
		SET(PNG_BINARIES libpng.dll libzlib.dll)
		SET(PNG_DEPENDENCY libpng libzlib)
		SET(PNG_LIBRARIES libpng libzlib)
	ELSE()
		SET(PNG_DEPENDENCY png zlib)
		SET(PNG_LIBRARIES png zlib)
	ENDIF()
ELSE()
	FIND_PACKAGE(PNG)
	SET(PNG_SET_DEPENDENCY FALSE)
	SET(PNG_LIBRARIES png z)
ENDIF()

INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIR})
LINK_DIRECTORIES(${PNG_LIBRARY_DIR})
ADD_DEFINITIONS(${PNG_CXX_FLAGS})

