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
CMAKE_SOURCE_DIR = /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples

# Include any dependencies generated for this target.
include CMakeFiles/examples-common.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/examples-common.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/examples-common.dir/flags.make

CMakeFiles/examples-common.dir/utils.o: CMakeFiles/examples-common.dir/flags.make
CMakeFiles/examples-common.dir/utils.o: utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/examples-common.dir/utils.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/examples-common.dir/utils.o   -c /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/utils.c

CMakeFiles/examples-common.dir/utils.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/examples-common.dir/utils.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/utils.c > CMakeFiles/examples-common.dir/utils.i

CMakeFiles/examples-common.dir/utils.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/examples-common.dir/utils.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/utils.c -o CMakeFiles/examples-common.dir/utils.s

CMakeFiles/examples-common.dir/unix/platform_utils.o: CMakeFiles/examples-common.dir/flags.make
CMakeFiles/examples-common.dir/unix/platform_utils.o: unix/platform_utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/examples-common.dir/unix/platform_utils.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/examples-common.dir/unix/platform_utils.o   -c /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/unix/platform_utils.c

CMakeFiles/examples-common.dir/unix/platform_utils.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/examples-common.dir/unix/platform_utils.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/unix/platform_utils.c > CMakeFiles/examples-common.dir/unix/platform_utils.i

CMakeFiles/examples-common.dir/unix/platform_utils.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/examples-common.dir/unix/platform_utils.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/unix/platform_utils.c -o CMakeFiles/examples-common.dir/unix/platform_utils.s

examples-common: CMakeFiles/examples-common.dir/utils.o
examples-common: CMakeFiles/examples-common.dir/unix/platform_utils.o
examples-common: CMakeFiles/examples-common.dir/build.make

.PHONY : examples-common

# Rule to build all files generated by this target.
CMakeFiles/examples-common.dir/build: examples-common

.PHONY : CMakeFiles/examples-common.dir/build

CMakeFiles/examples-common.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/examples-common.dir/cmake_clean.cmake
.PHONY : CMakeFiles/examples-common.dir/clean

CMakeFiles/examples-common.dir/depend:
	cd /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles/examples-common.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/examples-common.dir/depend

