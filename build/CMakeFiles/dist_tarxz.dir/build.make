# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/philip/eigen-cddlib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/philip/eigen-cddlib/build

# Utility rule file for dist_tarxz.

# Include the progress variables for this target.
include CMakeFiles/dist_tarxz.dir/progress.make

CMakeFiles/dist_tarxz:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/philip/eigen-cddlib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating tar.xz tarball and its signature..."
	/bin/tar -cJf eigen-cddlib-0.4.4-9-gc777-dirty.tar.xz eigen-cddlib-0.4.4-9-gc777-dirty/ && /usr/bin/gpg --detach-sign --armor -o /home/philip/eigen-cddlib/build/eigen-cddlib-0.4.4-9-gc777-dirty.tar.xz.sig /home/philip/eigen-cddlib/build/eigen-cddlib-0.4.4-9-gc777-dirty.tar.xz

dist_tarxz: CMakeFiles/dist_tarxz
dist_tarxz: CMakeFiles/dist_tarxz.dir/build.make

.PHONY : dist_tarxz

# Rule to build all files generated by this target.
CMakeFiles/dist_tarxz.dir/build: dist_tarxz

.PHONY : CMakeFiles/dist_tarxz.dir/build

CMakeFiles/dist_tarxz.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dist_tarxz.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dist_tarxz.dir/clean

CMakeFiles/dist_tarxz.dir/depend:
	cd /home/philip/eigen-cddlib/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/philip/eigen-cddlib /home/philip/eigen-cddlib /home/philip/eigen-cddlib/build /home/philip/eigen-cddlib/build /home/philip/eigen-cddlib/build/CMakeFiles/dist_tarxz.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dist_tarxz.dir/depend

