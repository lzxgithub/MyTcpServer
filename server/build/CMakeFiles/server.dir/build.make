# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /opt/cmake-3.14.5/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.14.5/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/terry/Workspace/commlib/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/terry/Workspace/commlib/server/build

# Include any dependencies generated for this target.
include CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.dir/flags.make

CMakeFiles/server.dir/server/server.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/server/server.cpp.o: ../server/server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/terry/Workspace/commlib/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server.dir/server/server.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/server/server.cpp.o -c /home/terry/Workspace/commlib/server/server/server.cpp

CMakeFiles/server.dir/server/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/server/server.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/terry/Workspace/commlib/server/server/server.cpp > CMakeFiles/server.dir/server/server.cpp.i

CMakeFiles/server.dir/server/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/server/server.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/terry/Workspace/commlib/server/server/server.cpp -o CMakeFiles/server.dir/server/server.cpp.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/server/server.cpp.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

bin/server: CMakeFiles/server.dir/server/server.cpp.o
bin/server: CMakeFiles/server.dir/build.make
bin/server: CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/terry/Workspace/commlib/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.dir/build: bin/server

.PHONY : CMakeFiles/server.dir/build

CMakeFiles/server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.dir/clean

CMakeFiles/server.dir/depend:
	cd /home/terry/Workspace/commlib/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/terry/Workspace/commlib/server /home/terry/Workspace/commlib/server /home/terry/Workspace/commlib/server/build /home/terry/Workspace/commlib/server/build /home/terry/Workspace/commlib/server/build/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server.dir/depend
