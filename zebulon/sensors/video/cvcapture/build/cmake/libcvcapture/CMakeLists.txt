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

# Script to setup output directories.
FIND_PATH(ZEBULON_MACROS_ROOT_DIR 
			SetOutputDirectories.cmake
			PATHS
				../../../../../../
				../../../../../../../)
include(${ZEBULON_MACROS_ROOT_DIR}/SetOutputDirectories.cmake)

# Set the name to use for the DLL Export Preprocessor
# definition for WIN32 MSVC
set(DLL_EXPORT_NAME CV_IMAGE_CAPTURE_EXPORT)
# Set name of this project, and dependencies needed.
# Names are different between MSVC and other build systems, so
# in MSVC add prefix to name(s) as needed.
set(LIB_PROJECT_NAME cvcapture)

# Source/Directories to sort, for virtual folders in MSVC
set(SOURCE_GROUPS "")
# In other systems besides MSVC, a lib prefix is
# added to all libraries.  The LIB_PREFIX value is
# defined and set in the SetOutputDirectories.cmake file to
# make things consistent between environments.
set(LIB_NAME ${LIB_PREFIX}${LIB_PROJECT_NAME})

# For UNIX (Ubuntu) add other external dependencies
# to add support for different capture systems
if(UNIX)
	add_subdirectory(../../../ext/dvgrab/ ../cmake.${LIB_PROJECT_NAME}.dvgrab)
	add_subdirectory(../../../ext/fw1394/ ../cmake.${LIB_PROJECT_NAME}.fw1394)
	add_subdirectory(../../../ext/libv4l/ ../cmake.${LIB_PROJECT_NAME}.libv4l)
endif(UNIX)

# Try import support for Basler Pylon Camera
# driver (GIGE based Camera)
include(FindPylon.cmake)

# Set variable name of input directories for code.
set(LIB_INCLUDE_DIR ../../../include/${LIB_PROJECT_NAME})
set(LIB_SRC_DIR ../../../src/${LIB_PROJECT_NAME})

# Get header files.
#file(GLOB_RECURSE LIB_HEADERS ${LIB_INCLUDE_DIR}/*.h)
if(MSVC)
	include_directories(../../../include/)
	set(LIB_HEADERS 
			${LIB_INCLUDE_DIR}/cvimagecapture.h)
	# Get source files.
	set(LIB_SOURCES 
			${LIB_SRC_DIR}/cvimagecaptureimage.cpp
			${LIB_SRC_DIR}/cvimagecapturewindows.cpp)
else(UNIX)
	include_directories(../../../include/
                            ../../../ext/dvgrab
                            ../../../ext/fw1394/include
                            ../../../ext/libv4l)
	set(LIB_HEADERS 
                        ${LIB_INCLUDE_DIR}/imagecaptureutils.h
			${LIB_INCLUDE_DIR}/cvimagecapture.h
			${LIB_INCLUDE_DIR}/imageloader.h
			${LIB_INCLUDE_DIR}/imagecapturedc1394.h
			${LIB_INCLUDE_DIR}/imagecapturedv1394.h
			${LIB_INCLUDE_DIR}/imagecapturev4l.h
			${LIB_INCLUDE_DIR}/imagecapturev4l2.h)
	# Get source files.
	set(LIB_SOURCES 
			${LIB_SRC_DIR}/cvimagecaptureimage.cpp
			${LIB_SRC_DIR}/imageloader.cpp
			${LIB_SRC_DIR}/cvimagecapturelinux.cpp
			${LIB_SRC_DIR}/imagecapturedc1394.cpp
			${LIB_SRC_DIR}/imagecapturedv1394.cpp
			${LIB_SRC_DIR}/imagecapturev4l.cpp
			${LIB_SRC_DIR}/imagecapturev4l2.cpp)
endif(MSVC)

