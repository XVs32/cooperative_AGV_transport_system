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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/standalone/src/lib/raspicam-0.1.6

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/standalone/src/lib/raspicam-0.1.6/build

# Include any dependencies generated for this target.
include src/CMakeFiles/raspicam.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/raspicam.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/raspicam.dir/flags.make

src/CMakeFiles/raspicam.dir/raspicam.cpp.o: src/CMakeFiles/raspicam.dir/flags.make
src/CMakeFiles/raspicam.dir/raspicam.cpp.o: ../src/raspicam.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/standalone/src/lib/raspicam-0.1.6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/raspicam.dir/raspicam.cpp.o"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/raspicam.dir/raspicam.cpp.o -c /home/pi/standalone/src/lib/raspicam-0.1.6/src/raspicam.cpp

src/CMakeFiles/raspicam.dir/raspicam.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/raspicam.dir/raspicam.cpp.i"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/standalone/src/lib/raspicam-0.1.6/src/raspicam.cpp > CMakeFiles/raspicam.dir/raspicam.cpp.i

src/CMakeFiles/raspicam.dir/raspicam.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/raspicam.dir/raspicam.cpp.s"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/standalone/src/lib/raspicam-0.1.6/src/raspicam.cpp -o CMakeFiles/raspicam.dir/raspicam.cpp.s

src/CMakeFiles/raspicam.dir/raspicam_still.cpp.o: src/CMakeFiles/raspicam.dir/flags.make
src/CMakeFiles/raspicam.dir/raspicam_still.cpp.o: ../src/raspicam_still.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/standalone/src/lib/raspicam-0.1.6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/raspicam.dir/raspicam_still.cpp.o"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/raspicam.dir/raspicam_still.cpp.o -c /home/pi/standalone/src/lib/raspicam-0.1.6/src/raspicam_still.cpp

src/CMakeFiles/raspicam.dir/raspicam_still.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/raspicam.dir/raspicam_still.cpp.i"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/standalone/src/lib/raspicam-0.1.6/src/raspicam_still.cpp > CMakeFiles/raspicam.dir/raspicam_still.cpp.i

src/CMakeFiles/raspicam.dir/raspicam_still.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/raspicam.dir/raspicam_still.cpp.s"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/standalone/src/lib/raspicam-0.1.6/src/raspicam_still.cpp -o CMakeFiles/raspicam.dir/raspicam_still.cpp.s

src/CMakeFiles/raspicam.dir/private/private_impl.cpp.o: src/CMakeFiles/raspicam.dir/flags.make
src/CMakeFiles/raspicam.dir/private/private_impl.cpp.o: ../src/private/private_impl.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/standalone/src/lib/raspicam-0.1.6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/raspicam.dir/private/private_impl.cpp.o"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/raspicam.dir/private/private_impl.cpp.o -c /home/pi/standalone/src/lib/raspicam-0.1.6/src/private/private_impl.cpp

src/CMakeFiles/raspicam.dir/private/private_impl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/raspicam.dir/private/private_impl.cpp.i"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/standalone/src/lib/raspicam-0.1.6/src/private/private_impl.cpp > CMakeFiles/raspicam.dir/private/private_impl.cpp.i

src/CMakeFiles/raspicam.dir/private/private_impl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/raspicam.dir/private/private_impl.cpp.s"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/standalone/src/lib/raspicam-0.1.6/src/private/private_impl.cpp -o CMakeFiles/raspicam.dir/private/private_impl.cpp.s

src/CMakeFiles/raspicam.dir/private/threadcondition.cpp.o: src/CMakeFiles/raspicam.dir/flags.make
src/CMakeFiles/raspicam.dir/private/threadcondition.cpp.o: ../src/private/threadcondition.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/standalone/src/lib/raspicam-0.1.6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/raspicam.dir/private/threadcondition.cpp.o"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/raspicam.dir/private/threadcondition.cpp.o -c /home/pi/standalone/src/lib/raspicam-0.1.6/src/private/threadcondition.cpp

src/CMakeFiles/raspicam.dir/private/threadcondition.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/raspicam.dir/private/threadcondition.cpp.i"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/standalone/src/lib/raspicam-0.1.6/src/private/threadcondition.cpp > CMakeFiles/raspicam.dir/private/threadcondition.cpp.i

