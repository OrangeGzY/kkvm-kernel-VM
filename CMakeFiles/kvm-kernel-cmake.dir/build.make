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
CMAKE_SOURCE_DIR = /root/kkvm-kernel-VM

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/kkvm-kernel-VM

# Include any dependencies generated for this target.
include CMakeFiles/kvm-kernel-cmake.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/kvm-kernel-cmake.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kvm-kernel-cmake.dir/flags.make

CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o: CMakeFiles/kvm-kernel-cmake.dir/flags.make
CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o: kvm-kernel.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/kkvm-kernel-VM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o   -c /root/kkvm-kernel-VM/kvm-kernel.c

CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/kkvm-kernel-VM/kvm-kernel.c > CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.i

CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/kkvm-kernel-VM/kvm-kernel.c -o CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.s

CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o.requires:

.PHONY : CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o.requires

CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o.provides: CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o.requires
	$(MAKE) -f CMakeFiles/kvm-kernel-cmake.dir/build.make CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o.provides.build
.PHONY : CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o.provides

CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o.provides.build: CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o


# Object files for target kvm-kernel-cmake
kvm__kernel__cmake_OBJECTS = \
"CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o"

# External object files for target kvm-kernel-cmake
kvm__kernel__cmake_EXTERNAL_OBJECTS =

kvm-kernel-cmake: CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o
kvm-kernel-cmake: CMakeFiles/kvm-kernel-cmake.dir/build.make
kvm-kernel-cmake: CMakeFiles/kvm-kernel-cmake.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/kkvm-kernel-VM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable kvm-kernel-cmake"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kvm-kernel-cmake.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kvm-kernel-cmake.dir/build: kvm-kernel-cmake

.PHONY : CMakeFiles/kvm-kernel-cmake.dir/build

CMakeFiles/kvm-kernel-cmake.dir/requires: CMakeFiles/kvm-kernel-cmake.dir/kvm-kernel.c.o.requires

.PHONY : CMakeFiles/kvm-kernel-cmake.dir/requires

CMakeFiles/kvm-kernel-cmake.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kvm-kernel-cmake.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kvm-kernel-cmake.dir/clean

CMakeFiles/kvm-kernel-cmake.dir/depend:
	cd /root/kkvm-kernel-VM && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/kkvm-kernel-VM /root/kkvm-kernel-VM /root/kkvm-kernel-VM /root/kkvm-kernel-VM /root/kkvm-kernel-VM/CMakeFiles/kvm-kernel-cmake.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kvm-kernel-cmake.dir/depend