# If Basler Pylon library found
# include support for it
if(Pylon_FOUND)
	#set(ENV{PYLON_ROOT} /opt/pylon)
	#set(ENV{GENICAM_ROOT_V2_1} /opt/pylon/genicam)
	include_directories(${Pylon_INCLUDE_DIRS})
        link_directories(${Pylon_LIBRARY_DIRS})
        #message(STATUS ${Pylon_LIBRARY_DIRS})
	set(LIB_HEADERS ${LIB_HEADERS} ${LIB_INCLUDE_DIR}/imagecapturepylon.h)
	set(LIB_SOURCES ${LIB_SOURCES} ${LIB_SRC_DIR}/imagecapturepylon.cpp)
	set(EXT_LIBS ${EXT_LIBS} ${Pylon_LIBRARIES})
        message(STATUS "Pylon Dirs: ${Pylon_LIBRARY_DIRS}")
        message(STATUS "Pylon Libs: ${Pylon_LIBRARIES}")
        add_definitions(${Pylon_CXX_FLAGS})
	#message(STATUS ${Pylon_CXX_FLAGS})
        message(STATUS "CvImageCapture - Basler Pylon Library Found")
else(Pylon_FOUND)
	message(STATUS "CvImageCapture - Basler Pylon Library NOT FOUND, skipping support")
endif(Pylon_FOUND)

# Add support for image loader interface, but
# only if CxUtils is found because it is required
if(CxUtils_FOUND)
	set(LIB_HEADERS ${LIB_HEADERS} ${LIB_INCLUDE_DIR}/imageloader.h)
	set(LIB_SOURCES ${LIB_SOURCES} ${LIB_SRC_DIR}/imageloader.cpp)
endif(CxUtils_FOUND)

# Setup any additional compiler flags.
if(MSVC)
	# Setup to use UNICODE for MSVC, and disable warnings for
	# use of s_* standard C functions.
	add_definitions(-D"WINVER 0x0500" -D"_WIN32_WINNT 0x0500")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE)
	# Add support for parallel compilation
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /wd4995")
ENDIF()

if(UNIX AND NOT MINGW)
	# Ensure using gcc for our code
	set(CMAKE_C_COMPILER gcc)
	# Add fPIC to support 64 bit building issues
	add_definitions(-fPIC -DHAVE_LIBDV)
endif()

if(MSVC)
	# Find DirectoShow and DirectX files.
	find_path(DIRECT_SHOW_BASE_CLASSES_INCLUDE_DIR  videoctl.h "C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Samples\\multimedia\\directshow\\baseclasses")
	find_path(DIRECT_SHOW_BASE_CLASSES_LIB_DEBUG  strmbasd.lib "${DIRECT_SHOW_BASE_CLASSES_INCLUDE_DIR}\\Debug")
	find_path(DIRECT_SHOW_BASE_CLASSES_LIB_RELEASE  strmbase.lib "${DIRECT_SHOW_BASE_CLASSES_INCLUDE_DIR}\\Release")
	find_path(DIRECTX_9_SDK_FEBRUARY_2007_INCLUDE_DIR  dxtrans.h "C:\\Program Files (x86)\\Microsoft DirectX SDK (February 2007)\\Include")
	find_path(DIRECTX_9_SDK_INCLUDE_DIR  d3d9.h "C:\\Program Files (x86)\\Microsoft DirectX SDK (March 2009)\\Include")
	find_path(WINDOWS_6_A_SDK_INCLUDE_DIR  AccCtrl.h "C:\\Program Files\\Microsoft SDKs\\Windows\\v6.0A\\Include")
	find_path(WINDOWS_7_SDK_INCLUDE_DIR  AccCtrl.h "C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Include")

	# Order matters here.
	include_directories(${DIRECT_SHOW_BASE_CLASSES_INCLUDE_DIR}
						${DIRECTX_9_SDK_INCLUDE_DIR}
						${DIRECTX_9_SDK_FEBRUARY_2007_INCLUDE_DIR}
						${WINDOWS_6_A_SDK_INCLUDE_DIR}
						${WINDOWS_7_SDK_INCLUDE_DIR})
	set(EXT_LIBS ${EXT_LIBS}
					wmvcore.lib winmm.lib
					debug ${DIRECT_SHOW_BASE_CLASSES_LIB_DEBUG}/strmbasd.lib
					optimized ${DIRECT_SHOW_BASE_CLASSES_LIB_RELEASE}/strmbase.lib)
	if(OpenCV_FOUND)
		set(EXT_LIBS ${EXT_LIBS}
					${OpenCV_LIBRARIES})
		link_directories(${OpenCV_LIBRARY_DIRS})
	endif(OpenCV_FOUND)
endif(MSVC)

