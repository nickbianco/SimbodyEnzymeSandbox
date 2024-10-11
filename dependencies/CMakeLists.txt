cmake_minimum_required(VERSION 3.5)

project(SimbodyEnzymeSandboxDependencies)

include(ExternalProject)

# Requires an LLVM (15-18) build located at LLVM_DIR.
# https://enzyme.mit.edu/Installation/
ExternalProject_Add(Enzyme
        GIT_REPOSITORY   https://github.com/EnzymeAD/Enzyme.git
        GIT_TAG          v0.0.153
        SOURCE_DIR       ${CMAKE_SOURCE_DIR}/Enzyme
        TMP_DIR          ${CMAKE_BINARY_DIR}/Enzyme/tmp
        STAMP_DIR        ${CMAKE_BINARY_DIR}/Enzyme/stamp
        BINARY_DIR       ${CMAKE_BINARY_DIR}/Enzyme/build
        INSTALL_DIR      ${CMAKE_SOURCE_DIR}/install/Enzyme
        CMAKE_ARGS       -S ${CMAKE_SOURCE_DIR}/Enzyme/enzyme
                         -B ${CMAKE_BINARY_DIR}/Enzyme/build
        CMAKE_CACHE_ARGS -DLLVM_DIR:PATH=${LLVM_DIR}
                         -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
                         -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
                         -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                         -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
)

# Enable link-time optimization. This assumes that your 
# local ld compiler has been symlinked to lld.
# See https://lld.llvm.org/ section "Using LLD".
# Needed to provide -DNDEBUG and -O2 flags manually here,
# Otherwise, Enzyme AD was failing to compile.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto -DNDEBUG -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto -DNDEBUG -O2")
ExternalProject_Add(simbody
        GIT_REPOSITORY   https://github.com/simbody/simbody.git
        GIT_TAG          master
        SOURCE_DIR       ${CMAKE_SOURCE_DIR}/simbody
        TMP_DIR          ${CMAKE_BINARY_DIR}/simbody/tmp
        STAMP_DIR        ${CMAKE_BINARY_DIR}/simbody/stamp
        BINARY_DIR       ${CMAKE_BINARY_DIR}/simbody/build
        INSTALL_DIR      ${CMAKE_SOURCE_DIR}/install/simbody
        CMAKE_CACHE_ARGS 
            -DBUILD_EXAMPLES:BOOL=OFF
            -DBUILD_TESTING:BOOL=OFF
            -DBUILD_STATIC_LIBRARIES:BOOL=ON # LTO requires static libraries.  
            -DBUILD_DYNAMIC_LIBRARIES:BOOL=OFF  
            -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
)
