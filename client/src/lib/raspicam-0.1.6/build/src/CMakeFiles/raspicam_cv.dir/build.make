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
include src/CMakeFiles/raspicam_cv.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/raspicam_cv.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/raspicam_cv.dir/flags.make

src/CMakeFiles/raspicam_cv.dir/raspicam_cv.cpp.o: src/CMakeFiles/raspicam_cv.dir/flags.make
src/CMakeFiles/raspicam_cv.dir/raspicam_cv.cpp.o: ../src/raspicam_cv.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/standalone/src/lib/raspicam-0.1.6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/raspicam_cv.dir/raspicam_cv.cpp.o"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/raspicam_cv.dir/raspicam_cv.cpp.o -c /home/pi/standalone/src/lib/raspicam-0.1.6/src/raspicam_cv.cpp

src/CMakeFiles/raspicam_cv.dir/raspicam_cv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/raspicam_cv.dir/raspicam_cv.cpp.i"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/standalone/src/lib/raspicam-0.1.6/src/raspicam_cv.cpp > CMakeFiles/raspicam_cv.dir/raspicam_cv.cpp.i

src/CMakeFiles/raspicam_cv.dir/raspicam_cv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/raspicam_cv.dir/raspicam_cv.cpp.s"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/standalone/src/lib/raspicam-0.1.6/src/raspicam_cv.cpp -o CMakeFiles/raspicam_cv.dir/raspicam_cv.cpp.s

src/CMakeFiles/raspicam_cv.dir/raspicam_still_cv.cpp.o: src/CMakeFiles/raspicam_cv.dir/flags.make
src/CMakeFiles/raspicam_cv.dir/raspicam_still_cv.cpp.o: ../src/raspicam_still_cv.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/standalone/src/lib/raspicam-0.1.6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/raspicam_cv.dir/raspicam_still_cv.cpp.o"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/raspicam_cv.dir/raspicam_still_cv.cpp.o -c /home/pi/standalone/src/lib/raspicam-0.1.6/src/raspicam_still_cv.cpp

src/CMakeFiles/raspicam_cv.dir/raspicam_still_cv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/raspicam_cv.dir/raspicam_still_cv.cpp.i"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/standalone/src/lib/raspicam-0.1.6/src/raspicam_still_cv.cpp > CMakeFiles/raspicam_cv.dir/raspicam_still_cv.cpp.i

src/CMakeFiles/raspicam_cv.dir/raspicam_still_cv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/raspicam_cv.dir/raspicam_still_cv.cpp.s"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/standalone/src/lib/raspicam-0.1.6/src/raspicam_still_cv.cpp -o CMakeFiles/raspicam_cv.dir/raspicam_still_cv.cpp.s

# Object files for target raspicam_cv
raspicam_cv_OBJECTS = \
"CMakeFiles/raspicam_cv.dir/raspicam_cv.cpp.o" \
"CMakeFiles/raspicam_cv.dir/raspicam_still_cv.cpp.o"

# External object files for target raspicam_cv
raspicam_cv_EXTERNAL_OBJECTS =

src/libraspicam_cv.so: src/CMakeFiles/raspicam_cv.dir/raspicam_cv.cpp.o
src/libraspicam_cv.so: src/CMakeFiles/raspicam_cv.dir/raspicam_still_cv.cpp.o
src/libraspicam_cv.so: src/CMakeFiles/raspicam_cv.dir/build.make
src/libraspicam_cv.so: /opt/vc/lib/libmmal_core.so
src/libraspicam_cv.so: /opt/vc/lib/libmmal_util.so
src/libraspicam_cv.so: /opt/vc/lib/libmmal.so
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_shape.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_stitching.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_superres.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_videostab.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_aruco.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_bgsegm.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_bioinspired.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_ccalib.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_datasets.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_dpm.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_face.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_freetype.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_fuzzy.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_hdf.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_line_descriptor.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_optflow.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_plot.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_reg.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_saliency.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_stereo.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_structured_light.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_surface_matching.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_text.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_ximgproc.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_xobjdetect.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_xphoto.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_video.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_viz.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_phase_unwrapping.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_rgbd.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_calib3d.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_features2d.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_flann.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_objdetect.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_ml.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_highgui.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_photo.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_videoio.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_imgcodecs.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_imgproc.so.3.2.0
src/libraspicam_cv.so: /usr/lib/arm-linux-gnueabihf/libopencv_core.so.3.2.0
src/libraspicam_cv.so: src/CMakeFiles/raspicam_cv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/standalone/src/lib/raspicam-0.1.6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libraspicam_cv.so"
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/raspicam_cv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/raspicam_cv.dir/build: src/libraspicam_cv.so

.PHONY : src/CMakeFiles/raspicam_cv.dir/build

src/CMakeFiles/raspicam_cv.dir/clean:
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build/src && $(CMAKE_COMMAND) -P CMakeFiles/raspicam_cv.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/raspicam_cv.dir/clean

src/CMakeFiles/raspicam_cv.dir/depend:
	cd /home/pi/standalone/src/lib/raspicam-0.1.6/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/standalone/src/lib/raspicam-0.1.6 /home/pi/standalone/src/lib/raspicam-0.1.6/src /home/pi/standalone/src/lib/raspicam-0.1.6/build /home/pi/standalone/src/lib/raspicam-0.1.6/build/src /home/pi/standalone/src/lib/raspicam-0.1.6/build/src/CMakeFiles/raspicam_cv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/raspicam_cv.dir/depend

