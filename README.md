# EchoMap
## Summary
TODO: detailed description.

## Build Instructions
1. Produce a `CMakeUserPresets.json` in the repository root to indicate the absolute location of the Emscripten SDK and
`vcpkg` install directory to CMake. An example can be found in
[CMakeUserPresets_SAMPLE.json](https://github.com/oliverdixon/echomap/blob/master/CMakeUserPresets_SAMPLE.json).

2. Build the project with the native (Dawn) WebGPU backend, or compile with Emscripten to generate the Web Assembly
bundle, using the CMake presets `native-debug-local` and `wasm-debug-local` respectively. Run configurations for
JetBrains IDEs with CMake support are included in `/.idea`.

The intended inheritance hierarchy of CMake profiles looks like the following:
```mermaid
%%{init: {
  "theme": "base",
  "themeVariables": {
    "clusterBkg": "#f8fbff",
    "clusterBorder": "#ccd8f0",
    "titleColor": "#333333"
  }
}}%%
flowchart LR
    default["default"]

    subgraph platformPresets["Platform"]
        direction TB
        native["native"]
        wasm["wasm"]
    end

    subgraph buildTypePresets["Build"]
        direction TB
        debug["debug"]
        release["release"]
    end

    subgraph concretePresets["Concrete"]
        direction TB
        native_debug["native-debug"]
        native_release["native-release"]
        wasm_debug["wasm-debug"]
        wasm_release["wasm-release"]
    end

    subgraph localEnvPresets["Environment"]
        direction TB
        vcpkg_local["vcpkg-local"]
        wasm_local["wasm-local"]
    end

    subgraph localPresets["Local"]
        direction TB
        native_debug_local["native-debug-local"]
        native_release_local["native-release-local"]
        wasm_debug_local["wasm-debug-local"]
        wasm_release_local["wasm-release-local"]
    end

    %% Root/platform inheritance
    default --> native
    default --> wasm

    %% Platform inheritance into concrete presets
    native --> native_debug
    native --> native_release
    wasm --> wasm_debug
    wasm --> wasm_release

    %% Build-type inheritance into concrete presets
    debug --> native_debug
    debug --> wasm_debug
    release --> native_release
    release --> wasm_release

    %% Concrete inheritance into usable local presets
    native_debug --> native_debug_local
    native_release --> native_release_local
    wasm_debug --> wasm_debug_local
    wasm_release --> wasm_release_local

    %% Environment inheritance into usable local presets
    vcpkg_local --> native_debug_local
    vcpkg_local --> native_release_local
    vcpkg_local --> wasm_debug_local
    vcpkg_local --> wasm_release_local

    wasm_local --> wasm_debug_local
    wasm_local --> wasm_release_local

    %% Styling
    classDef project fill:#e8f1ff,stroke:#3366cc,stroke-width:1.5px,color:#111;
    classDef local fill:#eeeeee88,stroke:#99999988,stroke-width:1.5px,color:#666666;

    class default,native,wasm,debug,release,native_debug,native_release,wasm_debug,wasm_release project;
    class vcpkg_local,wasm_local,native_debug_local,native_release_local,wasm_debug_local,wasm_release_local local;
```
