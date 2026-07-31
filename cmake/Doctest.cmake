#
# Doctest targets for EchoMap
#
# Author: Oliver Dixon
# Date: 2026-07-31
#

if (ECHOMAP_BUILD_TESTS)
    find_package(doctest CONFIG REQUIRED)

    add_executable(EchoMapTests
            ${ECHOMAP_SOURCES}
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Doctest.cpp"
    )

    target_link_libraries(EchoMapTests
            PRIVATE
            DummyTarget
            doctest::doctest
    )

    add_test(
            NAME EchoMapTests
            COMMAND EchoMapTests
    )
endif ()
