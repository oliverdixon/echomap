#
# Interface target for EchoMap to encode common sources and build options.
#
# Author: Oliver Dixon
# Date: 2026-08-01
#

target_compile_features(EchoMapInterface INTERFACE cxx_std_23)

if (ECHOMAP_BUILD_APPLICATION OR ECHOMAP_BUILD_TESTS)
    # If an executable will be built downstream, validate dependencies and setup compile/link options that can be
    # inherited by the executable targets.

    find_package(Dawn CONFIG REQUIRED)
    find_package(ImGui REQUIRED)
    find_package(simdjson REQUIRED)
    find_package(ImPlot REQUIRED)
    find_package(ImPlot3D REQUIRED)
    find_package(FFTW3f CONFIG REQUIRED)
    find_package(sigc++-3 CONFIG REQUIRED)

    target_link_libraries(EchoMapInterface INTERFACE
            imgui::imgui
            implot::implot
            implot3d::implot3d
            simdjson::simdjson
            FFTW3::fftw3f
            sigc-3.0
    )

    if (EMSCRIPTEN)
        if (TARGET dawn_glfw)
            target_compile_definitions(dawn_glfw PRIVATE EMSCRIPTEN=1)
        endif ()

        target_compile_options(EchoMapInterface INTERFACE
                "-fwasm-exceptions"
                "-pthread"
                $<$<CONFIG:Debug,RelWithDebInfo>:-g>
        )

        target_link_options(EchoMapInterface INTERFACE
                "-sALLOW_MEMORY_GROWTH=1"
                "-fwasm-exceptions"
                "-pthread"
        )
    else ()
        find_package(glfw3 REQUIRED)
        target_link_libraries(EchoMapInterface INTERFACE
                dawn::webgpu_dawn
                glfw
        )
    endif ()
endif ()
