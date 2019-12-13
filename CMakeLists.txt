cmake_minimum_required( VERSION 3.12...3.16 )

project( xml
         LANGUAGES CXX
         DESCRIPTION "A library to serialize custom classes to and from XML by adding a very minimal amount of code to a class" )

include(${project_root}/cwm4/cmake/AICxxProject.cmake)

#==============================================================================
# DEPENDENCIES
#

find_package( Boost REQUIRED COMPONENTS system filesystem )
pkg_check_modules( Libxmlpp26 libxml++-2.6 IMPORTED_TARGET )

#==============================================================================
# BUILD PROJECT
#

# This project is an OBJECT-library, used by other git submodules and the main project.
add_library( xml_ObjLib OBJECT )

# Set SOURCE_DIR to the current directory.
set_property(
    TARGET     xml_ObjLib
    PROPERTY   SOURCE_DIR       "${CMAKE_CURRENT_SOURCE_DIR}"
)

# The list of source files.
target_sources( xml_ObjLib
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
target_include_directories( xml_ObjLib
    PUBLIC
        "${CWDS_INTERFACE_INCLUDE_DIRECTORIES}" # For sys.h and debug.h.
        "${CMAKE_CURRENT_BINARY_DIR}"           # For generated files like config.h.
)

# Require support for C++17.
target_compile_features( xml_ObjLib
    PUBLIC cxx_std_17
)

# Set link dependencies.
target_link_libraries( xml_ObjLib
    PRIVATE
        Threads::Threads
    PUBLIC
        Boost::system
        Boost::filesystem
        PkgConfig::Libxmlpp26
)

if ( OptionEnableLibcwd )
  target_link_libraries( xml_ObjLib
      PUBLIC
          PkgConfig::Libcwd_r 
)
endif()

# Create an ALIAS target.
add_library( AICxx::xml ALIAS xml_ObjLib )

# Prepend this object library to list.
set( AICXX_OBJECTS_LIST AICxx::xml ${AICXX_OBJECTS_LIST} CACHE INTERNAL "List of OBJECT libaries that this project uses." )

# Testsuite.

add_executable( catalog_test )
target_sources( catalog_test
    PRIVATE
        "catalog_test.cxx"
)
target_link_libraries( catalog_test PRIVATE ${AICXX_OBJECTS_LIST} )

add_executable( example_test )
target_sources( example_test
    PRIVATE
        "example_test.cxx"
)
target_link_libraries( example_test PRIVATE ${AICXX_OBJECTS_LIST} )
