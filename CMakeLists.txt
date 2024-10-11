cmake_minimum_required(VERSION 3.14)
project(SimbodyTestProject LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enzyme
# ------
# TODO the Enzyme installation target does not include LLDEnzymeLooseTypeFlags, LLDEnzymePrintTypeFlags, etc.
# set(Enzyme_DIR ${CMAKE_SOURCE_DIR}/dependencies/install/Enzyme/lib/cmake/Enzyme)
set(Enzyme_DIR ${CMAKE_SOURCE_DIR}/dependencies/build/Enzyme/build)
find_package(Enzyme REQUIRED CONFIG NO_DEFAULT_PATH)
get_property(importTargetsAfter DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY IMPORTED_TARGETS)
message("imported targets ${importTargetsAfter}")

# Simbody
# -------
# TODO: Building Simbody using ExternalProject_Add is causing Enzyme AD to fail.
# set(Simbody_DIR ${CMAKE_SOURCE_DIR}/dependencies/install/simbody/lib/cmake/simbody)
# set(SIMBODY_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dependencies/install/simbody/include/simbody)
set(Simbody_DIR ${CMAKE_SOURCE_DIR}/dependencies/simbody_install/lib/cmake/simbody)
set(SIMBODY_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dependencies/simbody_install/include/simbody)
find_package(Simbody REQUIRED)

add_subdirectory(src)