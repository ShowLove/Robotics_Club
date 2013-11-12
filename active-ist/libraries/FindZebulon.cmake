# Macros/Scripts for locating Zebulon Files

# Clear return values in case the module is loaded more than once.
#SET(Zebulon_FOUND FALSE)			# Found?
#SET(Zebulon_SET_DEPENDENCY TRUE)	# Build as dependency
#SET(Zebulon_DEPENDENCIES "")		# Value to use (name of project) for dependency.
SET(Zebulon_CXX_FLAGS "")			# Compiler Flags
SET(Zebulon_BINARIES "")			# Binary files
SET(Zebulon_VERSION 4)

IF(WIN32)
	SET(Zebulon_LIBRARY_PREFIX "lib")
	SET(Zebulon_ARCHIVE_EXTENSION ".lib")
	SET(Zebulon_BINARY_EXTENSION ".dll")
ELSE()
	SET(Zebulon_LIBRARY_PREFIX "")
	SET(Zebulon_ARCHIVE_EXTENSION ".so")
	SET(Zebulon_BINARY_EXTENSION "")
ENDIF()

FIND_PATH(Zebulon_ROOT_DIR 
			NAMES 	hardware
					sensors
					utility
			PATHS
				"C:\\Zebulon\\v4\\software\\"
				"C:\\Zebulon\\software\\"
				"C:\\Zebulon\\"
				"E:\\software\\Zebulon\\v4\\software\\"
				"C:\\software\\Zebulon\\v4\\software\\"
				"C:\\software\\Zebulon\\software\\"
				"~/Software/zebulon/v4/software"
				"~/Software/zebulon/software"
				"~/software/zebulon/v4/software"
				"~/software/zebulon/software"
				"~/software/zebulon/"
                "~/Software/zebulon/"
                "~/zebulon/"
                "~/vc/zebulon/v4/software/")
				
IF(Zebulon_ROOT_DIR)
	SET(Zebulon_FOUND TRUE)
ENDIF(Zebulon_ROOT_DIR)

SET(Zebulon_SENSORS_COMPASS_PACKAGES 
		compass
		compassfactory
		coralahrs
		microstrain
		tcm250
		inclinometer)
SET(Zebulon_SENSORS_GPS_PACKAGES 
		gps
		gpsfactory
		nmea)
SET(Zebulon_SENSORS_LASER_PACKAGES 
		laser
		laserfactory
		sick
		hokuyo)
SET(Zebulon_SENSORS_COMPASS_PACKAGE_DIRS 
			${Zebulon_ROOT_DIR}/sensors/compass/abstract
			${Zebulon_ROOT_DIR}/sensors/compass/abstract
			${Zebulon_ROOT_DIR}/sensors/compass/coralahrs
			${Zebulon_ROOT_DIR}/sensors/compass/microstrain
			${Zebulon_ROOT_DIR}/sensors/compass/tcm250
			${Zebulon_ROOT_DIR}/sensors/compass/inclinometer)
SET(Zebulon_SENSORS_GPS_PACKAGE_DIRS 
			${Zebulon_ROOT_DIR}/sensors/gps/abstract
			${Zebulon_ROOT_DIR}/sensors/gps/abstract
			${Zebulon_ROOT_DIR}/sensors/gps/nmea)
SET(Zebulon_SENSORS_LASER_PACKAGE_DIRS 
			${Zebulon_ROOT_DIR}/sensors/laser/abstract
			${Zebulon_ROOT_DIR}/sensors/laser/abstract
			${Zebulon_ROOT_DIR}/sensors/laser/sick
			${Zebulon_ROOT_DIR}/sensors/laser/hokuyo)

# To add a new library, add its name to this
# list, then add the path to its root folder
# to the Zebulon_PACKAGE_DIRS list.
SET(Zebulon_PACKAGES
		mcu
		dynamixel
		minissc 
		roboteq 
		sabertooth
		cvcapture
		${Zebulon_SENSORS_COMPASS_PACKAGES}
		${Zebulon_SENSORS_GPS_PACKAGES}
		${Zebulon_SENSORS_LASER_PACKAGES}
		mixer 
		encoders 
		filter
		xmlconfig 
		baseline
		dvision 
		gvision
		cvision
		homography
		statemachine
		display 
		grapher
		)
