# SimbodyEnzymeSandbox

A sandbox for testing the [Enzyme automatic differentiation framework](https://enzyme.mit.edu/) on code where Simbody is a dependent library of the source code (e.g., OpenSim).

## Install LLVM

Enzyme requires [LLVM 15+ with Clang and LLD enabled](https://enzyme.mit.edu/Installation/). See the Dev Container instructions below to configure Clang and LLD.

## Install the dependencies

Install Simbody and Enzyme via `dependencies/CMakeLists.txt`.  

## Dev Container

To use the version of LLVM Clang installed in the container, update the `cmake-tools-kits.json` with the following entry (see `.devcontainer/llvm-18-cmake-kit.json`). You may need to close and reopen the container for the changes to take effect.

    {
        "name": "LLVM 18 Clang",
        "compilers": {
          "C": "/opt/llvm-project/build/bin/clang",
          "CXX": "/opt/llvm-project/build/bin/clang++"
        },
        "isTrusted": true
    }

Set the `LLVM_DIR` CMake variable in `dependencies/.vscode/settings.json`.

    "cmake.configureSettings": {
        "LLVM_DIR": "/opt/llvm-project/build/lib/cmake/llvm",
    }

