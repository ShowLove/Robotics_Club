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
include configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/depend.make

# Include the progress variables for this target.
include configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/progress.make

# Include the compile flags for this target's objects.
include configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/flags.make

configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o: configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/flags.make
configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o: /home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/configuration/libxmlconfig && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o -c /home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp

configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.i"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/configuration/libxmlconfig && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp > CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.i

configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.s"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/configuration/libxmlconfig && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp -o CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.s

configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o.requires:
.PHONY : configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o.requires

configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o.provides: configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o.requires
	$(MAKE) -f configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/build.make configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o.provides.build
.PHONY : configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o.provides

configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o.provides.build: configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o

# Object files for target xmlconfig
xmlconfig_OBJECTS = \
"CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o"

# External object files for target xmlconfig
xmlconfig_EXTERNAL_OBJECTS =

/home/showlove/software/zebulon/projects/auvc/2012/lib/libxmlconfig.so: configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o
/home/showlove/software/zebulon/projects/auvc/2012/lib/libxmlconfig.so: /home/showlove/software/zebulon/projects/auvc/2012/lib/libtinyxml.so
/home/showlove/software/zebulon/projects/auvc/2012/lib/libxmlconfig.so: configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/build.make
/home/showlove/software/zebulon/projects/auvc/2012/lib/libxmlconfig.so: configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library /home/showlove/software/zebulon/projects/auvc/2012/lib/libxmlconfig.so"
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/configuration/libxmlconfig && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/xmlconfig.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/build: /home/showlove/software/zebulon/projects/auvc/2012/lib/libxmlconfig.so
.PHONY : configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/build

configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/requires: configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/home/showlove/software/zebulon/utility/configuration/xmlconfig/src/xmlconfig/xmlconfigurator.cpp.o.requires
.PHONY : configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/requires

configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/clean:
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/configuration/libxmlconfig && $(CMAKE_COMMAND) -P CMakeFiles/xmlconfig.dir/cmake_clean.cmake
.PHONY : configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/clean

configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/depend:
	cd /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/showlove/software/zebulon/projects/auvc/2012/build/cmake /home/showlove/software/zebulon/utility/configuration/xmlconfig/build/cmake/libxmlconfig /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/configuration/libxmlconfig /home/showlove/software/zebulon/projects/auvc/2012/build/qtcreator-build/configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : configuration/libxmlconfig/CMakeFiles/xmlconfig.dir/depend
