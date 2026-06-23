# Web**CFD**
## Summary
Web**CFD** is a software package implementing customisable hardware-accelerated Computational Fluid Dynamics simulations
in the browser using the modern WebAssembly and WebGPU standards with an ImGui front-end.

Web**CFD** is a *highly experimental* prototype. It comes with no warranty, or even suggestion of correctness, and
should be considered to be multiple development cycles *prior to alpha*.

## Build Instructions
1. Clone the Dawn, ImGui, ImPlot, and DrLibs submodules into the working tree.

2. Produce a `CMakeUserPresets.json` in the repository root to indicate the absolute location of the Emscripten SDK to
CMake. An example can be found in
[CMakeUserPresets_SAMPLE.json](https://github.com/oliverdixon/webcfd/blob/master/CMakeUserPresets_SAMPLE.json).

3. Build the project with the native (Dawn) WebGPU backend, or compile with Emscripten to generate the Web Assembly
bundle, using the CMake presets `native-debug` and `wasm-debug-local` respectively. Run configurations for JetBrains
IDEs with CMake support are included in `/.idea`.
