# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/prahlad/Sudoku_Solver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/prahlad/Sudoku_Solver

# Include any dependencies generated for this target.
include CMakeFiles/sudoku_former.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sudoku_former.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sudoku_former.dir/flags.make

CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o: CMakeFiles/sudoku_former.dir/flags.make
CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o: sudoku_former.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/prahlad/Sudoku_Solver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o -c /home/prahlad/Sudoku_Solver/sudoku_former.cpp

CMakeFiles/sudoku_former.dir/sudoku_former.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sudoku_former.dir/sudoku_former.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/prahlad/Sudoku_Solver/sudoku_former.cpp > CMakeFiles/sudoku_former.dir/sudoku_former.cpp.i

CMakeFiles/sudoku_former.dir/sudoku_former.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sudoku_former.dir/sudoku_former.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/prahlad/Sudoku_Solver/sudoku_former.cpp -o CMakeFiles/sudoku_former.dir/sudoku_former.cpp.s

CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o.requires:

.PHONY : CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o.requires

CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o.provides: CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o.requires
	$(MAKE) -f CMakeFiles/sudoku_former.dir/build.make CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o.provides.build
.PHONY : CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o.provides

CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o.provides.build: CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o


# Object files for target sudoku_former
sudoku_former_OBJECTS = \
"CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o"

# External object files for target sudoku_former
sudoku_former_EXTERNAL_OBJECTS =

sudoku_former: CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o
sudoku_former: CMakeFiles/sudoku_former.dir/build.make
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_shape.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_aruco.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_bgsegm.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_bioinspired.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_ccalib.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_datasets.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_dpm.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_face.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_freetype.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_fuzzy.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_hdf.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_line_descriptor.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_optflow.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_plot.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_reg.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_saliency.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_stereo.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_structured_light.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_surface_matching.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_text.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_ximgproc.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_xobjdetect.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_xphoto.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_video.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_viz.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_phase_unwrapping.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_rgbd.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_videoio.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_imgcodecs.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.3.2.0
sudoku_former: /usr/lib/x86_64-linux-gnu/libopencv_core.so.3.2.0
sudoku_former: CMakeFiles/sudoku_former.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/prahlad/Sudoku_Solver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sudoku_former"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sudoku_former.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sudoku_former.dir/build: sudoku_former

.PHONY : CMakeFiles/sudoku_former.dir/build

CMakeFiles/sudoku_former.dir/requires: CMakeFiles/sudoku_former.dir/sudoku_former.cpp.o.requires

.PHONY : CMakeFiles/sudoku_former.dir/requires

CMakeFiles/sudoku_former.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sudoku_former.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sudoku_former.dir/clean

CMakeFiles/sudoku_former.dir/depend:
	cd /home/prahlad/Sudoku_Solver && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/prahlad/Sudoku_Solver /home/prahlad/Sudoku_Solver /home/prahlad/Sudoku_Solver /home/prahlad/Sudoku_Solver /home/prahlad/Sudoku_Solver/CMakeFiles/sudoku_former.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sudoku_former.dir/depend
