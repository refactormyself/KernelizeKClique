# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/codeblocks-build

# Include any dependencies generated for this target.
include CMakeFiles/dominate.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dominate.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dominate.dir/flags.make

CMakeFiles/dominate.dir/src/main.cpp.o: CMakeFiles/dominate.dir/flags.make
CMakeFiles/dominate.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/codeblocks-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/dominate.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dominate.dir/src/main.cpp.o -c /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/src/main.cpp

CMakeFiles/dominate.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dominate.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/src/main.cpp > CMakeFiles/dominate.dir/src/main.cpp.i

CMakeFiles/dominate.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dominate.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/src/main.cpp -o CMakeFiles/dominate.dir/src/main.cpp.s

CMakeFiles/dominate.dir/src/graph.cpp.o: CMakeFiles/dominate.dir/flags.make
CMakeFiles/dominate.dir/src/graph.cpp.o: ../src/graph.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/codeblocks-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/dominate.dir/src/graph.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dominate.dir/src/graph.cpp.o -c /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/src/graph.cpp

CMakeFiles/dominate.dir/src/graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dominate.dir/src/graph.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/src/graph.cpp > CMakeFiles/dominate.dir/src/graph.cpp.i

CMakeFiles/dominate.dir/src/graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dominate.dir/src/graph.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/src/graph.cpp -o CMakeFiles/dominate.dir/src/graph.cpp.s

CMakeFiles/dominate.dir/src/serial_dominate.cpp.o: CMakeFiles/dominate.dir/flags.make
CMakeFiles/dominate.dir/src/serial_dominate.cpp.o: ../src/serial_dominate.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/codeblocks-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/dominate.dir/src/serial_dominate.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dominate.dir/src/serial_dominate.cpp.o -c /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/src/serial_dominate.cpp

CMakeFiles/dominate.dir/src/serial_dominate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dominate.dir/src/serial_dominate.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/src/serial_dominate.cpp > CMakeFiles/dominate.dir/src/serial_dominate.cpp.i

CMakeFiles/dominate.dir/src/serial_dominate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dominate.dir/src/serial_dominate.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/src/serial_dominate.cpp -o CMakeFiles/dominate.dir/src/serial_dominate.cpp.s

CMakeFiles/dominate.dir/src/omp_dominate.cpp.o: CMakeFiles/dominate.dir/flags.make
CMakeFiles/dominate.dir/src/omp_dominate.cpp.o: ../src/omp_dominate.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/codeblocks-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/dominate.dir/src/omp_dominate.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dominate.dir/src/omp_dominate.cpp.o -c /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/src/omp_dominate.cpp

CMakeFiles/dominate.dir/src/omp_dominate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dominate.dir/src/omp_dominate.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/src/omp_dominate.cpp > CMakeFiles/dominate.dir/src/omp_dominate.cpp.i

CMakeFiles/dominate.dir/src/omp_dominate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dominate.dir/src/omp_dominate.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/src/omp_dominate.cpp -o CMakeFiles/dominate.dir/src/omp_dominate.cpp.s

# Object files for target dominate
dominate_OBJECTS = \
"CMakeFiles/dominate.dir/src/main.cpp.o" \
"CMakeFiles/dominate.dir/src/graph.cpp.o" \
"CMakeFiles/dominate.dir/src/serial_dominate.cpp.o" \
"CMakeFiles/dominate.dir/src/omp_dominate.cpp.o"

# External object files for target dominate
dominate_EXTERNAL_OBJECTS =

dominate: CMakeFiles/dominate.dir/src/main.cpp.o
dominate: CMakeFiles/dominate.dir/src/graph.cpp.o
dominate: CMakeFiles/dominate.dir/src/serial_dominate.cpp.o
dominate: CMakeFiles/dominate.dir/src/omp_dominate.cpp.o
dominate: CMakeFiles/dominate.dir/build.make
dominate: CMakeFiles/dominate.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/codeblocks-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable dominate"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dominate.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dominate.dir/build: dominate

.PHONY : CMakeFiles/dominate.dir/build

CMakeFiles/dominate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dominate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dominate.dir/clean

CMakeFiles/dominate.dir/depend:
	cd /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/codeblocks-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/codeblocks-build /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/codeblocks-build /home/saheed/DEV/MSc/KCliqueKernel/Submit/v_final/codeblocks-build/CMakeFiles/dominate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dominate.dir/depend

