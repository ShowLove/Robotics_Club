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
include configuration/libbaseline/CMakeFiles/baseline.dir/depend.make

# Include the progress variables for this target.
include configuration/libbaseline/CMakeFiles/baseline.dir/progress.make

# Include the compile flags for this target's objects.
include configuration/libbaseline/CMakeFiles/baseline.dir/flags.make

configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o: configuration/libbaseline/CMakeFiles/baseline.dir/flags.make
configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o: /home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/configuration/libbaseline && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o -c /home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp

configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.i"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/configuration/libbaseline && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp > CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.i

configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.s"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/configuration/libbaseline && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp -o CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.s

configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o.requires:
.PHONY : configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o.requires

configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o.provides: configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o.requires
	$(MAKE) -f configuration/libbaseline/CMakeFiles/baseline.dir/build.make configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o.provides.build
.PHONY : configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o.provides

configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o.provides.build: configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o

# Object files for target baseline
baseline_OBJECTS = \
"CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o"

# External object files for target baseline
baseline_EXTERNAL_OBJECTS =

/home/showlove/software/zebulon/projects/auvc/2012/lib/libbaseline_d.so: configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o
/home/showlove/software/zebulon/projects/auvc/2012/lib/libbaseline_d.so: /home/showlove/software/zebulon/projects/auvc/2012/lib/libtinyxml_d.so
/home/showlove/software/zebulon/projects/auvc/2012/lib/libbaseline_d.so: /home/showlove/software/zebulon/projects/auvc/2012/lib/libxmlconfig_d.so
/home/showlove/software/zebulon/projects/auvc/2012/lib/libbaseline_d.so: /home/showlove/software/zebulon/projects/auvc/2012/lib/libtinyxml_d.so
/home/showlove/software/zebulon/projects/auvc/2012/lib/libbaseline_d.so: configuration/libbaseline/CMakeFiles/baseline.dir/build.make
/home/showlove/software/zebulon/projects/auvc/2012/lib/libbaseline_d.so: configuration/libbaseline/CMakeFiles/baseline.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library /home/showlove/software/zebulon/projects/auvc/2012/lib/libbaseline_d.so"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/configuration/libbaseline && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/baseline.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
configuration/libbaseline/CMakeFiles/baseline.dir/build: /home/showlove/software/zebulon/projects/auvc/2012/lib/libbaseline_d.so
.PHONY : configuration/libbaseline/CMakeFiles/baseline.dir/build

configuration/libbaseline/CMakeFiles/baseline.dir/requires: configuration/libbaseline/CMakeFiles/baseline.dir/home/showlove/software/zebulon/utility/configuration/baseline/src/baseline/baseline.cpp.o.requires
.PHONY : configuration/libbaseline/CMakeFiles/baseline.dir/requires

configuration/libbaseline/CMakeFiles/baseline.dir/clean:
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/configuration/libbaseline && $(CMAKE_COMMAND) -P CMakeFiles/baseline.dir/cmake_clean.cmake
.PHONY : configuration/libbaseline/CMakeFiles/baseline.dir/clean

configuration/libbaseline/CMakeFiles/baseline.dir/depend:
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/showlove/software/zebulon/projects/auvc/2012/build/cmake /home/showlove/software/zebulon/utility/configuration/baseline/build/cmake/libbaseline /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/configuration/libbaseline /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build-debug/configuration/libbaseline/CMakeFiles/baseline.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : configuration/libbaseline/CMakeFiles/baseline.dir/depend

