# Port Overlays

As they replicate upstream build processes in the EchoMap repository, `vcpkg` port overlays should be kept to an
absolute minimum. However, in some cases, it is necessary to maintain local overrides for the *Im-* family of libraries'
portfiles due to their reliance on header-based configuration. Use of port overlays is not ideal, but it is preferable
to maintaining submodules of each dependency.

If required port changes do not depend on EchoMap files, they can be contributed to
[oliverdixon/vcpkg](https://github.com/oliverdixon/vcpkg) and ideally sent back
[upstream](https://github.com/microsoft/vcpkg).

## imgui

Required to detain a custom `imconfig.h` compile-time configuration file.
