# Generated by CMake

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.5)
   message(FATAL_ERROR "CMake >= 2.6.0 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.6)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_targetsDefined)
set(_targetsNotDefined)
set(_expectedTargets)
foreach(_expectedTarget rabbitmq::rabbitmq rabbitmq::rabbitmq-static)
  list(APPEND _expectedTargets ${_expectedTarget})
  if(NOT TARGET ${_expectedTarget})
    list(APPEND _targetsNotDefined ${_expectedTarget})
  endif()
  if(TARGET ${_expectedTarget})
    list(APPEND _targetsDefined ${_expectedTarget})
  endif()
endforeach()
if("${_targetsDefined}" STREQUAL "${_expectedTargets}")
  unset(_targetsDefined)
  unset(_targetsNotDefined)
  unset(_expectedTargets)
  set(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT "${_targetsDefined}" STREQUAL "")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_targetsDefined}\nTargets not yet defined: ${_targetsNotDefined}\n")
endif()
unset(_targetsDefined)
unset(_targetsNotDefined)
unset(_expectedTargets)


# Create imported target rabbitmq::rabbitmq
add_library(rabbitmq::rabbitmq SHARED IMPORTED)

set_target_properties(rabbitmq::rabbitmq PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "/home/kishore/S3Server/amqpcpp/rabbitmq-c/build/librabbitmq/../include;/home/kishore/S3Server/amqpcpp/rabbitmq-c/librabbitmq/../include"
)

# Create imported target rabbitmq::rabbitmq-static
add_library(rabbitmq::rabbitmq-static STATIC IMPORTED)

set_target_properties(rabbitmq::rabbitmq-static PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "AMQP_STATIC"
  INTERFACE_INCLUDE_DIRECTORIES "/home/kishore/S3Server/amqpcpp/rabbitmq-c/build/librabbitmq/../include;/home/kishore/S3Server/amqpcpp/rabbitmq-c/librabbitmq/../include"
  INTERFACE_LINK_LIBRARIES "\$<LINK_ONLY:OpenSSL::SSL>;\$<LINK_ONLY:rt>;\$<LINK_ONLY:-pthread>"
)

# Import target "rabbitmq::rabbitmq" for configuration "Release"
set_property(TARGET rabbitmq::rabbitmq APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(rabbitmq::rabbitmq PROPERTIES
  IMPORTED_LOCATION_RELEASE "/home/kishore/S3Server/amqpcpp/rabbitmq-c/build/librabbitmq/librabbitmq.so.0.14.0"
  IMPORTED_SONAME_RELEASE "librabbitmq.so.4"
  )

# Import target "rabbitmq::rabbitmq-static" for configuration "Release"
set_property(TARGET rabbitmq::rabbitmq-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(rabbitmq::rabbitmq-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "/home/kishore/S3Server/amqpcpp/rabbitmq-c/build/librabbitmq/librabbitmq.a"
  )

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
