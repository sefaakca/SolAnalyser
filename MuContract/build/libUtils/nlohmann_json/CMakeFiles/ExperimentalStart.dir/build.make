# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/s1798183/Documents/TestSolidity-master/MuContract

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/s1798183/Documents/TestSolidity-master/MuContract/build

# Utility rule file for ExperimentalStart.

# Include the progress variables for this target.
include libUtils/nlohmann_json/CMakeFiles/ExperimentalStart.dir/progress.make

libUtils/nlohmann_json/CMakeFiles/ExperimentalStart:
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libUtils/nlohmann_json && /usr/local/bin/ctest -D ExperimentalStart

ExperimentalStart: libUtils/nlohmann_json/CMakeFiles/ExperimentalStart
ExperimentalStart: libUtils/nlohmann_json/CMakeFiles/ExperimentalStart.dir/build.make

.PHONY : ExperimentalStart

# Rule to build all files generated by this target.
libUtils/nlohmann_json/CMakeFiles/ExperimentalStart.dir/build: ExperimentalStart

.PHONY : libUtils/nlohmann_json/CMakeFiles/ExperimentalStart.dir/build

libUtils/nlohmann_json/CMakeFiles/ExperimentalStart.dir/clean:
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libUtils/nlohmann_json && $(CMAKE_COMMAND) -P CMakeFiles/ExperimentalStart.dir/cmake_clean.cmake
.PHONY : libUtils/nlohmann_json/CMakeFiles/ExperimentalStart.dir/clean

libUtils/nlohmann_json/CMakeFiles/ExperimentalStart.dir/depend:
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/s1798183/Documents/TestSolidity-master/MuContract /home/s1798183/Documents/TestSolidity-master/MuContract/libUtils/nlohmann_json /home/s1798183/Documents/TestSolidity-master/MuContract/build /home/s1798183/Documents/TestSolidity-master/MuContract/build/libUtils/nlohmann_json /home/s1798183/Documents/TestSolidity-master/MuContract/build/libUtils/nlohmann_json/CMakeFiles/ExperimentalStart.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libUtils/nlohmann_json/CMakeFiles/ExperimentalStart.dir/depend

