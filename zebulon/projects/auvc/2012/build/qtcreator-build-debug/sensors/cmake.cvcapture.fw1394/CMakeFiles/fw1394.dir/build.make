# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/cmake-gui

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/showlove/software/zebulon/projects/auvc/2012/build/cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug

# Include any dependencies generated for this target.
include sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/depend.make

# Include the progress variables for this target.
include sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/progress.make

# Include the compile flags for this target's objects.
include sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/flags.make

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/flags.make
sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o: /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/exceptions/fwException.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o -c /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/exceptions/fwException.cpp

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.i"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/exceptions/fwException.cpp > CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.i

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.s"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/exceptions/fwException.cpp -o CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.s

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o.requires:
.PHONY : sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o.requires

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o.provides: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o.requires
	$(MAKE) -f sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/build.make sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o.provides.build
.PHONY : sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o.provides

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o.provides.build: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/flags.make
sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o: /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/tools/time/fwTime.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o -c /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/tools/time/fwTime.cpp

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.i"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/tools/time/fwTime.cpp > CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.i

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.s"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/tools/time/fwTime.cpp -o CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.s

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o.requires:
.PHONY : sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o.requires

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o.provides: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o.requires
	$(MAKE) -f sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/build.make sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o.provides.build
.PHONY : sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o.provides

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o.provides.build: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/flags.make
sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o: /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/camera/generic_camera/fwCameraException.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o -c /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/camera/generic_camera/fwCameraException.cpp

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.i"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/camera/generic_camera/fwCameraException.cpp > CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.i

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.s"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/camera/generic_camera/fwCameraException.cpp -o CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.s

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o.requires:
.PHONY : sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o.requires

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o.provides: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o.requires
	$(MAKE) -f sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/build.make sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o.provides.build
.PHONY : sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o.provides

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o.provides.build: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/flags.make
sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o: /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/camera/1394/fw1394Camera.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o -c /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/camera/1394/fw1394Camera.cpp

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.i"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/camera/1394/fw1394Camera.cpp > CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.i

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.s"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394/src/camera/1394/fw1394Camera.cpp -o CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.s

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o.requires:
.PHONY : sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o.requires

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o.provides: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o.requires
	$(MAKE) -f sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/build.make sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o.provides.build
.PHONY : sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o.provides

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o.provides.build: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o

# Object files for target fw1394
fw1394_OBJECTS = \
"CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o" \
"CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o" \
"CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o" \
"CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o"

# External object files for target fw1394
fw1394_EXTERNAL_OBJECTS =

/home/showlove/software/zebulon/projects/auvc/2012/lib/libfw1394_d.so: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o
/home/showlove/software/zebulon/projects/auvc/2012/lib/libfw1394_d.so: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o
/home/showlove/software/zebulon/projects/auvc/2012/lib/libfw1394_d.so: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o
/home/showlove/software/zebulon/projects/auvc/2012/lib/libfw1394_d.so: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o
/home/showlove/software/zebulon/projects/auvc/2012/lib/libfw1394_d.so: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/build.make
/home/showlove/software/zebulon/projects/auvc/2012/lib/libfw1394_d.so: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library /home/showlove/software/zebulon/projects/auvc/2012/lib/libfw1394_d.so"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fw1394.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/build: /home/showlove/software/zebulon/projects/auvc/2012/lib/libfw1394_d.so
.PHONY : sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/build

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/requires: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/exceptions/fwException.cpp.o.requires
sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/requires: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/tools/time/fwTime.cpp.o.requires
sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/requires: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/generic_camera/fwCameraException.cpp.o.requires
sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/requires: sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/src/camera/1394/fw1394Camera.cpp.o.requires
.PHONY : sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/requires

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/clean:
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 && $(CMAKE_COMMAND) -P CMakeFiles/fw1394.dir/cmake_clean.cmake
.PHONY : sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/clean

sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/depend:
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/showlove/software/zebulon/projects/auvc/2012/build/cmake /home/showlove/software/zebulon/sensors/video/cvcapture/ext/fw1394 /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394 /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sensors/cmake.cvcapture.fw1394/CMakeFiles/fw1394.dir/depend

