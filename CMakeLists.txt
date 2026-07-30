# EchoMap CMakeLists.txt
#
# Author: Oliver Dixon
# Date: 2026-06-05

cmake_minimum_required(VERSION 4.0)
project(EchoMap)

# The DummyTarget provides a target to IntelliJ IDEs to manage. We extract the source vector for the real targets.
add_library(DummyTarget INTERFACE EXCLUDE_FROM_ALL
        src/platform/EchoMap.cpp
        src/platform/EchoMap.hpp
        src/App.cpp
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
        src/errors/IgnoredWarning.cpp
        src/errors/IgnoredWarning.hpp
        src/async/results/WorkerResult.hpp
        src/utility/Colour.hpp
        src/utility/Position.hpp
        src/objects/persistence/JSONDeserialiserHelpers.cpp
        src/objects/persistence/JSONDeserialiserHelpers.hpp
        src/objects/persistence/JSONDeserialiser.hpp
        src/utility/FFTRealComplex.cpp
        src/utility/FFTRealComplex.hpp
        src/services/IRenderInvalidator.hpp
        src/services/RenderHost.cpp
        src/services/RenderHost.hpp
        src/services/PanelHost.cpp
        src/services/PanelHost.hpp
        src/services/ProjectControllerBase.cpp
        src/services/ProjectControllerBase.hpp
        src/services/IProjectFilePicker.hpp
        src/services/IProjectMutationService.hpp
        src/services/IProjectOpenService.hpp
        src/services/IProjectObserveService.hpp
)

if (EMSCRIPTEN OR ECHOMAP_BUILD_DOCUMENTATION)
    # WebAssembly/Emscripten-only translation units.
    target_sources(DummyTarget PRIVATE
            src/platform/EchoMapWeb.cpp
            src/platform/EchoMapWeb.hpp
            src/panels/web/MapSourcesModal.cpp
            src/panels/web/MapSourcesModal.hpp
            src/web/EmscriptenExtra.hpp
            src/objects/web/PartialProject.cpp
            src/objects/web/PartialProject.hpp
            src/objects/persistence/web/JSONPartialDeserialiser.cpp
            src/objects/persistence/web/JSONPartialDeserialiser.hpp
            src/services/web/PartialProjectController.cpp
            src/services/web/PartialProjectController.hpp
            src/services/web/WebProjectFilePicker.cpp
            src/services/web/WebProjectFilePicker.hpp
            src/services/web/VFSPicker.cpp
            src/services/web/VFSPicker.hpp
            src/services/web/IPartialProjectObserveService.hpp
            src/services/web/IPartialProjectCompletionService.hpp
            src/services/web/IVFSRequestService.hpp
    )
endif ()

if (NOT EMSCRIPTEN OR ECHOMAP_BUILD_DOCUMENTATION)
    # Native-only translation units.
    target_sources(DummyTarget PRIVATE
            src/platform/EchoMapNative.cpp
            src/platform/EchoMapNative.hpp
            src/panels/native/FileChooser.cpp
            src/panels/native/FileChooser.hpp
            src/panels/native/FilesystemCombo.cpp
            src/panels/native/FilesystemCombo.hpp
            src/objects/persistence/native/JSONFullDeserialiser.cpp
            src/objects/persistence/native/JSONFullDeserialiser.hpp
            src/services/native/FullProjectController.cpp
            src/services/native/FullProjectController.hpp
            src/services/native/NativeProjectFilePicker.cpp
            src/services/native/NativeProjectFilePicker.hpp
    )
endif ()

get_target_property(ECHOMAP_SOURCES DummyTarget SOURCES)

include(cmake/ApplicationTargets.cmake)
include(cmake/DoxygenTarget.cmake)
