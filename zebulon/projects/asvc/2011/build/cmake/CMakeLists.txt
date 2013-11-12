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
project (asvc)

# Set output folders for binaries and libraries.
include(../../../../../SetOutputDirectories.cmake)
include(../../../../../FindACTIVE.cmake)
include(../../../../../FindZebulon.cmake)
include(../../../../../FindOpenCV.cmake)

# Adds the CxUtils CMakeLists.txt file if available.
ACTIVE_ADD_SUBDIRECTORY(CxUtils ./libcxutils)
ACTIVE_ADD_SUBDIRECTORY(TinyXML ./libtinyxml)
ACTIVE_ADD_SUBDIRECTORY(JAUS ./libjaus)

if(OpenCV_FOUND)
    option(CARTOGRAPHER_USE_OPENCV "Use OpenCV Visualizations for Cartographer" TRUE)
    set(CARTOGRAPHER_USE_OPENCV TRUE)
    add_definitions(-DUSE_OPENCV)
endif(OpenCV_FOUND)

ACTIVE_ADD_SUBDIRECTORY(Cartographer ./libcartographer)

if(OpenCV_FOUND)
    option(CARTOGRAPHER_USE_OPENCV "Use OpenCV Visualizations for Cartographer" TRUE)
    set(CARTOGRAPHER_USE_OPENCV TRUE)
    add_definitions(-DUSE_OPENCV)
endif(OpenCV_FOUND)

# Compass Sensors
ZEBULON_ADD_SUBDIRECTORY(compassfactory ./sensors/compass)
# GPS Sensors
ZEBULON_ADD_SUBDIRECTORY(gpsfactory ./sensors/gps)
# Laser Sensors
ZEBULON_ADD_SUBDIRECTORY(laserfactory ./sensors/laser)
# Motor Controller
ZEBULON_ADD_SUBDIRECTORY(mcu ./mcu/microcontroller)
ZEBULON_ADD_SUBDIRECTORY(dynamixel ./motor/libdynamixel)
# Math Utilities
ZEBULON_ADD_SUBDIRECTORY(mixer ./math/libmixer)
ZEBULON_ADD_SUBDIRECTORY(filter ./math/libfilters)
# Visualization
ZEBULON_ADD_SUBDIRECTORY(display ./visualizations/libdisplay)
ZEBULON_ADD_SUBDIRECTORY(grapher ./visualizations/libgrapher)
# Configuration
ZEBULON_ADD_SUBDIRECTORY(xmlconfig ./configuration/libxmlconfig)
ZEBULON_ADD_SUBDIRECTORY(baseline ./configuration/libbaseline)

# Video Sensors
if(WIN32)
add_definitions(-DCV_USE_OPEN_CV)
endif(WIN32)
ZEBULON_ADD_SUBDIRECTORY(cvcapture ./sensors/video)

# Programs
add_subdirectory(baseline_program ./asvc/baseline_program)

# AI Utilities
ZEBULON_ADD_SUBDIRECTORY(statemachine ./ai/libstatemachine)

# Machin Vision Utilities
ZEBULON_ADD_SUBDIRECTORY(gvision ./vision/gvision)
ZEBULON_ADD_SUBDIRECTORY(homography ./vision/homography)
#ZEBULON_ADD_SUBDIRECTORY(dvision ./vision/dvision)
#ZEBULON_ADD_SUBDIRECTORY(cvision ./vision/cvision)

add_subdirectory(surface_program ./asvc/surface_program)
#add_subdirectory(calibration_program ./asvc/calibration_program)

#add_subdirectory(../../ext/ransac ./math/ransac)
#add_subdirectory(surface ./asvc/surface)

