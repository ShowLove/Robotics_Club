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


# Create a solution/workspace name and a specific library/binary name.
set(PROJECT_NAME JAUS++)

set(PROJECT_VERSION 3)

# Name the project (for dependancy)
project (${PROJECT_NAME})

# Script to setup output directories.
FIND_PATH(ACTIVE_MACROS_ROOT_DIR SetOutputDirectories.cmake
			PATHS
				../../../../
				../../../../../
				../../../../../../)
include(${ACTIVE_MACROS_ROOT_DIR}/SetOutputDirectories.cmake)
include(${ACTIVE_MACROS_ROOT_DIR}/FindACTIVE.cmake)
include(${ACTIVE_MACROS_ROOT_DIR}/FindVisualLeakDetector.cmake)

# Add external dependecies we have projects for.
set(PACKAGES_TO_LOAD CxUtils TinyXML)

# Add internal library dependencies using their CMake Files.
foreach(PKG ${PACKAGES_TO_LOAD})
	set(${PKG}_VERSION_TO_USE "trunk" CACHE STRING "${PKG} version selected at CMake configure time")
	set_property(CACHE ${PKG}_VERSION_TO_USE PROPERTY STRINGS "${ACTIVE_${PKG}_AVAILABLE_VERSIONS}")
	ACTIVE_ADD_SUBDIRECTORY_VER(${PKG} ./lib${PKG} ${${PKG}_VERSION_TO_USE})	
endforeach()

# Get Boost!
#set(Boost_USE_STATIC_LIBS   ON)
#set(Boost_USE_MULTITHREADED      ON)
#set(Boost_DEBUG TRUE)
set(Boost_ADDITIONAL_VERSIONS 
			"1.47" 
			"1.47.0"
			"1.48"
			"1.48.0")
find_package(Boost COMPONENTS thread date_time REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Lookup JAUS Version for this directory.
# Use the current path, but only the last folder name.
string(REPLACE "/" ";" PATH_TOKENS ${OUTPUT_DIRECTORY_ROOT})
list(LENGTH PATH_TOKENS PATH_TOKENS_LENGTH)
math(EXPR TOKEN_INDEX "${PATH_TOKENS_LENGTH} - 1")
list(GET PATH_TOKENS ${TOKEN_INDEX} JAUS_VER_TOKEN)

# Append the branches folder if not trunk
if(NOT ${JAUS_VER_TOKEN} MATCHES "trunk")
	set(JAUS_VER_TOKEN "branches/${JAUS_VER_TOKEN}")
endif()

set(JAUS_VERSION_TO_USE ${JAUS_VER_TOKEN} CACHE STRING "${PKG} version selected at CMake configure time")
set_property(CACHE JAUS_VERSION_TO_USE PROPERTY STRINGS "${ACTIVE_JAUS_AVAILABLE_VERSIONS}")
ACTIVE_ADD_SUBDIRECTORY_VER(JAUS ./libJAUS ${JAUS_VERSION_TO_USE})
	
add_subdirectory(programs ./cmake.jaus.programs)

OPTION(BUILD_TUTORIALS "Build Tutorial" FALSE)

if(BUILD_TUTORIALS)
	add_subdirectory(tutorial ./cmake.jaus.tutorial)
endif()

OPTION(BUILD_JAUS_EXAMPLES "Build Additional Examples" FALSE)

if(BUILD_JAUS_EXAMPLES)
	add_subdirectory(examples ./cmake.jaus.examples)
endif()

OPTION(BUILD_JAUS_UNIT_TESTS "Build Unit Tests" FALSE)

if(BUILD_JAUS_UNIT_TESTS)
	add_subdirectory(unit_tests ./cmake.jaus.unit_tests)
endif()

install(DIRECTORY 
		../../build/cmake/examples
		../../build/cmake/programs
		../../build/cmake/tutorial
		DESTINATION src/active-ist/libraries/jaus++/trunk/build/cmake
		COMPONENT SourceCode
		PATTERN ".svn" EXCLUDE)
install(FILES 
		../../build/cmake/CMakeLists.txt
		DESTINATION src/active-ist/libraries/jaus++/trunk/build/cmake
		COMPONENT SourceCode)
		
set(CPACK_PACKAGE_NAME "JAUS++")
IF(MINGW)
	set(CPACK_PACKAGE_NAME "JAUS++.MinGW")
ENDIF()
set(CPACK_PACKAGE_VENDOR "Univeristy of Central Florida - IST - ACTIVE Lab")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Joint Architecture for Unmanned Systems (JAUS) C++ SDK")
set(CPACK_PACKAGE_CONTACT "Daniel Barber - dbarber@ist.ucf.edu")

TODAY(BUILD_DATE YEAR MONTH DAY)


set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}.${YEAR}${MONTH}${DAY}")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "JAUS++ ${PROJECT_VERSION}.${YEAR}")

find_file(LICENSE_FILE_NAME license.txt ../../)
if(LICENSE_FILE_NAME)
    set(CPACK_RESOURCE_FILE_LICENSE "${LICENSE_FILE_NAME}")
