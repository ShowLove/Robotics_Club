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
project (auvc)

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
ZEBULON_ADD_SUBDIRECTORY(keller ./sensors/keller)
ZEBULON_ADD_SUBDIRECTORY(mcu ./mcu/microcontroller)
ZEBULON_ADD_SUBDIRECTORY(zmcu ./zmcu/microcontroller)
ZEBULON_ADD_SUBDIRECTORY(gx3 ./sensors/gx3)

# Laser Sensors
ZEBULON_ADD_SUBDIRECTORY(laserfactory ./sensors/laser)
# Math Utilities
ZEBULON_ADD_SUBDIRECTORY(mixer ./math/libmixer)
ZEBULON_ADD_SUBDIRECTORY(filter ./math/libfilters)
# Configuration
ZEBULON_ADD_SUBDIRECTORY(baseline ./configuration/libbaseline)
# Visualization
ZEBULON_ADD_SUBDIRECTORY(display ./visualizations/libdisplay)
ZEBULON_ADD_SUBDIRECTORY(grapher ./visualizations/libgrapher)
# Configuration
ZEBULON_ADD_SUBDIRECTORY(xmlconfig ./configuration/libxmlconfig)
ZEBULON_ADD_SUBDIRECTORY(ransac ./math/libransac)
# AI Utilities
ZEBULON_ADD_SUBDIRECTORY(statemachine ./ai/libstatemachine)


# Video Sensors
if(WIN32)
add_definitions(-DCV_USE_OPEN_CV)
endif(WIN32)
ZEBULON_ADD_SUBDIRECTORY(cvcapture ./sensors/video)


# Machin Vision Utilities
ZEBULON_ADD_SUBDIRECTORY(gvision ./vision/gvision)
ZEBULON_ADD_SUBDIRECTORY(dvision ./vision/dvision)
ZEBULON_ADD_SUBDIRECTORY(cvision ./vision/cvision)
ZEBULON_ADD_SUBDIRECTORY(homography ./vision/homography)

#add_subdirectory(../../ext/ransac ./math/ransac)
add_subdirectory(baseline_program ./auvc/baseline_program)
add_subdirectory(underwatervision ./auvc/underwatervision)
add_subdirectory(underwater_program ./auvc/underwater_program)
add_subdirectory(example_opencv ./auvc/example_opencv)
#add_subdirectory(calibration_program ./auvc/calibration_program)
add_subdirectory(joystick_program ./auvc/joystick_program)

