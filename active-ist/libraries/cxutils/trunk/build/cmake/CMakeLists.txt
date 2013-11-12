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

# Create a solution/workspace name and a specific library/binary name.
set(PROJECT_NAME cxutils)

set(CxUtils_VERSION 2)

# Name the project (for dependancy)
project (${PROJECT_NAME})


FIND_PATH(ACTIVE_MACROS_ROOT_DIR SetOutputDirectories.cmake
			PATHS
				../../../../
				../../../../../
				../../../../../../)
include(${ACTIVE_MACROS_ROOT_DIR}/SetOutputDirectories.cmake)

# Include the subdirectories for other projects here.
add_subdirectory(libcxutils ./cmake.libcxutils)

# Example projects are optional.
OPTION(BUILD_EXAMPLES "Build examples" TRUE)
if(BUILD_EXAMPLES)
	add_subdirectory(examples ./cmake.cxutils.examples)
endif()

# CPack Installer Generation
if(WIN32)

	TODAY(BUILD_DATE YEAR MONTH DAY)

	install(DIRECTORY 
			../../build/cmake/examples
			DESTINATION src/active-ist/libraries/cxutils/trunk/build/cmake
			COMPONENT SourceCode
			PATTERN ".svn" EXCLUDE)
	install(FILES 
			../../build/cmake/CMakeLists.txt
			DESTINATION src/active-ist/libraries/cxutils/trunk/build/cmake
			COMPONENT SourceCode)
	install(FILES 
			../../bin/calculon640.jpg
			DESTINATION src/active-ist/libraries/cxutils/trunk/bin
			COMPONENT SourceCode)

	set(CPACK_PACKAGE_NAME "CxUtils")
	set(CPACK_PACKAGE_VENDOR "Univeristy of Central Florida - IST - ACTIVE Lab")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CxUtils - Cross Platform Utilities Library")

	
	set(CPACK_PACKAGE_VERSION "${CxUtils_VERSION}.${YEAR}${MONTH}${DAY}")

	set(CPACK_PACKAGE_INSTALL_DIRECTORY "CxUtils ${CxUtils_VERSION}.${YEAR}${MONTH}${DAY}")
	# Get the license file to be included with installer
	find_file(LICENSE_FILE_NAME license.txt ../../)
	if(LICENSE_FILE_NAME)
		set(CPACK_RESOURCE_FILE_LICENSE "${LICENSE_FILE_NAME}")
		set(CPACK_COMPONENT_SDK_REQUIRED "SDK")
	endif(LICENSE_FILE_NAME)
	# Set what defined components CPack should use.
	set(CPACK_COMPONENTS_ALL SDK SourceCode)
	 
	# This must always be the last part of the CPack setup.
	include(CPack)
 
endif(WIN32)
