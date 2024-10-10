cmake_minimum_required(VERSION 3.14)
project(SimbodyTestProject LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Simbody
# -------
set(Simbody_DIR /home/nbianco/repos/SimbodyEnzymeSandbox/dependencies/simbody_install/lib/cmake/simbody)
set(SIMBODY_INCLUDE_DIRS /home/nbianco/repos/SimbodyEnzymeSandbox/dependencies/simbody_install/include/simbody)
set(SIMBODY_VISUALIZER_NAME simbody-visualizer)
find_package(Simbody REQUIRED)

# Enzyme
# ------
set(Enzyme_DIR /home/nbianco/repos/Enzyme/enzyme/build)
find_package(Enzyme REQUIRED CONFIG NO_DEFAULT_PATH)
get_property(importTargetsAfter DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY IMPORTED_TARGETS)
message("imported targets ${importTargetsAfter}")

add_subdirectory(src)