# Order must match order of Zebulon_PACKAGES
SET(Zebulon_PACKAGE_DIRS
		${Zebulon_ROOT_DIR}/hardware/microcontroller/mcu
		${Zebulon_ROOT_DIR}/hardware/motor/dynamixel
		${Zebulon_ROOT_DIR}/hardware/motor/minissc
		${Zebulon_ROOT_DIR}/hardware/motor/roboteq
		${Zebulon_ROOT_DIR}/hardware/motor/sabertooth
		${Zebulon_ROOT_DIR}/sensors/video/cvcapture
		${Zebulon_SENSORS_COMPASS_PACKAGE_DIRS}
		${Zebulon_SENSORS_GPS_PACKAGE_DIRS}
		${Zebulon_SENSORS_LASER_PACKAGE_DIRS}
		${Zebulon_ROOT_DIR}/utility/math/mixer
		${Zebulon_ROOT_DIR}/utility/math/encoders
		${Zebulon_ROOT_DIR}/utility/math/filter
		${Zebulon_ROOT_DIR}/utility/configuration/xmlconfig
		${Zebulon_ROOT_DIR}/utility/configuration/baseline
		${Zebulon_ROOT_DIR}/utility/vision/dvision
		${Zebulon_ROOT_DIR}/utility/vision/gvision
		${Zebulon_ROOT_DIR}/utility/vision/cvision
		${Zebulon_ROOT_DIR}/utility/vision/homography
		${Zebulon_ROOT_DIR}/utility/AI/statemachine
		${Zebulon_ROOT_DIR}/utility/visualizations/display
		${Zebulon_ROOT_DIR}/utility/visualizations/grapher
		)

