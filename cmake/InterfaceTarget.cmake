#
# Interface target for EchoMap to encode common sources and build options.
#
# Author: Oliver Dixon
# Date: 2026-08-01
#

# Platform-independent sources.
add_library(EchoMapInterface INTERFACE EXCLUDE_FROM_ALL
        src/platform/EchoMap.cpp
        src/platform/EchoMap.hpp
        src/panels/IPanel.hpp
        src/RobotoMedium.hpp
        src/utility/Logger.cpp
        src/utility/Logger.hpp
        src/errors/ConfigurationError.cpp
        src/errors/ConfigurationError.hpp
        src/objects/factories/SignalFactory.cpp
        src/objects/factories/SignalFactory.hpp
        src/objects/Signal.cpp
        src/objects/Signal.hpp
        src/objects/Sensor.cpp
        src/objects/Sensor.hpp
        src/panels/ProjectPanel.cpp
        src/panels/ProjectPanel.hpp
        src/panels/MenuPanel.cpp
        src/panels/MenuPanel.hpp
        src/objects/Project.cpp
        src/objects/Project.hpp
        src/objects/BidirectionalUnorderedMapping.hpp
        src/objects/Object.hpp
        src/objects/IDAllocator.hpp
        src/panels/ErrorModal.cpp
        src/panels/ErrorModal.hpp
        src/platform/SurfaceFactory.cpp
        src/platform/SurfaceFactory.hpp
        src/errors/LocatableError.cpp
        src/errors/LocatableError.hpp
        src/objects/persistence/JSONSerialiser.cpp
        src/objects/persistence/JSONSerialiser.hpp
        src/objects/persistence/ISerialiser.hpp
        src/panels/SignalWaveformPanel.cpp
        src/panels/SignalWaveformPanel.hpp
        src/panels/SensorGeometryPanel.cpp
        src/panels/SensorGeometryPanel.hpp
        src/panels/ChannelMappingPanel.cpp
        src/panels/ChannelMappingPanel.hpp
        src/async/ThreadSafeQueue.hpp
        src/async/tasks/ITask.hpp
        src/async/tasks/LoadProjectTask.cpp
        src/async/tasks/LoadProjectTask.hpp
        src/async/Worker.cpp
        src/async/Worker.hpp
        src/async/results/ErrorResult.cpp
        src/async/results/ErrorResult.hpp
        src/async/results/LoadProjectResult.cpp
        src/async/results/LoadProjectResult.hpp
        src/async/tasks/DownsampleTask.cpp
        src/async/tasks/DownsampleTask.hpp
        src/async/results/DownsampleResult.cpp
        src/async/results/DownsampleResult.hpp
        src/objects/FrequencySpectrum.cpp
        src/objects/FrequencySpectrum.hpp
        src/objects/factories/FrequencySpectrumFactory.cpp
        src/objects/factories/FrequencySpectrumFactory.hpp
        src/panels/SignalDFTPanel.cpp
        src/panels/SignalDFTPanel.hpp
        src/async/tasks/DFTTask.cpp
        src/async/tasks/DFTTask.hpp
        src/async/results/DFTResult.cpp
        src/async/results/DFTResult.hpp
        src/async/tasks/ITask.cpp
        src/async/WorkerResultDespatcher.cpp
        src/async/WorkerResultDespatcher.hpp
        src/async/ResultChannel.hpp
        src/objects/factories/WindowFunctions.hpp
        src/objects/factories/WindowFunctions.cpp
        src/utility/VariantHelpers.hpp
        src/async/tasks/LoadSignalFileTask.cpp
        src/async/tasks/LoadSignalFileTask.hpp
        src/async/results/LoadSignalFileResult.cpp
        src/async/results/LoadSignalFileResult.hpp
        src/async/results/WorkerResult.hpp
        src/utility/Colour.hpp
        src/utility/Position.hpp
        src/objects/persistence/JSONDeserialiserHelpers.cpp
        src/objects/persistence/JSONDeserialiserHelpers.hpp
        src/objects/persistence/JSONDeserialiser.hpp
        src/utility/FFTRealComplex.cpp
        src/utility/FFTRealComplex.hpp
        src/services/IRenderInvalidateService.hpp
        src/controllers/RenderHost.cpp
        src/controllers/RenderHost.hpp
        src/controllers/PanelHost.cpp
        src/controllers/PanelHost.hpp
        src/controllers/ProjectControllerBase.cpp
        src/controllers/ProjectControllerBase.hpp
        src/services/IFilePickerService.hpp
        src/services/IProjectMutationService.hpp
        src/services/IProjectOpenService.hpp
        src/services/IProjectObserveService.hpp
)

if (EMSCRIPTEN OR ECHOMAP_BUILD_DOCUMENTATION)
    # WebAssembly/Emscripten-only translation units.
    target_sources(EchoMapInterface PRIVATE
            src/platform/EchoMapWeb.cpp
            src/platform/EchoMapWeb.hpp
            src/panels/web/MapSourcesModal.cpp
            src/panels/web/MapSourcesModal.hpp
            src/web/EmscriptenExtra.hpp
            src/objects/web/PartialProject.cpp
            src/objects/web/PartialProject.hpp
            src/objects/persistence/web/JSONPartialDeserialiser.cpp
            src/objects/persistence/web/JSONPartialDeserialiser.hpp
            src/controllers/web/PartialProjectController.cpp
            src/controllers/web/PartialProjectController.hpp
            src/services/web/WebProjectFilePickerService.cpp
            src/services/web/WebProjectFilePickerService.hpp
            src/services/web/VFSPickerService.cpp
            src/services/web/VFSPickerService.hpp
            src/services/web/IPartialProjectObserveService.hpp
            src/services/web/IPartialProjectCompletionService.hpp
            src/services/web/IPartialProjectBuilderService.hpp
    )
endif ()

if (NOT EMSCRIPTEN OR ECHOMAP_BUILD_DOCUMENTATION)
    # Native-only translation units.
    target_sources(EchoMapInterface PRIVATE
            src/platform/EchoMapNative.cpp
            src/platform/EchoMapNative.hpp
            src/panels/native/FileChooser.cpp
            src/panels/native/FileChooser.hpp
            src/panels/native/FilesystemCombo.cpp
            src/panels/native/FilesystemCombo.hpp
            src/objects/persistence/native/JSONFullDeserialiser.cpp
            src/objects/persistence/native/JSONFullDeserialiser.hpp
            src/controllers/native/FullProjectController.cpp
            src/controllers/native/FullProjectController.hpp
            src/services/native/NativeProjectFilePickerService.cpp
            src/services/native/NativeProjectFilePickerService.hpp
    )
endif ()

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
