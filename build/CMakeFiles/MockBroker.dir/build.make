# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/walczaksz/projects/cpp-testy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/walczaksz/projects/cpp-testy/build

# Include any dependencies generated for this target.
include CMakeFiles/MockBroker.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MockBroker.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MockBroker.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MockBroker.dir/flags.make

CMakeFiles/MockBroker.dir/src/main_broker.cpp.o: CMakeFiles/MockBroker.dir/flags.make
CMakeFiles/MockBroker.dir/src/main_broker.cpp.o: ../src/main_broker.cpp
CMakeFiles/MockBroker.dir/src/main_broker.cpp.o: CMakeFiles/MockBroker.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/walczaksz/projects/cpp-testy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MockBroker.dir/src/main_broker.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MockBroker.dir/src/main_broker.cpp.o -MF CMakeFiles/MockBroker.dir/src/main_broker.cpp.o.d -o CMakeFiles/MockBroker.dir/src/main_broker.cpp.o -c /home/walczaksz/projects/cpp-testy/src/main_broker.cpp

CMakeFiles/MockBroker.dir/src/main_broker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MockBroker.dir/src/main_broker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/walczaksz/projects/cpp-testy/src/main_broker.cpp > CMakeFiles/MockBroker.dir/src/main_broker.cpp.i

CMakeFiles/MockBroker.dir/src/main_broker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MockBroker.dir/src/main_broker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/walczaksz/projects/cpp-testy/src/main_broker.cpp -o CMakeFiles/MockBroker.dir/src/main_broker.cpp.s

CMakeFiles/MockBroker.dir/src/broker/Server.cpp.o: CMakeFiles/MockBroker.dir/flags.make
CMakeFiles/MockBroker.dir/src/broker/Server.cpp.o: ../src/broker/Server.cpp
CMakeFiles/MockBroker.dir/src/broker/Server.cpp.o: CMakeFiles/MockBroker.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/walczaksz/projects/cpp-testy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MockBroker.dir/src/broker/Server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MockBroker.dir/src/broker/Server.cpp.o -MF CMakeFiles/MockBroker.dir/src/broker/Server.cpp.o.d -o CMakeFiles/MockBroker.dir/src/broker/Server.cpp.o -c /home/walczaksz/projects/cpp-testy/src/broker/Server.cpp

CMakeFiles/MockBroker.dir/src/broker/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MockBroker.dir/src/broker/Server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/walczaksz/projects/cpp-testy/src/broker/Server.cpp > CMakeFiles/MockBroker.dir/src/broker/Server.cpp.i

CMakeFiles/MockBroker.dir/src/broker/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MockBroker.dir/src/broker/Server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/walczaksz/projects/cpp-testy/src/broker/Server.cpp -o CMakeFiles/MockBroker.dir/src/broker/Server.cpp.s

# Object files for target MockBroker
MockBroker_OBJECTS = \
"CMakeFiles/MockBroker.dir/src/main_broker.cpp.o" \
"CMakeFiles/MockBroker.dir/src/broker/Server.cpp.o"

# External object files for target MockBroker
MockBroker_EXTERNAL_OBJECTS =

MockBroker: CMakeFiles/MockBroker.dir/src/main_broker.cpp.o
MockBroker: CMakeFiles/MockBroker.dir/src/broker/Server.cpp.o
MockBroker: CMakeFiles/MockBroker.dir/build.make
MockBroker: CMakeFiles/MockBroker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/walczaksz/projects/cpp-testy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable MockBroker"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MockBroker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MockBroker.dir/build: MockBroker
.PHONY : CMakeFiles/MockBroker.dir/build

CMakeFiles/MockBroker.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MockBroker.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MockBroker.dir/clean

CMakeFiles/MockBroker.dir/depend:
	cd /home/walczaksz/projects/cpp-testy/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/walczaksz/projects/cpp-testy /home/walczaksz/projects/cpp-testy /home/walczaksz/projects/cpp-testy/build /home/walczaksz/projects/cpp-testy/build /home/walczaksz/projects/cpp-testy/build/CMakeFiles/MockBroker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MockBroker.dir/depend