MACRO(ZEBULON_ADD_SUBDIRECTORY PACKAGE BINARY_DIR)
	# There is a special case in windows for CvCapture where we
	# want to use a precompiled binary.
	IF(WIN32 AND ${PACKAGE} MATCHES cvcapture)
		SET(Zebulon_INCLUDE_DIRS 
				${Zebulon_INCLUDE_DIRS} 
				${Zebulon_ROOT_DIR}/sensors/video/cvcapture/include)
		SET(Zebulon_LIBRARY_DIRS 
				${Zebulon_ROOT_DIR}/sensors/video/cvcapture/lib)
		
		IF(MSVC10)
			SET(VC_POSTFIX "_vc10")
		ENDIF()
		SET(Zebulon_LIBRARIES 
			${Zebulon_LIBRARIES}
			optimized ${Zebulon_ROOT_DIR}/sensors/video/cvcapture/lib/${Zebulon_LIBRARY_PREFIX}${PACKAGE}${VC_POSTFIX}${Zebulon_ARCHIVE_EXTENSION}
			debug ${Zebulon_ROOT_DIR}/sensors/video/cvcapture/lib/${Zebulon_LIBRARY_PREFIX}${PACKAGE}${VC_POSTFIX}_d${Zebulon_ARCHIVE_EXTENSION})
		SET(Zebulon_BINARIES
				${Zebulon_BINARIES}
					${Zebulon_ROOT_DIR}/sensors/video/cvcapture/bin/${Zebulon_LIBRARY_PREFIX}${PACKAGE}${VC_POSTFIX}${Zebulon_BINARY_EXTENSION}
					${Zebulon_ROOT_DIR}/sensors/video/cvcapture/bin/${Zebulon_LIBRARY_PREFIX}${PACKAGE}${VC_POSTFIX}_d${Zebulon_BINARY_EXTENSION})
		#MESSAGE(STATUS ${Zebulon_LIBRARIES})
		#MESSAGE(STATUS ${Zebulon_BINARIES})
		EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy 
						${Zebulon_ROOT_DIR}/sensors/video/cvcapture/bin/${Zebulon_LIBRARY_PREFIX}${PACKAGE}${VC_POSTFIX}${Zebulon_BINARY_EXTENSION}
						${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
						RESULT_VARIABLE COPY_RESULT
						ERROR_VARIABLE ERROR)
		#MESSAGE(STATUS ${COPY_RESULT} ${ERROR})
		EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy 
						${Zebulon_ROOT_DIR}/sensors/video/cvcapture/bin/${Zebulon_LIBRARY_PREFIX}${PACKAGE}${VC_POSTFIX}_d${Zebulon_BINARY_EXTENSION}
						${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
						RESULT_VARIABLE COPY_RESULT
						ERROR_VARIABLE ERROR)
		#MESSAGE("${Zebulon_ROOT_DIR}/sensors/video/cvcapture/include")
		INCLUDE_DIRECTORIES(${Zebulon_ROOT_DIR}/sensors/video/cvcapture/include)
		MESSAGE(STATUS "Added Zebulon Package: ${PACKAGE}")
	ELSEIF(NOT Zebulon_${PACKAGE}_SUBDIRECTORY_ADDED)
		SET(PKG_INDEX 0)
		FOREACH(PKG ${Zebulon_PACKAGES})
			#MESSAGE("${PKG}")
			# See if package exists
			IF(${PKG} MATCHES ${PACKAGE})
				SET(DIR_INDEX 0)
				# Get root directory.
				FOREACH(DIR ${Zebulon_PACKAGE_DIRS})
					IF(${DIR_INDEX} MATCHES ${PKG_INDEX})
						SET(Zebulon_${PKG}_ROOT_DIR ${DIR})
						#MESSAGE("${Zebulon_${PKG}_ROOT_DIR}")
						BREAK()
					ENDIF()
					MATH(EXPR DIR_INDEX "${DIR_INDEX} + 1")
				ENDFOREACH(DIR)
				SET(Zebulon_CMAKELISTS 	
						${Zebulon_CMAKELISTS} 
						"${Zebulon_${PKG}_ROOT_DIR}/build/cmake/lib${PACKAGE}")
				SET(Zebulon_INCLUDE_DIRS 
						${Zebulon_INCLUDE_DIRS} 
						"${Zebulon_${PKG}_ROOT_DIR}/include")
				
				# For any of the "Factory" packages, add the
				# directories for its dependencies to the global
				# include lists.
				IF(${PACKAGE} MATCHES compassfactory)
					FOREACH(DIR ${Zebulon_SENSORS_COMPASS_PACKAGE_DIRS})
						SET(Zebulon_INCLUDE_DIRS ${Zebulon_INCLUDE_DIRS} ${DIR}/include)
					ENDFOREACH()
					INCLUDE_DIRECTORIES(${Zebulon_INCLUDE_DIRS})
				ELSEIF(${PACKAGE} MATCHES gpsfactory)
					FOREACH(DIR ${Zebulon_SENSORS_GPS_PACKAGE_DIRS})
						SET(Zebulon_INCLUDE_DIRS ${Zebulon_INCLUDE_DIRS} ${DIR}/include)
					ENDFOREACH()
					INCLUDE_DIRECTORIES(${Zebulon_INCLUDE_DIRS})
				ELSEIF(${PACKAGE} MATCHES laserfactory)
					FOREACH(DIR ${Zebulon_SENSORS_LASER_PACKAGE_DIRS})
						SET(Zebulon_INCLUDE_DIRS ${Zebulon_INCLUDE_DIRS} ${DIR}/include)
					ENDFOREACH()
					INCLUDE_DIRECTORIES(${Zebulon_INCLUDE_DIRS})
				ENDIF()
				
				SET(Zebulon_LIBRARY_DIRS 
						${Zebulon_LIBRARY_DIRS} "${Zebulon_${PKG}_ROOT_DIR}/lib")
				SET(Zebulon_SET_DEPENDENCY TRUE)
				SET(Zebulon_DEPENDENCIES 
						${Zebulon_DEPENDENCIES} ${Zebulon_LIBRARY_PREFIX}${PACKAGE})
				SET(Zebulon_LIBRARIES 
						${Zebulon_LIBRARIES}
						optimized ${Zebulon_LIBRARY_PREFIX}${PACKAGE}${Zebulon_ARCHIVE_EXTENSION}
						debug ${Zebulon_LIBRARY_PREFIX}${PACKAGE}_d${Zebulon_ARCHIVE_EXTENSION})

				ADD_SUBDIRECTORY("${Zebulon_${PKG}_ROOT_DIR}/build/cmake/lib${PACKAGE}" ${BINARY_DIR})
				SET(Zebulon_${PACKAGE}_SUBDIRECTORY_ADDED TRUE)
				MESSAGE(STATUS "Added Zebulon Package: ${PACKAGE}")
				INCLUDE_DIRECTORIES("${Zebulon_${PKG}_ROOT_DIR}/include")
				BREAK()
			ENDIF()
			MATH(EXPR PKG_INDEX "${PKG_INDEX} + 1")
		ENDFOREACH(PKG)
		
		IF(NOT Zebulon_${PACKAGE}_SUBDIRECTORY_ADDED)
			MESSAGE(STATUS "Failed to Add Package: ${PACKAGE}")
		ELSE()
			#MESSAGE(STATUS "Added Package: ${PACKAGE}")
		ENDIF(NOT Zebulon_${PACKAGE}_SUBDIRECTORY_ADDED)
	
	ENDIF()
	
	
	
