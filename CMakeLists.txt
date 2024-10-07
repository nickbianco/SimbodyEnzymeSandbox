cmake_minimum_required(VERSION 3.14)
project(SimbodyTestProject LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Simbody
# -------
set(Simbody_DIR /Users/nbianco/Repos/SimbodyEnzymeSandbox/dependencies/simbody_install/lib/cmake/simbody)
set(SIMBODY_INCLUDE_DIRS /Users/nbianco/Repos/SimbodyEnzymeSandbox/dependencies/simbody_install/include/simbody)
set(SIMBODY_VISUALIZER_NAME simbody-visualizer)
find_package(Simbody REQUIRED)

# Enzyme
# ------
set(Enzyme_DIR /Users/nbianco/repos/Enzyme/enzyme/enzyme_build)
find_package(Enzyme REQUIRED CONFIG NO_DEFAULT_PATH)
get_property(importTargetsAfter DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY IMPORTED_TARGETS)
message("imported targets ${importTargetsAfter}")

# Enable LTO (Link-Time Optimization) for both compiler and linker
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")

# For the linker flags
set(CMAKE_LINKER "/Users/nbianco/repos/llvm-project/build/bin/ld.lld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=/Users/nbianco/repos/llvm-project/build/bin/ld.lld")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=/Users/nbianco/repos/llvm-project/build/bin/ld.lld")


# Add the executable
add_executable(SimbodyEnzymeTest src/main.cpp)
target_compile_definitions(SimbodyEnzymeTest PUBLIC -DNOVERSION)
target_link_libraries(SimbodyEnzymeTest PRIVATE SimTKcommon_static SimTKmath_static SimTKsimbody_static PUBLIC LLDEnzymeFlags)
target_include_directories(SimbodyEnzymeTest PRIVATE ${SIMBODY_INCLUDE_DIRS})