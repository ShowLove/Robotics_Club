# Macros/Scripts for locating JPEG SDK Files
# This macro determines what version of JPEG is installed on the
# the system or available and configures the appropriate options
# for CxUtils.

#MESSAGE(STATUS "Finding OSG SDK...")

# Clear return values in case the module is loaded more than once.
SET(LibPNG_FOUND TRUE)
SET(LibPNG_SET_DEPENDENCY TRUE)	# If true, we are building JPEG, so make dependency.
SET(LibPNG_DEPENDENCY "")
SET(LibPNG_LIBRARIES "")	# No JPEG Files located yet
SET(LibPNG_CXX_FLAGS "")	# No JPEG Files located yet
SET(LibPNG_BINARIES "")	# Binary files
SET(LibPNG_INCLUDE_DIRS "")# Include directories
SET(LibPNG_LIBRARY_DIRS "")# Library directories

IF(WIN32 OR XCODE_VERSION)
	SET(LibPNG_INCLUDE_DIRS 
		../../../ext/zlib-1.2.3 
		../../../ext/libpng-1.2.35)
	ADD_SUBDIRECTORY(../../../ext/libpng-1.2.35/ ./cmake.libpng)
	SET(LibPNG_LIBRARY_DIRS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
	IF(MSVC)
		SET(LibPNG_BINARIES libpng.dll libzlib.dll)
		SET(LibPNG_DEPENDENCY libpng libzlib)
		SET(LibPNG_LIBRARIES libpng libzlib)
	ELSE()
		SET(LibPNG_DEPENDENCY png zlib)
		SET(LibPNG_LIBRARIES png zlib)
	ENDIF()
ELSE()
	FIND_PACKAGE(PNG)
	SET(LibPNG_SET_DEPENDENCY FALSE)
	SET(LibPNG_LIBRARIES png z)
ENDIF()

INCLUDE_DIRECTORIES(${LibPNG_INCLUDE_DIRS})
LINK_DIRECTORIES(${LibPNG_LIBRARY_DIRS})
ADD_DEFINITIONS(${LibPNG_CXX_FLAGS})

