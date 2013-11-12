# Install script for directory: /home/showlove/software/zebulon/projects/asvc/2012/build/cmake

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/libcxutils/CxUtils/libcxutils/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/libtinyxml/TinyXML/libtinyxml/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/libjaus/JAUS/libjauscore/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/libjaus/JAUS/libjausmobility/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/libjaus/JAUS/libjausextras/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/libcartographer/Cartographer/libcartographer/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/compass/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/gps/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/laser/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/raytek/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/gx3/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/mcu/microcontroller/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/motor/roboteq/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/motor/dynamixel/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/math/libmixer/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/math/libfilters/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/visualizations/libdisplay/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/visualizations/libgrapher/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/configuration/libxmlconfig/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/configuration/libbaseline/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/video/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/asvc/baseline_program/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/ai/libstatemachine/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/vision/gvision/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/vision/homography/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/vision/dvision/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/vision/cvision/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/asvc/hotsuite/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/asvc/examples/cmake_install.cmake")
  INCLUDE("/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/asvc/surface_program/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
