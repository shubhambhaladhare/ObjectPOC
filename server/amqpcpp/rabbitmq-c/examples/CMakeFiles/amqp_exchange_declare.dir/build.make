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
include CMakeFiles/amqp_exchange_declare.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/amqp_exchange_declare.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/amqp_exchange_declare.dir/flags.make

CMakeFiles/amqp_exchange_declare.dir/amqp_exchange_declare.o: CMakeFiles/amqp_exchange_declare.dir/flags.make
CMakeFiles/amqp_exchange_declare.dir/amqp_exchange_declare.o: amqp_exchange_declare.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/amqp_exchange_declare.dir/amqp_exchange_declare.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/amqp_exchange_declare.dir/amqp_exchange_declare.o   -c /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/amqp_exchange_declare.c

CMakeFiles/amqp_exchange_declare.dir/amqp_exchange_declare.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/amqp_exchange_declare.dir/amqp_exchange_declare.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/amqp_exchange_declare.c > CMakeFiles/amqp_exchange_declare.dir/amqp_exchange_declare.i

CMakeFiles/amqp_exchange_declare.dir/amqp_exchange_declare.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/amqp_exchange_declare.dir/amqp_exchange_declare.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/amqp_exchange_declare.c -o CMakeFiles/amqp_exchange_declare.dir/amqp_exchange_declare.s

# Object files for target amqp_exchange_declare
amqp_exchange_declare_OBJECTS = \
"CMakeFiles/amqp_exchange_declare.dir/amqp_exchange_declare.o"

# External object files for target amqp_exchange_declare
amqp_exchange_declare_EXTERNAL_OBJECTS = \
"/home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles/examples-common.dir/utils.o" \
"/home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles/examples-common.dir/unix/platform_utils.o"

amqp_exchange_declare: CMakeFiles/amqp_exchange_declare.dir/amqp_exchange_declare.o
amqp_exchange_declare: CMakeFiles/examples-common.dir/utils.o
amqp_exchange_declare: CMakeFiles/examples-common.dir/unix/platform_utils.o
amqp_exchange_declare: CMakeFiles/amqp_exchange_declare.dir/build.make
amqp_exchange_declare: CMakeFiles/amqp_exchange_declare.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable amqp_exchange_declare"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/amqp_exchange_declare.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/amqp_exchange_declare.dir/build: amqp_exchange_declare

.PHONY : CMakeFiles/amqp_exchange_declare.dir/build

CMakeFiles/amqp_exchange_declare.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/amqp_exchange_declare.dir/cmake_clean.cmake
.PHONY : CMakeFiles/amqp_exchange_declare.dir/clean

CMakeFiles/amqp_exchange_declare.dir/depend:
	cd /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples /home/kishore/S3Server/amqpcpp/rabbitmq-c/examples/CMakeFiles/amqp_exchange_declare.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/amqp_exchange_declare.dir/depend

