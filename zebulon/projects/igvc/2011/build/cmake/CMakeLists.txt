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

# Name the project
project (igvc)

# Set output folders for binaries and libraries.
include(../../../../../SetOutputDirectories.cmake)
include(../../../../../FindACTIVE.cmake)
include(../../../../../FindZebulon.cmake)
include(../../../../../FindOpenCV.cmake)

# Adds the CxUtils CMakeLists.txt file if available.
ACTIVE_ADD_SUBDIRECTORY(CxUtils ./libcxutils)
ACTIVE_ADD_SUBDIRECTORY(TinyXML ./libtinyxml)
ACTIVE_ADD_SUBDIRECTORY(JAUS ./libjaus)
ACTIVE_ADD_SUBDIRECTORY(Cartographer ./libcartographer)

if(OpenCV_FOUND)
    if(CARTOGRAPHER_USE_OPENCV)
        add_definitions(-DUSE_OPENCV)
    endif(CARTOGRAPHER_USE_OPENCV)
endif(OpenCV_FOUND)

# Compass Sensors
ZEBULON_ADD_SUBDIRECTORY(compassfactory ./sensors/compass)
# GPS Sensors
ZEBULON_ADD_SUBDIRECTORY(gpsfactory ./sensors/gps)
# Laser Sensors
ZEBULON_ADD_SUBDIRECTORY(laserfactory ./sensors/laser)
# Microcontroller
ZEBULON_ADD_SUBDIRECTORY(mcu ./microcontroller/libmcu)
# Motor Controller
ZEBULON_ADD_SUBDIRECTORY(roboteq ./motors/libroboteq)
# Math Utilities
ZEBULON_ADD_SUBDIRECTORY(mixer ./math/libmixer)
ZEBULON_ADD_SUBDIRECTORY(encoders ./math/libencoders)
ZEBULON_ADD_SUBDIRECTORY(filter ./math/libfilters)
# Visualization
ZEBULON_ADD_SUBDIRECTORY(display ./visualizations/libdisplay)
ZEBULON_ADD_SUBDIRECTORY(grapher ./visualizations/libgrapher)
# Configuration
ZEBULON_ADD_SUBDIRECTORY(xmlconfig ./configuration/libxmlconfig)
ZEBULON_ADD_SUBDIRECTORY(baseline ./configuration/libbaseline)
ZEBULON_ADD_SUBDIRECTORY(ransac ./math/libransac)

# Video Sensors
if(WIN32)
add_definitions(-DCV_USE_OPEN_CV)
endif(WIN32)
ZEBULON_ADD_SUBDIRECTORY(cvcapture ./sensors/video)

# Programs
add_subdirectory(baseline_program ./igvc/baseline_program)

# AI Utilities
ZEBULON_ADD_SUBDIRECTORY(statemachine ./ai/libstatemachine)

# Machin Vision Utilities
ZEBULON_ADD_SUBDIRECTORY(gvision ./vision/gvision)
ZEBULON_ADD_SUBDIRECTORY(dvision ./vision/dvision)

add_subdirectory(libground ./igvc/libground)
add_subdirectory(ground_program ./igvc/ground_program)
add_subdirectory(calibration_program ./igvc/calibration_program)

#add_subdirectory(../../ext/ransac ./math/ransac)
add_subdirectory(libhomography ./igvc/libhomography)
#add_subdirectory(ground ./igvc/ground)

option(BUILD_IGVC_TESTS "Build IGVC Test Programs" ${BUILD_IGVC_TESTS}) 
set(BUILD_IGVC_TESTS true)
if(BUILD_IGVC_TESTS)
        add_subdirectory(example_opencv ./igvc/example_opencv)
        add_subdirectory(example_find_groundplane ./igvc/example_find_groundplane)
        add_subdirectory(example_laneline_mission ./igvc/example_laneline_mission)
        add_subdirectory(example_test_path ./igvc/example_test_path)
	add_subdirectory(example_test_path_astar ./igvc/example_test_path_astar)
        add_subdirectory(example_homography ./igvc/example_homography)
endif()
