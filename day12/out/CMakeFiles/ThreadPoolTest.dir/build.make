# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/fang/ServerLinux/day12

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fang/ServerLinux/day12/out

# Include any dependencies generated for this target.
include CMakeFiles/ThreadPoolTest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ThreadPoolTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ThreadPoolTest.dir/flags.make

CMakeFiles/ThreadPoolTest.dir/src/ThreadPool.cpp.o: CMakeFiles/ThreadPoolTest.dir/flags.make
CMakeFiles/ThreadPoolTest.dir/src/ThreadPool.cpp.o: ../src/ThreadPool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fang/ServerLinux/day12/out/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ThreadPoolTest.dir/src/ThreadPool.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ThreadPoolTest.dir/src/ThreadPool.cpp.o -c /home/fang/ServerLinux/day12/src/ThreadPool.cpp

CMakeFiles/ThreadPoolTest.dir/src/ThreadPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ThreadPoolTest.dir/src/ThreadPool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fang/ServerLinux/day12/src/ThreadPool.cpp > CMakeFiles/ThreadPoolTest.dir/src/ThreadPool.cpp.i

CMakeFiles/ThreadPoolTest.dir/src/ThreadPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ThreadPoolTest.dir/src/ThreadPool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fang/ServerLinux/day12/src/ThreadPool.cpp -o CMakeFiles/ThreadPoolTest.dir/src/ThreadPool.cpp.s

CMakeFiles/ThreadPoolTest.dir/ThreadPoolTest.cpp.o: CMakeFiles/ThreadPoolTest.dir/flags.make
CMakeFiles/ThreadPoolTest.dir/ThreadPoolTest.cpp.o: ../ThreadPoolTest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fang/ServerLinux/day12/out/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ThreadPoolTest.dir/ThreadPoolTest.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ThreadPoolTest.dir/ThreadPoolTest.cpp.o -c /home/fang/ServerLinux/day12/ThreadPoolTest.cpp

CMakeFiles/ThreadPoolTest.dir/ThreadPoolTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ThreadPoolTest.dir/ThreadPoolTest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fang/ServerLinux/day12/ThreadPoolTest.cpp > CMakeFiles/ThreadPoolTest.dir/ThreadPoolTest.cpp.i

CMakeFiles/ThreadPoolTest.dir/ThreadPoolTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ThreadPoolTest.dir/ThreadPoolTest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fang/ServerLinux/day12/ThreadPoolTest.cpp -o CMakeFiles/ThreadPoolTest.dir/ThreadPoolTest.cpp.s

# Object files for target ThreadPoolTest
ThreadPoolTest_OBJECTS = \
"CMakeFiles/ThreadPoolTest.dir/src/ThreadPool.cpp.o" \
"CMakeFiles/ThreadPoolTest.dir/ThreadPoolTest.cpp.o"

# External object files for target ThreadPoolTest
ThreadPoolTest_EXTERNAL_OBJECTS =

ThreadPoolTest: CMakeFiles/ThreadPoolTest.dir/src/ThreadPool.cpp.o
ThreadPoolTest: CMakeFiles/ThreadPoolTest.dir/ThreadPoolTest.cpp.o
ThreadPoolTest: CMakeFiles/ThreadPoolTest.dir/build.make
ThreadPoolTest: CMakeFiles/ThreadPoolTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fang/ServerLinux/day12/out/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ThreadPoolTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ThreadPoolTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ThreadPoolTest.dir/build: ThreadPoolTest

.PHONY : CMakeFiles/ThreadPoolTest.dir/build

CMakeFiles/ThreadPoolTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ThreadPoolTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ThreadPoolTest.dir/clean

CMakeFiles/ThreadPoolTest.dir/depend:
	cd /home/fang/ServerLinux/day12/out && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fang/ServerLinux/day12 /home/fang/ServerLinux/day12 /home/fang/ServerLinux/day12/out /home/fang/ServerLinux/day12/out /home/fang/ServerLinux/day12/out/CMakeFiles/ThreadPoolTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ThreadPoolTest.dir/depend

