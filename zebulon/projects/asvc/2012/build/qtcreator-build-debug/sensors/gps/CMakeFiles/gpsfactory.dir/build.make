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
CMAKE_SOURCE_DIR = /home/showlove/software/zebulon/projects/asvc/2012/build/cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug

# Include any dependencies generated for this target.
include sensors/gps/CMakeFiles/gpsfactory.dir/depend.make

# Include the progress variables for this target.
include sensors/gps/CMakeFiles/gpsfactory.dir/progress.make

# Include the compile flags for this target's objects.
include sensors/gps/CMakeFiles/gpsfactory.dir/flags.make

sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o: sensors/gps/CMakeFiles/gpsfactory.dir/flags.make
sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o: /home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o"
	cd /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/gps && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o -c /home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp

sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.i"
	cd /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/gps && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp > CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.i

sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.s"
	cd /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/gps && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp -o CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.s

sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o.requires:
.PHONY : sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o.requires

sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o.provides: sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o.requires
	$(MAKE) -f sensors/gps/CMakeFiles/gpsfactory.dir/build.make sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o.provides.build
.PHONY : sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o.provides

sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o.provides.build: sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o

# Object files for target gpsfactory
gpsfactory_OBJECTS = \
"CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o"

# External object files for target gpsfactory
gpsfactory_EXTERNAL_OBJECTS =

/home/showlove/software/zebulon/projects/asvc/2012/lib/libgpsfactory_d.so: sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o
/home/showlove/software/zebulon/projects/asvc/2012/lib/libgpsfactory_d.so: /home/showlove/software/zebulon/projects/asvc/2012/lib/libgps_d.so
/home/showlove/software/zebulon/projects/asvc/2012/lib/libgpsfactory_d.so: /home/showlove/software/zebulon/projects/asvc/2012/lib/libnmea_d.so
/home/showlove/software/zebulon/projects/asvc/2012/lib/libgpsfactory_d.so: /home/showlove/software/zebulon/projects/asvc/2012/lib/libgps_d.so
/home/showlove/software/zebulon/projects/asvc/2012/lib/libgpsfactory_d.so: sensors/gps/CMakeFiles/gpsfactory.dir/build.make
/home/showlove/software/zebulon/projects/asvc/2012/lib/libgpsfactory_d.so: sensors/gps/CMakeFiles/gpsfactory.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library /home/showlove/software/zebulon/projects/asvc/2012/lib/libgpsfactory_d.so"
	cd /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/gps && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gpsfactory.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sensors/gps/CMakeFiles/gpsfactory.dir/build: /home/showlove/software/zebulon/projects/asvc/2012/lib/libgpsfactory_d.so
.PHONY : sensors/gps/CMakeFiles/gpsfactory.dir/build

sensors/gps/CMakeFiles/gpsfactory.dir/requires: sensors/gps/CMakeFiles/gpsfactory.dir/home/showlove/software/zebulon/sensors/gps/abstract/src/gps/gpsfactory.cpp.o.requires
.PHONY : sensors/gps/CMakeFiles/gpsfactory.dir/requires

sensors/gps/CMakeFiles/gpsfactory.dir/clean:
	cd /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/gps && $(CMAKE_COMMAND) -P CMakeFiles/gpsfactory.dir/cmake_clean.cmake
.PHONY : sensors/gps/CMakeFiles/gpsfactory.dir/clean

sensors/gps/CMakeFiles/gpsfactory.dir/depend:
	cd /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/showlove/software/zebulon/projects/asvc/2012/build/cmake /home/showlove/software/zebulon/sensors/gps/abstract/build/cmake/libgpsfactory /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/gps /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/gps/CMakeFiles/gpsfactory.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sensors/gps/CMakeFiles/gpsfactory.dir/depend

