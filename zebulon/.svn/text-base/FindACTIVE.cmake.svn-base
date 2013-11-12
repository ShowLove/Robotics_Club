# Macros/Scripts for locating ACTIVE Files

IF (NOT FIND_ACTIVE_SCRIPT_LOADED)
	SET(FIND_ACTIVE_SCRIPT_LOADED TRUE)

# Clear return values in case the module is loaded more than once.
#SET(ACTIVE_FOUND FALSE)		# Found?
#SET(ACTIVE_SET_DEPENDENCY TRUE)	# Build as dependency
#SET(ACTIVE_DEPENDENCIES "")		# Value to use (name of project) for dependency.
SET(ACTIVE_CXX_FLAGS "")		# Compiler Flags
SET(ACTIVE_BINARIES "")			# Binary files
IF(WIN32)
	IF(NOT MSVC AND NOT MINGW)
	MESSAGE(STATUS "YOU MUST CALL PROJECT BEFORE INCLUDING THIS FILE.")
	ENDIF()
ENDIF()

OPTION(USE_SOLUTION_FOLDERS "Use solution folders (e.g. Visual Studio)" TRUE)

IF(USE_SOLUTION_FOLDERS)
	SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
ENDIF()

IF(WIN32)
	IF(MINGW)
		SET(ACTIVE_LIBRARY_PREFIX "")
		SET(ACTIVE_ARCHIVE_EXTENSION ".dll.a")
	ELSE()
		SET(ACTIVE_LIBRARY_PREFIX "lib")
		SET(ACTIVE_ARCHIVE_EXTENSION ".lib")
	ENDIF(MINGW)
	SET(ACTIVE_BINARY_EXTENSION ".dll")
ELSE()
	SET(ACTIVE_LIBRARY_PREFIX "")
	SET(ACTIVE_ARCHIVE_EXTENSION ".so")
	SET(ACTIVE_BINARY_EXTENSION "")
ENDIF(WIN32)
#MESSAGE(STATUS "Lib Prefix: " ${ACTIVE_LIBRARY_PREFIX} " Ext: " ${ACTIVE_ARCHIVE_EXTENSION})

FIND_PATH(ACTIVE_ROOT_DIR 
			NAMES 	
				cxutils
				jaus++
			PATHS
				"./"
				${ACTIVE_MACROS_ROOT_DIR}
				"../../../../../libraries/"
				"../../../../libraries/"
				"C:\\Active\\libraries"
				"C:\\Active\\libraries"
				"E:\\software\\active\\libraries"
				"C:\\software\\Active\\libraries"
				"C:\\Active-IST\\libraries"
				"C:\\Active-IST\\libraries"
				"E:\\software\\active-ist\\libraries"
				"C:\\software\\Active-IST\\libraries"
				"~/Software/active/libraries"				
				"~/software/active/libraries"
				"~/Software/active-ist/libraries"
				"~/software/active-ist/libraries"
                "~/vc/active-ist/libraries"
                                "~/active-ist/libraries"
                                "~/active/libraries"
                                "~/svn/active/libraries"
                                "~/svn/active-ist/libraries"
				)
				
SET(USING_ZEBULON TRUE)

IF(ACTIVE_ROOT_DIR)
	SET(ACTIVE_FOUND TRUE)
ENDIF(ACTIVE_ROOT_DIR)

######################################################
#  Use the MACRO 
#  ACTIVE_ADD_SUBDIRECTORY(PACKAGE BIN_DIR)
#  to add the CMakeLists.txt file for a package
#  to your project.  All include paths will be added
#  to ACTIVE_INCLUDE_DIRS, ACTIVE_LIBRARY_DIRS, 
#  and for each specific package
#  ${PACKAGE}_INCLUDE_DIRS
#  ${PACKAGE}_LIBRARY_DIRS
#  ${PACKAGE}_BINARIES
#  ${PACKAGE}_LIBRARIES
#  ${PACKAGE}_SET_DEPENDENCY
#  ${PACKAGE}_DEPENDENCY (to add dependency)
######################################################


