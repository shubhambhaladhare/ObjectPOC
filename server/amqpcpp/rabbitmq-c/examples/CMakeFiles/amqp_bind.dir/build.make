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
include CMakeFiles/amqp_bind.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/amqp_bind.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/amqp_bind.dir/flags.make

CMakeFiles/amqp_bind.dir/amqp_bind.o: CMakeFiles/amqp_bind.dir/flags.make
CMakeFiles/amqp_bind.dir/amqp_bind.o: amqp_bind.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/amqp_bind.dir/amqp_bind.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/amqp_bind.dir/amqp_bind.o   -c /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/amqp_bind.c

CMakeFiles/amqp_bind.dir/amqp_bind.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/amqp_bind.dir/amqp_bind.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/amqp_bind.c > CMakeFiles/amqp_bind.dir/amqp_bind.i

CMakeFiles/amqp_bind.dir/amqp_bind.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/amqp_bind.dir/amqp_bind.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/amqp_bind.c -o CMakeFiles/amqp_bind.dir/amqp_bind.s

# Object files for target amqp_bind
amqp_bind_OBJECTS = \
"CMakeFiles/amqp_bind.dir/amqp_bind.o"

# External object files for target amqp_bind
amqp_bind_EXTERNAL_OBJECTS = \
"/home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles/examples-common.dir/utils.o" \
"/home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles/examples-common.dir/unix/platform_utils.o"

amqp_bind: CMakeFiles/amqp_bind.dir/amqp_bind.o
amqp_bind: CMakeFiles/examples-common.dir/utils.o
amqp_bind: CMakeFiles/examples-common.dir/unix/platform_utils.o
amqp_bind: CMakeFiles/amqp_bind.dir/build.make
amqp_bind: CMakeFiles/amqp_bind.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable amqp_bind"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/amqp_bind.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/amqp_bind.dir/build: amqp_bind

.PHONY : CMakeFiles/amqp_bind.dir/build

CMakeFiles/amqp_bind.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/amqp_bind.dir/cmake_clean.cmake
.PHONY : CMakeFiles/amqp_bind.dir/clean

CMakeFiles/amqp_bind.dir/depend:
	cd /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles/amqp_bind.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/amqp_bind.dir/depend

