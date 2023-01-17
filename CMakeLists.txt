cmake_minimum_required(VERSION 3.12...3.16)

project(xml
        LANGUAGES CXX
        DESCRIPTION "A library to serialize custom classes to and from XML by adding a very minimal amount of code to a class" )

include(AICxxProject)

#==============================================================================
# DEPENDENCIES
#

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

pkg_check_modules(Libxmlpp30 libxml++-3.0 IMPORTED_TARGET)
if (Libxmlpp30_FOUND)
  set(LIBXMLPP PkgConfig::Libxmlpp30)
  set(LIBXMLPP_VERSION 30)
else ()
  pkg_check_modules(Libxmlpp26 libxml++-2.6 IMPORTED_TARGET)
  if (Libxmlpp26_FOUND)
    set(LIBXMLPP PkgConfig::Libxmlpp26)
    set(LIBXMLPP_VERSION 26)
  else ()
    message(FATAL_ERROR "Neither libxml++-3.0 nor libxml++-2.6 could be found!")
  endif ()
endif ()

#==============================================================================
# BUILD PROJECT
#

# This project is an OBJECT-library, used by other git submodules and the main project.
add_library(xml_ObjLib OBJECT)

# Let the code know what version is being used.
target_compile_definitions(xml_ObjLib PUBLIC XMLPP_VERSION=${LIBXMLPP_VERSION})

# Set SOURCE_DIR to the current directory.
set_property(
    TARGET     xml_ObjLib
    PROPERTY   SOURCE_DIR       "${CMAKE_CURRENT_SOURCE_DIR}"
)

# The list of source files.
target_sources(xml_ObjLib
    PRIVATE
        "Bridge.cxx"
        "escape.cxx"
        "ReadBridge.cxx"
        "Reader.cxx"
        "read_from_stream.cxx"
        "read_from_string.cxx"
        "SetLocale.cxx"
        "WriteBridge.cxx"
        "Writer.cxx"
        "write_to_stream.cxx"

        "Bridge.h"
        "escape.h"
        "ReadBridge.h"
        "Reader.h"
        "read_from_stream.h"
        "read_from_string.h"
        "set_locale_for.h"
        "SetLocale.h"
        "WriteBridge.h"
        "Writer.h"
        "write_to_stream.h"
        "write_to_string.h"
)

# Required include search-paths.
get_target_property(CWDS_INTERFACE_INCLUDE_DIRECTORIES AICxx::cwds INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(xml_ObjLib
    PUBLIC
        "${CWDS_INTERFACE_INCLUDE_DIRECTORIES}"     # For sys.h and debug.h.
        "${CMAKE_CURRENT_BINARY_DIR}"               # For generated files like config.h.
)

# Require support for C++17.
target_compile_features(xml_ObjLib
    PUBLIC cxx_std_17
)

# Set link dependencies.
target_link_libraries(xml_ObjLib
    PRIVATE
        Threads::Threads
    PUBLIC
        Boost::system
        Boost::filesystem
        ${LIBXMLPP}
)

if (OptionEnableLibcwd)
  target_link_libraries(xml_ObjLib
      PUBLIC
          ${libcwd_r_TARGET}
)
endif()

# Create an ALIAS target.
add_library(AICxx::xml ALIAS xml_ObjLib)

# Prepend this object library to list.
set(AICXX_OBJECTS_LIST AICxx::xml ${AICXX_OBJECTS_LIST} CACHE INTERNAL "List of OBJECT libaries that this project uses.")

# Testsuite.

add_executable(catalog_test EXCLUDE_FROM_ALL)
target_sources(catalog_test
    PRIVATE
        "catalog_test.cxx"
)
target_link_libraries(catalog_test PRIVATE ${AICXX_OBJECTS_LIST})

add_executable(example_test EXCLUDE_FROM_ALL)
target_sources(example_test
    PRIVATE
        "example_test.cxx"
)
target_link_libraries(example_test PRIVATE ${AICXX_OBJECTS_LIST})