endif(LICENSE_FILE_NAME)

set(CPACK_COMPONENT_SDK_REQUIRED "SDK")
set(CPACK_COMPONENTS_ALL SDK Programs SourceCode)

if(WIN32)
    # NSIS Installer
	set(CPACK_NSIS_MENU_LINKS "bin" "Programs")
else(UNIX)
    
    # DEB Package
    # DEBIAN/control
    # debian policy enforce lower case for package name
    # Package: (mandatory)
    IF(NOT CPACK_DEBIAN_PACKAGE_NAME)
        STRING(TOLOWER "${CPACK_PACKAGE_NAME}" CPACK_DEBIAN_PACKAGE_NAME)
    ENDIF(NOT CPACK_DEBIAN_PACKAGE_NAME)

    # Version: (mandatory)
    IF(NOT CPACK_DEBIAN_PACKAGE_VERSION)
        IF(NOT CPACK_PACKAGE_VERSION)
            MESSAGE(FATAL_ERROR "Debian package requires a package version")
        ENDIF(NOT CPACK_PACKAGE_VERSION)
        SET(CPACK_DEBIAN_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})
    ENDIF(NOT CPACK_DEBIAN_PACKAGE_VERSION)

    # Architecture: (mandatory)
    IF(NOT CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
        # There is no such thing as i686 architecture on debian, you should use i386 instead
        # $ dpkg --print-architecture
        FIND_PROGRAM(DPKG_CMD dpkg)
        IF(NOT DPKG_CMD)
            MESSAGE(STATUS "Can not find dpkg in your path, default to i386.")
            SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
        ENDIF(NOT DPKG_CMD)
        EXECUTE_PROCESS(COMMAND "${DPKG_CMD}" --print-architecture
                        OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        )
    ENDIF(NOT CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
 

    # Maintainer: (mandatory)
    IF(NOT CPACK_DEBIAN_PACKAGE_MAINTAINER)
        IF(NOT CPACK_PACKAGE_CONTACT)
            MESSAGE(FATAL_ERROR "Debian package requires a maintainer for a package, set CPACK_PACKAGE_CONTACT or CPACK_DEBIAN_PACKAGE_MAINTAINER")
        ENDIF(NOT CPACK_PACKAGE_CONTACT)
        SET(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CPACK_PACKAGE_CONTACT})
    ENDIF(NOT CPACK_DEBIAN_PACKAGE_MAINTAINER)

    # Description: (mandatory)
    IF(NOT CPACK_DEBIAN_PACKAGE_DESCRIPTION)
        IF(NOT CPACK_PACKAGE_DESCRIPTION_SUMMARY)
            MESSAGE(FATAL_ERROR "Debian package requires a summary for a package, set CPACK_PACKAGE_DESCRIPTION_SUMMARY or CPACK_DEBIAN_PACKAGE_DESCRIPTION")
        ENDIF(NOT CPACK_PACKAGE_DESCRIPTION_SUMMARY)
        SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION_SUMMARY})
    ENDIF(NOT CPACK_DEBIAN_PACKAGE_DESCRIPTION)

    # Section: (recommended)
    IF(NOT CPACK_DEBIAN_PACKAGE_SECTION)
        SET(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    ENDIF(NOT CPACK_DEBIAN_PACKAGE_SECTION)
    
    # Priority: (recommended)
    IF(NOT CPACK_DEBIAN_PACKAGE_PRIORITY)
        SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    ENDIF(NOT CPACK_DEBIAN_PACKAGE_PRIORITY )

    # Recommends:
    # You should set: CPACK_DEBIAN_PACKAGE_RECOMMENDS

    # Suggests:
    # You should set: CPACK_DEBIAN_PACKAGE_SUGGESTS

    # CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    # This variable allow advanced user to add custom script to the control.tar.gz (inside the .deb archive)
    # Typical examples are: 
    # - conffiles
    # - postinst
    # - postrm
    # - prerm"
    # Usage:
    # SET(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA 
    #    "${CMAKE_CURRENT_SOURCE_DIR/prerm;${CMAKE_CURRENT_SOURCE_DIR}/postrm")


    # For debian source packages:
    # debian/control
    # http://www.debian.org/doc/debian-policy/ch-controlfields.html#s-sourcecontrolfiles

    # .dsc
    # http://www.debian.org/doc/debian-policy/ch-controlfields.html#s-debiansourcecontrolfiles

    # Builds-Depends:
    #IF(NOT CPACK_DEBIAN_PACKAGE_BUILDS_DEPENDS)
    #  SET(CPACK_DEBIAN_PACKAGE_BUILDS_DEPENDS
    #    "debhelper (>> 5.0.0), libncurses5-dev, tcl8.4"
    #  )
    #ENDIF(NOT CPACK_DEBIAN_PACKAGE_BUILDS_DEPENDS)

endif(WIN32)

# This must always be last!
include(CPack)


