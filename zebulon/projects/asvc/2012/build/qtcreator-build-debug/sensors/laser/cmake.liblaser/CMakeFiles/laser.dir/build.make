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
include sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/depend.make

# Include the progress variables for this target.
include sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/progress.make

# Include the compile flags for this target's objects.
include sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/flags.make

sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o: sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/flags.make
sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o: /home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o"
	cd /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/laser/cmake.liblaser && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o -c /home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp

sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.i"
	cd /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/laser/cmake.liblaser && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp > CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.i

sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.s"
	cd /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/laser/cmake.liblaser && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp -o CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.s

sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o.requires:
.PHONY : sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o.requires

sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o.provides: sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o.requires
	$(MAKE) -f sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/build.make sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o.provides.build
.PHONY : sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o.provides

sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o.provides.build: sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o

# Object files for target laser
laser_OBJECTS = \
"CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o"

# External object files for target laser
laser_EXTERNAL_OBJECTS =

/home/showlove/software/zebulon/projects/asvc/2012/lib/liblaser_d.so: sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o
/home/showlove/software/zebulon/projects/asvc/2012/lib/liblaser_d.so: sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/build.make
/home/showlove/software/zebulon/projects/asvc/2012/lib/liblaser_d.so: sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library /home/showlove/software/zebulon/projects/asvc/2012/lib/liblaser_d.so"
	cd /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/laser/cmake.liblaser && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/laser.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/build: /home/showlove/software/zebulon/projects/asvc/2012/lib/liblaser_d.so
.PHONY : sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/build

sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/requires: sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/home/showlove/software/zebulon/sensors/laser/abstract/src/laser/laser.cpp.o.requires
.PHONY : sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/requires

sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/clean:
	cd /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/laser/cmake.liblaser && $(CMAKE_COMMAND) -P CMakeFiles/laser.dir/cmake_clean.cmake
.PHONY : sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/clean

sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/depend:
	cd /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/showlove/software/zebulon/projects/asvc/2012/build/cmake /home/showlove/software/zebulon/sensors/laser/abstract/build/cmake/liblaser /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/laser/cmake.liblaser /home/showlove/software/zebulon/projects/asvc/2012/build/qtcreator-build-debug/sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sensors/laser/cmake.liblaser/CMakeFiles/laser.dir/depend