######################################################
# To add a new library, add its name to this
# list, then add the path to its root folder
# to the ACTIVE_PACKAGE_DIRS list.  Also,
# for each package we must have a variable
# called ${PACKAGE}_PACKAGES that lists
# the subfolders/libraries of the package.
######################################################
SET(ACTIVE_PACKAGES
		CxUtils
		TinyXML
		JAUS
		Cartographer
		LogTools
		PhysioSync
		OptiTrack
		Gesture
		Wii
		Kinect
		AudioGen
		SimView
        OSGWidgetX
        ScreenCapture
		Glove)
# Order must match order of ACTIVE_PACKAGES
SET(ACTIVE_PACKAGE_DIRS
		${ACTIVE_ROOT_DIR}/cxutils
		${ACTIVE_ROOT_DIR}/tinyxml
		${ACTIVE_ROOT_DIR}/jaus++
		${ACTIVE_ROOT_DIR}/cartographer
		${ACTIVE_ROOT_DIR}/logtools
		${ACTIVE_ROOT_DIR}/opss
		${ACTIVE_ROOT_DIR}/optitrack
		${ACTIVE_ROOT_DIR}/gesture
		${ACTIVE_ROOT_DIR}/wii
		${ACTIVE_ROOT_DIR}/kinect
		${ACTIVE_ROOT_DIR}/audiogen
		${ACTIVE_ROOT_DIR}/simview
		${ACTIVE_ROOT_DIR}/osgwidgetx
		${ACTIVE_ROOT_DIR}/screencapture
		${ACTIVE_ROOT_DIR}/glove)
# Order must match order of ACTIVE_PACKAGES
# These are the latest or most stable version numbers.
SET(ACTIVE_PACKAGE_VERSIONS
		trunk
		trunk
		trunk
		trunk
		trunk
		trunk
		trunk
		trunk
		trunk
		branches/1.0
		trunk
		trunk
		trunk
		trunk
		trunk)
		
######################################################
#  For each Package, list the subfolders/library 
#  names associated with them (some have multiple)
######################################################

# CxUtils
SET(CxUtils_PACKAGES
	cxutils)
# TinyXML
SET(TinyXML_PACKAGES tinyxml)
# JAUS
SET(JAUS_PACKAGES
	jauscore
	jausmobility
	jausextras)
# Cartographer
SET(Cartographer_PACKAGES cartographer)
# LogTools
SET(LogTools_PACKAGES logtools)
# PhysioSync
SET(PhysioSync_PACKAGES 
		physio
		abm)
# OptiTrack
SET(OptiTrack_PACKAGES
		optitrack)
SET(Wii_PACKAGES wii)
SET(Kinect_PACKAGES kinect)
SET(Gesture_PACKAGES gesture)
# AudioGen
SET(AudioGen_PACKAGES
	audiogen)
# SimView
SET(SimView_PACKAGES
	simview)
SET(OSGWidgetX_PACKAGES
	osgwidgetx)
SET(ScreenCapture_PACKAGES
	screencapture)
	
IF (USING_ZEBULON)
    option(ACTIVE_INCLUDE_ALL_FOLDERS "Include all ACTIVE Package Header Folders" TRUE)
ELSE()
    option(ACTIVE_INCLUDE_ALL_FOLDERS "Include all ACTIVE Package Header Folders" FALSE)
ENDIF()

