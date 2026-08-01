#
# Doctest targets for EchoMap
#
# Author: Oliver Dixon
# Date: 2026-07-31
#

find_package(doctest CONFIG REQUIRED)

add_executable(EchoMapTests
        ${ECHOMAP_SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Doctest.cpp"
)

set_target_properties(EchoMapTests PROPERTIES CXX_SCAN_FOR_MODULES OFF)

target_link_libraries(EchoMapTests
        PRIVATE
        EchoMapInterface
        doctest::doctest
)

if (EMSCRIPTEN)
    target_link_options(EchoMapTests
            PRIVATE
            "-sEXIT_RUNTIME=1"
            "-sENVIRONMENT=node"
    )
endif ()

include("${doctest_DIR}/doctest.cmake")
doctest_discover_tests(EchoMapTests TEST_PREFIX "EchoMap.")