ENDMACRO(ZEBULON_ADD_SUBDIRECTORY)

##################################################################
# MACROS BELOW THIS POINT ARE DEPRECATED AND SHOULD
# BE AVOIDED.
##################################################################	

##################################################################
# HARDWARE MACROS
##################################################################	
MACRO(ZEBULON_ADD_MICROCONTROLLER_CMAKELISTS PACKAGE BINARY_DIR)
	ZEBULON_ADD_SUBDIRECTORY(${PACKAGE} ${BINARY_DIR})
ENDMACRO(ZEBULON_ADD_MICROCONTROLLER_CMAKELISTS)

MACRO(ZEBULON_ADD_MOTOR_CMAKELISTS PACKAGE BINARY_DIR)
	ZEBULON_ADD_SUBDIRECTORY(${PACKAGE} ${BINARY_DIR})
ENDMACRO(ZEBULON_ADD_MOTOR_CMAKELISTS)

MACRO(ZEBULON_ADD_BASELINE_CMAKELISTS PACKAGE BINARY_DIR)
	ZEBULON_ADD_SUBDIRECTORY(${PACKAGE} ${BINARY_DIR})
ENDMACRO(ZEBULON_ADD_BASELINE_CMAKELISTS)

##################################################################
# SENSOR MACROS
##################################################################	
MACRO(ZEBULON_ADD_VIDEO_CMAKELISTS PACKAGE BINARY_DIR)
		ZEBULON_ADD_SUBDIRECTORY(${PACKAGE} ${BINARY_DIR})
ENDMACRO(ZEBULON_ADD_VIDEO_CMAKELISTS)

MACRO(ZEBULON_ADD_SENSORS_COMPASS_CMAKELISTS PACKAGE BINARY_DIR)
	ZEBULON_ADD_SUBDIRECTORY(${PACKAGE} ${BINARY_DIR})
ENDMACRO(ZEBULON_ADD_SENSORS_COMPASS_CMAKELISTS)

MACRO(ZEBULON_ADD_SENSORS_GPS_CMAKELISTS PACKAGE BINARY_DIR)
	ZEBULON_ADD_SUBDIRECTORY(${PACKAGE} ${BINARY_DIR})
ENDMACRO(ZEBULON_ADD_SENSORS_GPS_CMAKELISTS)

MACRO(ZEBULON_ADD_SENSORS_LASER_CMAKELISTS PACKAGE BINARY_DIR)
	ZEBULON_ADD_SUBDIRECTORY(${PACKAGE} ${BINARY_DIR})
ENDMACRO(ZEBULON_ADD_SENSORS_LASER_CMAKELISTS)

##################################################################
# UTILITY MACROS
##################################################################	
MACRO(ZEBULON_ADD_MATH_UTILITY_CMAKELISTS PACKAGE BINARY_DIR)
	ZEBULON_ADD_SUBDIRECTORY(${PACKAGE} ${BINARY_DIR})
ENDMACRO(ZEBULON_ADD_MATH_UTILITY_CMAKELISTS)

MACRO(ZEBULON_ADD_CONFIGURATION_UTILITY_CMAKELISTS PACKAGE BINARY_DIR)
	ZEBULON_ADD_SUBDIRECTORY(${PACKAGE} ${BINARY_DIR})
ENDMACRO(ZEBULON_ADD_CONFIGURATION_UTILITY_CMAKELISTS)

MACRO(ZEBULON_ADD_VISION_UTILITY_CMAKELISTS PACKAGE BINARY_DIR)
	ZEBULON_ADD_SUBDIRECTORY(${PACKAGE} ${BINARY_DIR})
ENDMACRO(ZEBULON_ADD_VISION_UTILITY_CMAKELISTS)

MACRO(ZEBULON_ADD_AI_UTILITY_CMAKELISTS PACKAGE BINARY_DIR)
	ZEBULON_ADD_SUBDIRECTORY(${PACKAGE} ${BINARY_DIR})
ENDMACRO(ZEBULON_ADD_AI_UTILITY_CMAKELISTS)

MACRO(ZEBULON_ADD_VISUALIZATIONS_UTILITY_CMAKELISTS PACKAGE BINARY_DIR)
	ZEBULON_ADD_SUBDIRECTORY(${PACKAGE} ${BINARY_DIR})
ENDMACRO(ZEBULON_ADD_VISUALIZATIONS_UTILITY_CMAKELISTS)