SET(PKG_INDEX 0)
FOREACH(PKG ${ACTIVE_PACKAGES})
	LIST(GET ACTIVE_PACKAGE_DIRS ${PKG_INDEX} DIR)
	FILE(GLOB ACTIVE_${PKG}_AVAILABLE_VERSIONS RELATIVE ${DIR} ${DIR}/branches/*)
	SET(ACTIVE_${PKG}_AVAILABLE_VERSIONS
			"trunk"
			${ACTIVE_${PKG}_AVAILABLE_VERSIONS})
	LIST(REMOVE_ITEM ACTIVE_${PKG}_AVAILABLE_VERSIONS "branches/.svn")
	MATH(EXPR PKG_INDEX "${PKG_INDEX} + 1")
ENDFOREACH(PKG ${ACTIVE_PACKAGES})

MACRO(ACTIVE_ADD_SUBDIRECTORY_VER PACKAGE BINARY_DIR SOURCE_VER)
	IF(ACTIVE_ROOT_DIR AND NOT ACTIVE_${PACKAGE}_SUBDIRECTORY_ADDED)
		SET(PKG_INDEX 0)
		FOREACH(PKG ${ACTIVE_PACKAGES})
			#MESSAGE("${PKG} =?= ${PACKAGE}")
			# See if package exists
			IF(${PKG} MATCHES ${PACKAGE})
				# Found package, use index in list to get additional
				# information like ROOT_DIR and version number.
				
				IF(NOT ${SOURCE_VER} MATCHES "DEFAULT")
					SET(PKG_VERSION ${SOURCE_VER})
				ELSE()
					LIST(GET ACTIVE_PACKAGE_VERSIONS ${PKG_INDEX} PKG_VERSION)
					IF(NOT PKG_VERSION)
						SET(PKG_VERSION "trunk")
					ENDIF()
				ENDIF()
				
				#MESSAGE(STATUS "Package Version: ${PKG_VERSION}")
				
				LIST(GET ACTIVE_PACKAGE_DIRS ${PKG_INDEX} DIR)
				SET(ACTIVE_${PKG}_ROOT_DIR ${DIR}/${PKG_VERSION})

				# Setup include folders.
				SET(ACTIVE_INCLUDE_DIRS 
					${ACTIVE_INCLUDE_DIRS}
					${ACTIVE_${PKG}_ROOT_DIR}/include)
				SET(${PACKAGE}_INCLUDE_DIRS
					${ACTIVE_${PKG}_ROOT_DIR}/include)
				#MESSAGE(STATUS "${${PACKAGE}_INCLUDE_DIRS}")
				IF(ACTIVE_INCLUDE_ALL_FOLDERS)
					INCLUDE_DIRECTORIES(${${PACKAGE}_INCLUDE_DIRS})
				ENDIF()
				# Setup library folders.
				SET(ACTIVE_LIBRARY_DIRS 
					${ACTIVE_LIBRARY_DIRS}
					${ACTIVE_${PKG}_ROOT_DIR}/lib)
				SET(${PACKAGE}_LIBRARY_DIRS
					${ACTIVE_${PKG}_ROOT_DIR}/lib)
				#MESSAGE(STATUS ${${PACKAGE}_LIBRARY_DIRS})
				
				# Setup libraries and dependencies
				FOREACH(LIB_PKG ${${PACKAGE}_PACKAGES})
					# Libraries
					
					# Only TinyXML is static lib
					IF(WIN32)
						IF(MINGW AND ${PACKAGE} MATCHES TinyXML)
							SET(${PACKAGE}_LIBRARIES
								${${PACKAGE}_LIBRARIES}
								optimized ${ACTIVE_LIBRARY_PREFIX}${LIB_PKG}.a
								debug ${ACTIVE_LIBRARY_PREFIX}${LIB_PKG}_d.a)
						ELSE()
							SET(${PACKAGE}_LIBRARIES
								${${PACKAGE}_LIBRARIES}
								optimized ${ACTIVE_LIBRARY_PREFIX}${LIB_PKG}${ACTIVE_ARCHIVE_EXTENSION}
								debug ${ACTIVE_LIBRARY_PREFIX}${LIB_PKG}_d${ACTIVE_ARCHIVE_EXTENSION})
						ENDIF()
					ELSE()
						SET(${PACKAGE}_LIBRARIES
							${${PACKAGE}_LIBRARIES}
							optimized ${ACTIVE_LIBRARY_PREFIX}${LIB_PKG}${ACTIVE_ARCHIVE_EXTENSION}
							debug ${ACTIVE_LIBRARY_PREFIX}${LIB_PKG}_d${ACTIVE_ARCHIVE_EXTENSION})
					ENDIF()
					#MESSAGE(STATUS ${${PACKAGE}_LIBRARIES})
					# Binaries (DLL's)
					IF(WIN32 AND NOT UNIX)
						SET(${PACKAGE}_BINARIES
							${${PACKAGE}_BINARIES}
							${ACTIVE_LIBRARY_PREFIX}${LIB_PKG}${ACTIVE_BINARY_EXTENSION}
							${ACTIVE_LIBRARY_PREFIX}${LIB_PKG}_d${ACTIVE_BINARY_EXTENSION})
					ELSE()
						SET(${PACKAGE}_BINARIES
								${${PACKAGE}_LIBRARIES})
					ENDIF(WIN32 AND NOT UNIX)
					# CMakeLists.txt files
					#MESSAGE(STATUS "${ACTIVE_${PKG}_ROOT_DIR}/build/cmake/lib${LIB_PKG}")
					IF(EXISTS ${ACTIVE_${PKG}_ROOT_DIR}/build/cmake/lib${LIB_PKG})
						SET(${PACKAGE}_ADD_DEPENDENCY TRUE)
						SET(${PACKAGE}_SET_DEPENDENCY TRUE)
						SET(${PACKAGE}_DEPENDENCY 
								${${PACKAGE}_DEPENDENCY}
								${ACTIVE_LIBRARY_PREFIX}${LIB_PKG})
						SET(${PACKAGE}_FOUND TRUE)
						#MESSAGE(STATUS ${${PACKAGE}_DEPENDENCY})
						ADD_SUBDIRECTORY(${ACTIVE_${PKG}_ROOT_DIR}/build/cmake/lib${LIB_PKG} 
											./${BINARY_DIR}/${PACKAGE_BINARY_DIR}/${PACKAGE}/lib${LIB_PKG})
						SET(ACTIVE_${PACKAGE}_SUBDIRECTORY_ADDED TRUE)
						SET(${PACKAGE}_CMAKE_SUBDIRECTORY_ADDED TRUE)
						# Solution folders.
						IF(USE_SOLUTION_FOLDERS)
							SET_PROPERTY(TARGET ${ACTIVE_LIBRARY_PREFIX}${LIB_PKG} PROPERTY FOLDER "Libraries")
						ENDIF()
						#MESSAGE(STATUS "Added Package: ${PACKAGE}")
					ENDIF()
					
				ENDFOREACH()
				#MESSAGE(STATUS ${${PACKAGE}_LIBRARIES})
				
			ENDIF(${PKG} MATCHES ${PACKAGE})
			MATH(EXPR PKG_INDEX "${PKG_INDEX} + 1")
		ENDFOREACH(PKG ${ACTIVE_PACKAGES})

		IF(NOT ACTIVE_${PACKAGE}_SUBDIRECTORY_ADDED)
			MESSAGE(STATUS "Failed to Add Package Subdirectory: ${PACKAGE}")
		ELSE()
			MESSAGE(STATUS "Added Package: ${PACKAGE} Version: ${PKG_VERSION}")
		ENDIF(NOT ACTIVE_${PACKAGE}_SUBDIRECTORY_ADDED)
	ENDIF()
	
ENDMACRO(ACTIVE_ADD_SUBDIRECTORY_VER PACKAGE BINARY_DIR SOURCE_DIR)

MACRO(ACTIVE_ADD_SUBDIRECTORY PACKAGE BINARY_DIR)
	ACTIVE_ADD_SUBDIRECTORY_VER(${PACKAGE} ${BINARY_DIR} "DEFAULT")
ENDMACRO(ACTIVE_ADD_SUBDIRECTORY PACKAGE BINARY_DIR)	

ENDIF(NOT FIND_ACTIVE_SCRIPT_LOADED)
