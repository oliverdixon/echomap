# EchoMap CMakeLists.txt
#
# Author: Oliver Dixon
# Date: 2026-06-05

cmake_minimum_required(VERSION 4.0)
project(EchoMap)

include(CTest)

option(ECHOMAP_BUILD_APPLICATION "Make available the standard EchoMap application targets" ON)
option(ECHOMAP_BUILD_DOCUMENTATION "Build EchoMap Doxygen documentation" OFF)
option(ECHOMAP_BUILD_TESTS "Build EchoMap tests" ${BUILD_TESTING})

# Prepare the common interface target and extract its source vector.
include(cmake/InterfaceTarget.cmake)
get_target_property(ECHOMAP_SOURCES EchoMapInterface SOURCES)

# Include handlers for each enabled aspect of the build.

if (ECHOMAP_BUILD_APPLICATION)
    include(cmake/ApplicationTargets.cmake)
endif ()

if (ECHOMAP_BUILD_TESTS)
    include(cmake/DoctestTargets.cmake)
endif ()

if (ECHOMAP_BUILD_DOCUMENTATION)
    include(cmake/DoxygenTarget.cmake)
endif ()