# Build a library
add_library(${LIB_NAME} SHARED ${LIB_SOURCES} ${LIB_HEADERS})

# Solution folders.
if(USE_SOLUTION_FOLDERS)
	SET_PROPERTY(TARGET ${LIB_NAME}  PROPERTY FOLDER "Zebulon")
endif()
		

if(MSVC)
	# Add a define for building a DLL in windows.
	set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "-D_USRDLL -D${DLL_EXPORT_NAME}=__declspec(dllexport)")
else()
	# Link symbols
	set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS -rdynamic)
endif(MSVC)

if(Pylon_FOUND)
	set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "-DUSE_PYLON")
endif()

# Add an _d when building debug version
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")

# Set build dependencies.  These projects will get built
# before this file.
if(CxUtils_FOUND)
	include_directories(${CxUtils_INCLUDE_DIRS})
	link_directories(${CxUtils_LIBRARY_DIRS})
	# If the option exists to build CxUtils from CMake
	# then add as a dependency so it builds first
	if(CxUtils_DEPENDENCY)
		add_dependencies(${LIB_NAME} ${CxUtils_DEPENDENCY})
	endif(CxUtils_DEPENDENCY)
	set(EXT_LIBS ${EXT_LIBS} ${CxUtils_LIBRARIES})
endif(CxUtils_FOUND)

# Ubuntu
if(UNIX) 
	# Add Dependencies that must compile first
	add_dependencies(${LIB_NAME} ${LIB_PREFIX}dvgrab)
        add_dependencies(${LIB_NAME} ${LIB_PREFIX}fw1394)
        add_dependencies(${LIB_NAME} ${LIB_PREFIX}v4l)
	# Add libraries to link against
        set(EXT_LIBS ${EXT_LIBS}
			dv
			avc1394
			iec61883
			rom1394
                        dc1394
			${OpenCV_LIBRARIES}
                        v4l1
			optimized ${LIB_PREFIX}dvgrab.so
			debug ${LIB_PREFIX}dvgrab_d.so
			optimized ${LIB_PREFIX}fw1394.so
			debug ${LIB_PREFIX}fw1394_d.so
			optimized ${LIB_PREFIX}v4l.so
                        debug ${LIB_PREFIX}v4l_d.so
                        )
endif(UNIX)


# Link against libraries (FINAL)
target_link_libraries(${LIB_NAME} ${CMAKE_REQUIRED_LIBRARIES} ${EXT_LIBS})

if(Pylon_FOUND)
	set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS ${Pylon_LINKER_FLAGS})
endif(Pylon_FOUND)

# CPack and other Installation Settings (i.e. make install)
if(UNIX)
	# Where to install headers
	install(DIRECTORY 
		${LIB_INCLUDE_DIR}
		DESTINATION include/zebulon/
		FILES_MATCHING PATTERN "*.h" 
		PATTERN ".svn" EXCLUDE)
	# Where to install targets
	install(TARGETS ${LIB_NAME} LIBRARY DESTINATION lib/zebulon)
endif(UNIX)

# CPack Installer Generation
if(WIN32)

	install(TARGETS ${LIB_NAME}
			DESTINATION bin
			ARCHIVE DESTINATION lib
			COMPONENT SDK)
	install(TARGETS ${LIB_NAME}
			DESTINATION bin
			RUNTIME DESTINATION bin
			COMPONENT SDK)
	install(TARGETS ${LIB_NAME}
			DESTINATION bin
			LIBRARY DESTINATION bin
			COMPONENT SDK)
	if(CV_IMAGE_LOADER_SUPPORT)
		install(FILES 
				${LIB_INCLUDE_DIR}/cvimagecapture.h
				${LIB_INCLUDE_DIR}/imageloader.h
				DESTINATION include/cvcapture
				COMPONENT SDK
				)
	else()
		install(FILES 
				${LIB_INCLUDE_DIR}/cvimagecapture.h
				DESTINATION include/cvcapture
				COMPONENT SDK
				)
	endif()

	install(FILES 
			../../../bin/${LIB_NAME}_d.dll
			DESTINATION bin
			COMPONENT SDK
			)
	install(FILES 
			../../../lib/${LIB_NAME}_d.lib
			DESTINATION lib
			COMPONENT SDK
			)
endif(WIN32)
