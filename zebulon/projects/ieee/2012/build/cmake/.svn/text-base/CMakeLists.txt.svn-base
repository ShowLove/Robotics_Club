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
project (ieee)

find_package(Qt4 REQUIRED)

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
    if(CARTOGRAPHER_USE_OPENCV)
        add_definitions(-DUSE_OPENCV)
    endif(CARTOGRAPHER_USE_OPENCV)
endif(OpenCV_FOUND)

# Laser Sensors
ZEBULON_ADD_SUBDIRECTORY(laserfactory ./sensors/laser)
# Microcontroller
ZEBULON_ADD_SUBDIRECTORY(mcu ./microcontroller/libmcu)
# Math Utilities
ZEBULON_ADD_SUBDIRECTORY(filter ./math/libfilters)
# Visualization
ZEBULON_ADD_SUBDIRECTORY(display ./visualizations/libdisplay)
ZEBULON_ADD_SUBDIRECTORY(grapher ./visualizations/libgrapher)
# Configuration
ZEBULON_ADD_SUBDIRECTORY(xmlconfig ./configuration/libxmlconfig)
ZEBULON_ADD_SUBDIRECTORY(ransac ./math/libransac)
#MCU Communication
ZEBULON_ADD_SUBDIRECTORY(mcu ./hardware/mcu)

# Video Sensors
if(WIN32)
add_definitions(-DCV_USE_OPEN_CV)
endif(WIN32)
#ZEBULON_ADD_SUBDIRECTORY(cvcapture ./sensors/video)

# Machin Vision Utilities
ZEBULON_ADD_SUBDIRECTORY(gvision ./vision/gvision)
ZEBULON_ADD_SUBDIRECTORY(dvision ./vision/dvision)

# State Machine
ZEBULON_ADD_SUBDIRECTORY(statemachine ./ai/statemachine)

# Programs
add_subdirectory(ieee_mike ./ieee/ieee_mike)
add_subdirectory(calibrate ./ieee/calibrate)
add_subdirectory(ieee_cassy ./ieee/ieee_cassy)
add_subdirectory(communication ./ieee/communication)
add_subdirectory(course ./ieee/course)
add_subdirectory(projector ./ieee/projector)


