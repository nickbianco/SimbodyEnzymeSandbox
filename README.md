# SimbodyEnzymeSandbox

A sandbox for testing the Enzyme AD framework on code where Simbody is a dependent library.

## Dev Container

To use the version of LLVM Clang installed in the container, update the `cmake-tools-kits.json` with the following entry. You may need to close and reopen the container for the changes to take effect.

    {
        "name": "LLVM 18 Clang",
        "compilers": {
          "C": "/opt/llvm-project/build/bin/clang",
          "CXX": "/opt/llvm-project/build/bin/clang++"
        },
        "isTrusted": true
    }

Mark dependency directories as safe

    git config --global --add safe.directory ${containerWorkspaceFolder}/dependencies/Enzyme 
    git config --global --add safe.directory ${containerWorkspaceFolder}/dependencies/simbody

Enabling the LLD linker

    sudo cp /usr/bin/ld /usr/bin/ld.bak
    sudo ln -sf /opt/llvm-project/build/bin/ld.lld /usr/bin/ld

Set the LLVM_DIR CMake variable in `settings.json`.

    "cmake.configureSettings": {
        "LLVM_DIR": "/opt/llvm-project/build/lib/cmake/llvm",
    }