src/CMakeFiles/raspicam.dir/private/threadcondition.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/raspicam.dir/private/threadcondition.cpp.s"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/standalone/src/lib/raspicam-0.1.6/src/private/threadcondition.cpp -o CMakeFiles/raspicam.dir/private/threadcondition.cpp.s

src/CMakeFiles/raspicam.dir/private_still/private_still_impl.cpp.o: src/CMakeFiles/raspicam.dir/flags.make
src/CMakeFiles/raspicam.dir/private_still/private_still_impl.cpp.o: ../src/private_still/private_still_impl.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/standalone/src/lib/raspicam-0.1.6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/raspicam.dir/private_still/private_still_impl.cpp.o"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/raspicam.dir/private_still/private_still_impl.cpp.o -c /home/pi/standalone/src/lib/raspicam-0.1.6/src/private_still/private_still_impl.cpp

src/CMakeFiles/raspicam.dir/private_still/private_still_impl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/raspicam.dir/private_still/private_still_impl.cpp.i"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/standalone/src/lib/raspicam-0.1.6/src/private_still/private_still_impl.cpp > CMakeFiles/raspicam.dir/private_still/private_still_impl.cpp.i

src/CMakeFiles/raspicam.dir/private_still/private_still_impl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/raspicam.dir/private_still/private_still_impl.cpp.s"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/standalone/src/lib/raspicam-0.1.6/src/private_still/private_still_impl.cpp -o CMakeFiles/raspicam.dir/private_still/private_still_impl.cpp.s

# Object files for target raspicam
raspicam_OBJECTS = \
"CMakeFiles/raspicam.dir/raspicam.cpp.o" \
"CMakeFiles/raspicam.dir/raspicam_still.cpp.o" \
"CMakeFiles/raspicam.dir/private/private_impl.cpp.o" \
"CMakeFiles/raspicam.dir/private/threadcondition.cpp.o" \
"CMakeFiles/raspicam.dir/private_still/private_still_impl.cpp.o"

# External object files for target raspicam
raspicam_EXTERNAL_OBJECTS =

src/libraspicam.so.0.1.6: src/CMakeFiles/raspicam.dir/raspicam.cpp.o
src/libraspicam.so.0.1.6: src/CMakeFiles/raspicam.dir/raspicam_still.cpp.o
src/libraspicam.so.0.1.6: src/CMakeFiles/raspicam.dir/private/private_impl.cpp.o
src/libraspicam.so.0.1.6: src/CMakeFiles/raspicam.dir/private/threadcondition.cpp.o
src/libraspicam.so.0.1.6: src/CMakeFiles/raspicam.dir/private_still/private_still_impl.cpp.o
src/libraspicam.so.0.1.6: src/CMakeFiles/raspicam.dir/build.make
src/libraspicam.so.0.1.6: /opt/vc/lib/libmmal_core.so
src/libraspicam.so.0.1.6: /opt/vc/lib/libmmal_util.so
src/libraspicam.so.0.1.6: /opt/vc/lib/libmmal.so
src/libraspicam.so.0.1.6: src/CMakeFiles/raspicam.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/standalone/src/lib/raspicam-0.1.6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX shared library libraspicam.so"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/raspicam.dir/link.txt --verbose=$(VERBOSE)
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && $(CMAKE_COMMAND) -E cmake_symlink_library libraspicam.so.0.1.6 libraspicam.so.0.1 libraspicam.so

src/libraspicam.so.0.1: src/libraspicam.so.0.1.6
	@$(CMAKE_COMMAND) -E touch_nocreate src/libraspicam.so.0.1

src/libraspicam.so: src/libraspicam.so.0.1.6
	@$(CMAKE_COMMAND) -E touch_nocreate src/libraspicam.so

# Rule to build all files generated by this target.
src/CMakeFiles/raspicam.dir/build: src/libraspicam.so

.PHONY : src/CMakeFiles/raspicam.dir/build

src/CMakeFiles/raspicam.dir/clean:
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && $(CMAKE_COMMAND) -P CMakeFiles/raspicam.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/raspicam.dir/clean

src/CMakeFiles/raspicam.dir/depend:
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/standalone/src/lib/raspicam-0.1.6 /home/pi/standalone/src/lib/raspicam-0.1.6/src /home/pi/standalone/src/lib/raspicam-0.1.6/build /home/pi/standalone/src/lib/raspicam-0.1.6/build/src /home/pi/standalone/src/lib/raspicam-0.1.6/build/src/CMakeFiles/raspicam.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/raspicam.dir/depend

