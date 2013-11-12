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
CMAKE_BINARY_DIR = /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build

# Include any dependencies generated for this target.
include sensors/keller/CMakeFiles/keller.dir/depend.make

# Include the progress variables for this target.
include sensors/keller/CMakeFiles/keller.dir/progress.make

# Include the compile flags for this target's objects.
include sensors/keller/CMakeFiles/keller.dir/flags.make

sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o: sensors/keller/CMakeFiles/keller.dir/flags.make
sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o: /home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/sensors/keller && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o -c /home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp

sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.i"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/sensors/keller && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp > CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.i

sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.s"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/sensors/keller && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp -o CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.s

sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o.requires:
.PHONY : sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o.requires

sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o.provides: sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o.requires
	$(MAKE) -f sensors/keller/CMakeFiles/keller.dir/build.make sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o.provides.build
.PHONY : sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o.provides

sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o.provides.build: sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o

# Object files for target keller
keller_OBJECTS = \
"CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o"

# External object files for target keller
keller_EXTERNAL_OBJECTS =

/home/showlove/software/zebulon/projects/auvc/2012/lib/libkeller.so: sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o
/home/showlove/software/zebulon/projects/auvc/2012/lib/libkeller.so: sensors/keller/CMakeFiles/keller.dir/build.make
/home/showlove/software/zebulon/projects/auvc/2012/lib/libkeller.so: sensors/keller/CMakeFiles/keller.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library /home/showlove/software/zebulon/projects/auvc/2012/lib/libkeller.so"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/sensors/keller && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/keller.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sensors/keller/CMakeFiles/keller.dir/build: /home/showlove/software/zebulon/projects/auvc/2012/lib/libkeller.so
.PHONY : sensors/keller/CMakeFiles/keller.dir/build

sensors/keller/CMakeFiles/keller.dir/requires: sensors/keller/CMakeFiles/keller.dir/home/showlove/software/zebulon/sensors/depth/keller/src/keller/keller.cpp.o.requires
.PHONY : sensors/keller/CMakeFiles/keller.dir/requires

sensors/keller/CMakeFiles/keller.dir/clean:
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/sensors/keller && $(CMAKE_COMMAND) -P CMakeFiles/keller.dir/cmake_clean.cmake
.PHONY : sensors/keller/CMakeFiles/keller.dir/clean

sensors/keller/CMakeFiles/keller.dir/depend:
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/showlove/software/zebulon/projects/auvc/2012/build/cmake /home/showlove/software/zebulon/sensors/depth/keller/build/cmake/libkeller /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/sensors/keller /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/sensors/keller/CMakeFiles/keller.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sensors/keller/CMakeFiles/keller.dir/depend

