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

# Include any dependencies generated for this target.
include libSolInstru/CMakeFiles/SolInstru.dir/depend.make

# Include the progress variables for this target.
include libSolInstru/CMakeFiles/SolInstru.dir/progress.make

# Include the compile flags for this target's objects.
include libSolInstru/CMakeFiles/SolInstru.dir/flags.make

libSolInstru/CMakeFiles/SolInstru.dir/ASTAnalyser.cpp.o: libSolInstru/CMakeFiles/SolInstru.dir/flags.make
libSolInstru/CMakeFiles/SolInstru.dir/ASTAnalyser.cpp.o: ../libSolInstru/ASTAnalyser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s1798183/Documents/TestSolidity-master/MuContract/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libSolInstru/CMakeFiles/SolInstru.dir/ASTAnalyser.cpp.o"
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SolInstru.dir/ASTAnalyser.cpp.o -c /home/s1798183/Documents/TestSolidity-master/MuContract/libSolInstru/ASTAnalyser.cpp

libSolInstru/CMakeFiles/SolInstru.dir/ASTAnalyser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SolInstru.dir/ASTAnalyser.cpp.i"
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s1798183/Documents/TestSolidity-master/MuContract/libSolInstru/ASTAnalyser.cpp > CMakeFiles/SolInstru.dir/ASTAnalyser.cpp.i

libSolInstru/CMakeFiles/SolInstru.dir/ASTAnalyser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SolInstru.dir/ASTAnalyser.cpp.s"
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s1798183/Documents/TestSolidity-master/MuContract/libSolInstru/ASTAnalyser.cpp -o CMakeFiles/SolInstru.dir/ASTAnalyser.cpp.s

libSolInstru/CMakeFiles/SolInstru.dir/ASTNodes.cpp.o: libSolInstru/CMakeFiles/SolInstru.dir/flags.make
libSolInstru/CMakeFiles/SolInstru.dir/ASTNodes.cpp.o: ../libSolInstru/ASTNodes.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s1798183/Documents/TestSolidity-master/MuContract/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object libSolInstru/CMakeFiles/SolInstru.dir/ASTNodes.cpp.o"
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SolInstru.dir/ASTNodes.cpp.o -c /home/s1798183/Documents/TestSolidity-master/MuContract/libSolInstru/ASTNodes.cpp

libSolInstru/CMakeFiles/SolInstru.dir/ASTNodes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SolInstru.dir/ASTNodes.cpp.i"
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s1798183/Documents/TestSolidity-master/MuContract/libSolInstru/ASTNodes.cpp > CMakeFiles/SolInstru.dir/ASTNodes.cpp.i

libSolInstru/CMakeFiles/SolInstru.dir/ASTNodes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SolInstru.dir/ASTNodes.cpp.s"
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s1798183/Documents/TestSolidity-master/MuContract/libSolInstru/ASTNodes.cpp -o CMakeFiles/SolInstru.dir/ASTNodes.cpp.s

libSolInstru/CMakeFiles/SolInstru.dir/ASTVisitor.cpp.o: libSolInstru/CMakeFiles/SolInstru.dir/flags.make
libSolInstru/CMakeFiles/SolInstru.dir/ASTVisitor.cpp.o: ../libSolInstru/ASTVisitor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s1798183/Documents/TestSolidity-master/MuContract/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object libSolInstru/CMakeFiles/SolInstru.dir/ASTVisitor.cpp.o"
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SolInstru.dir/ASTVisitor.cpp.o -c /home/s1798183/Documents/TestSolidity-master/MuContract/libSolInstru/ASTVisitor.cpp

libSolInstru/CMakeFiles/SolInstru.dir/ASTVisitor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SolInstru.dir/ASTVisitor.cpp.i"
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s1798183/Documents/TestSolidity-master/MuContract/libSolInstru/ASTVisitor.cpp > CMakeFiles/SolInstru.dir/ASTVisitor.cpp.i

libSolInstru/CMakeFiles/SolInstru.dir/ASTVisitor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SolInstru.dir/ASTVisitor.cpp.s"
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s1798183/Documents/TestSolidity-master/MuContract/libSolInstru/ASTVisitor.cpp -o CMakeFiles/SolInstru.dir/ASTVisitor.cpp.s

# Object files for target SolInstru
SolInstru_OBJECTS = \
"CMakeFiles/SolInstru.dir/ASTAnalyser.cpp.o" \
"CMakeFiles/SolInstru.dir/ASTNodes.cpp.o" \
"CMakeFiles/SolInstru.dir/ASTVisitor.cpp.o"

# External object files for target SolInstru
SolInstru_EXTERNAL_OBJECTS =

libSolInstru/libSolInstru.a: libSolInstru/CMakeFiles/SolInstru.dir/ASTAnalyser.cpp.o
libSolInstru/libSolInstru.a: libSolInstru/CMakeFiles/SolInstru.dir/ASTNodes.cpp.o
libSolInstru/libSolInstru.a: libSolInstru/CMakeFiles/SolInstru.dir/ASTVisitor.cpp.o
libSolInstru/libSolInstru.a: libSolInstru/CMakeFiles/SolInstru.dir/build.make
libSolInstru/libSolInstru.a: libSolInstru/CMakeFiles/SolInstru.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/s1798183/Documents/TestSolidity-master/MuContract/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libSolInstru.a"
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru && $(CMAKE_COMMAND) -P CMakeFiles/SolInstru.dir/cmake_clean_target.cmake
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SolInstru.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libSolInstru/CMakeFiles/SolInstru.dir/build: libSolInstru/libSolInstru.a

.PHONY : libSolInstru/CMakeFiles/SolInstru.dir/build

libSolInstru/CMakeFiles/SolInstru.dir/clean:
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru && $(CMAKE_COMMAND) -P CMakeFiles/SolInstru.dir/cmake_clean.cmake
.PHONY : libSolInstru/CMakeFiles/SolInstru.dir/clean

libSolInstru/CMakeFiles/SolInstru.dir/depend:
	cd /home/s1798183/Documents/TestSolidity-master/MuContract/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/s1798183/Documents/TestSolidity-master/MuContract /home/s1798183/Documents/TestSolidity-master/MuContract/libSolInstru /home/s1798183/Documents/TestSolidity-master/MuContract/build /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru /home/s1798183/Documents/TestSolidity-master/MuContract/build/libSolInstru/CMakeFiles/SolInstru.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libSolInstru/CMakeFiles/SolInstru.dir/depend

