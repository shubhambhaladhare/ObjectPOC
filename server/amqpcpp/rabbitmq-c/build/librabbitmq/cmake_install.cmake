# Install script for directory: /home/kishore/S3Server/amqpcpp/rabbitmq-c/librabbitmq

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xrabbitmq-c-runtimex" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librabbitmq.so.0.14.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librabbitmq.so.4"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "\$ORIGIN")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/kishore/S3Server/amqpcpp/rabbitmq-c/build/librabbitmq/librabbitmq.so.0.14.0"
    "/home/kishore/S3Server/amqpcpp/rabbitmq-c/build/librabbitmq/librabbitmq.so.4"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librabbitmq.so.0.14.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librabbitmq.so.4"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH ":::::::"
           NEW_RPATH "\$ORIGIN")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xruntime-c-developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librabbitmq.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librabbitmq.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librabbitmq.so"
         RPATH "\$ORIGIN")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/kishore/S3Server/amqpcpp/rabbitmq-c/build/librabbitmq/librabbitmq.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librabbitmq.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librabbitmq.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librabbitmq.so"
         OLD_RPATH ":::::::"
         NEW_RPATH "\$ORIGIN")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librabbitmq.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xrabbitmq-c-developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/kishore/S3Server/amqpcpp/rabbitmq-c/build/librabbitmq/librabbitmq.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xrabbitmq-c-developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/kishore/S3Server/amqpcpp/rabbitmq-c/librabbitmq/../include/amqp.h"
    "/home/kishore/S3Server/amqpcpp/rabbitmq-c/librabbitmq/../include/amqp_framing.h"
    "/home/kishore/S3Server/amqpcpp/rabbitmq-c/librabbitmq/../include/amqp_tcp_socket.h"
    "/home/kishore/S3Server/amqpcpp/rabbitmq-c/librabbitmq/../include/amqp_ssl_socket.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xrabbitmq-c-developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/rabbitmq-c" TYPE FILE FILES
    "/home/kishore/S3Server/amqpcpp/rabbitmq-c/librabbitmq/../include/rabbitmq-c/amqp.h"
    "/home/kishore/S3Server/amqpcpp/rabbitmq-c/librabbitmq/../include/rabbitmq-c/framing.h"
    "/home/kishore/S3Server/amqpcpp/rabbitmq-c/librabbitmq/../include/rabbitmq-c/tcp_socket.h"
    "/home/kishore/S3Server/amqpcpp/rabbitmq-c/librabbitmq/../include/rabbitmq-c/ssl_socket.h"
    "/home/kishore/S3Server/amqpcpp/rabbitmq-c/build/librabbitmq/../include/rabbitmq-c/export.h"
    )
endif()

