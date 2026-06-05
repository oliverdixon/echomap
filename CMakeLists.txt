# WebCFD CMakeLists.txt
#
# Author: Oliver Dixon
# Date: 2026-06-05

cmake_minimum_required(VERSION 4.0)
project(WebCFD)

set(CMAKE_CXX_STANDARD 23)

# Dawn and ImGui submodules configuration
set(DAWN_FETCH_DEPENDENCIES ON)
add_subdirectory(dawn EXCLUDE_FROM_ALL)
include(cmake/ImGui.cmake)
if(EMSCRIPTEN AND TARGET dawn_glfw)
    target_compile_definitions(dawn_glfw PRIVATE EMSCRIPTEN=1)
endif()

# WebCFD common configuration
add_executable(WebCFD
        src/WebCFD.cpp
        src/WebCFD.hpp
        src/App.cpp
        src/IPanel.hpp
        src/ParametersPanel.cpp
        src/ParametersPanel.hpp
        src/RenderPanel.cpp
        src/RenderPanel.hpp
        src/ViewportRenderer.cpp
        src/ViewportRenderer.hpp
)

target_link_libraries(WebCFD PRIVATE webgpu_glfw imgui)

if (EMSCRIPTEN)
    # If configuring for an Emscripten/WebAssembly target, we need targets for the HTML shell and any extra web-side
    # configuration files such as stylesheets and additional JavaScript. Only the shell is a link dependency for
    # Emscripten. Auxiliary web-side files are copied into the output directory when changed.
    set(WEB_SOURCES_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src/web/")
    set(WEB_SHELL "${WEB_SOURCES_ROOT}/shell.html")
    set(WEB_EXTRA "${WEB_SOURCES_ROOT}/shell.css")

    target_sources(WebCFD PRIVATE
            "${WEB_SHELL}"
            "${WEB_EXTRA}"
    )

    set_source_files_properties(
            "${WEB_SHELL}"
            "${WEB_EXTRA}"
            PROPERTIES HEADER_FILE_ONLY ON
    )

    set_property(TARGET WebCFD APPEND PROPERTY
            LINK_DEPENDS "${WEB_SHELL}")

    set_target_properties(WebCFD PROPERTIES SUFFIX ".html")
    target_link_libraries(WebCFD PRIVATE emdawnwebgpu_cpp)
    target_link_options(WebCFD PRIVATE
            "-sASYNCIFY=1"
            "-sUSE_GLFW=3"
            "--shell-file=${WEB_SHELL}"
    )

    # Define a target for each additional auxiliary file specified by WEB_EXTRA to be replicated in the WebCFD target
    # output directory.
    set(WEB_EXTRA_OUTPUTS)

    foreach(asset IN LISTS WEB_EXTRA)
        file(RELATIVE_PATH asset_relative "${WEB_SOURCES_ROOT}" "${asset}")
        set(asset_output "${CMAKE_CURRENT_BINARY_DIR}/${asset_relative}")

        add_custom_command(
                OUTPUT "${asset_output}"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${asset}" "${asset_output}"
                DEPENDS "${asset}"
                VERBATIM
        )

        list(APPEND WEB_EXTRA_OUTPUTS "${asset_output}")
    endforeach()

    add_custom_target(WebCFDShellExtra DEPENDS ${WEB_EXTRA_OUTPUTS})
    add_dependencies(WebCFD WebCFDShellExtra)
else()
    target_link_libraries(WebCFD PRIVATE dawn::webgpu_dawn glfw)
endif()
