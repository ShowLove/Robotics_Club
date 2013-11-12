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

# Name the project
project (CvCapture)

# Set output folders for binaries and libraries.
include(../../../../../SetOutputDirectories.cmake)
include(../../../../../FindACTIVE.cmake)
include(../../../../../FindOpenCV.cmake)
include(../../../../../FindZebulon.cmake)

# Try add CxUtils, this is required.
ACTIVE_ADD_SUBDIRECTORY(CxUtils ./libcxutils)

if(OpenCV_FOUND)
	add_definitions(-DCV_USE_OPEN_CV)
endif()

# Add the main project which is libcvcapture library
ZEBULON_ADD_SUBDIRECTORY(cvcapture ../cmake.libcvcapture)

option(BUILD_EXAMPLES "Build Examples" TRUE)

if(BUILD_EXAMPLES)
add_subdirectory(examples ../cmake.cvcapture.examples)
endif()


# CPack Installer Generation
if(WIN32)
	install(FILES ${CXUTILS_LIB_DIR}/libcxutils.lib
			DESTINATION lib
			COMPONENT CxUtils)
	install(FILES ${CXUTILS_BIN_DIR}/libcxutils.dll
			DESTINATION bin
			COMPONENT CxUtils)
	install(DIRECTORY 
			${CXUTILS_INCLUDE_DIR}
			DESTINATION ./ 
			COMPONENT CxUtils
			FILES_MATCHING PATTERN "*.h" 
			PATTERN ".svn" EXCLUDE
			)
endif(WIN32)

#CPack Windows Installer Settings.
if(WIN32)
		
	set(CPACK_PACKAGE_NAME "Zebulon Image Capture SDK")
	set(CPACK_PACKAGE_VendOR "Robotics Club at the Univeristy of Central Florida")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Robotics Club at UCF Zebulon Image Capture SDK")

	MACRO (TODAY RESULT YEAR MONTH DAY)
		if (WIN32)
			EXECUTE_PROCESS(COMMAND "cmd.exe" "/c" "date" "/T" OUTPUT_VARIABLE ${RESULT})
			string(REGEX REPLACE "....../../..(..).*" "\\1" ${YEAR} ${${RESULT}} )
			string(REGEX REPLACE "....(..)/../.*" "\\1" ${MONTH} ${${RESULT}} )
			string(REGEX REPLACE "....../(..)/.*" "\\1" ${DAY} ${${RESULT}} )
		elseif(UNIX)
			EXECUTE_PROCESS(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE ${RESULT})
			string(REGEX REPLACE "....../../..(..).*" "\\1" ${YEAR} ${${RESULT}} )
			string(REGEX REPLACE "....(..)/../.*" "\\1" ${MONTH} ${${RESULT}} )
			string(REGEX REPLACE "....../(..)/.*" "\\1" ${DAY} ${${RESULT}} )
		else (WIN32)
			MESSAGE(Send_ERROR "Date not implemented")
			SET(${RESULT} 000000)
			SET(${YEAR} 000000)
			SET(${MONTH} 000000)
			SET(${DAY} 000000)
		endif (WIN32)
	endMACRO (TODAY)

	TODAY(BUILD_DATE YEAR MONTH DAY)
	set(CPACK_PACKAGE_VERSION "3.${YEAR}${MONTH}${DAY}")

	set(CPACK_PACKAGE_INSTALL_DIRECTORY "Zebulon Image Capture SDK")
	find_file(LICENSE_FILE_NAME license.txt ../../)
	if(LICENSE_FILE_NAME)
	set(CPACK_RESOURCE_FILE_LICENSE "${LICENSE_FILE_NAME}")
	endif(LICENSE_FILE_NAME)
	set(CPACK_COMPONENT_SDK_REQUIRED "SDK" "CxUtils")
	set(CPACK_COMPONENTS_ALL SDK CxUtils)

	# This must always be last!
	include(CPack)

endif(WIN32)
