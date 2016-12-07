cmake_minimum_required (VERSION 3.1.0)
project (BetterTClonesArray)

find_package(ROOT REQUIRED)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ ISO Standard")
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()
include_directories(${ROOT_INCLUDE_DIRS})


# generate dictionary for Point
#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
ROOT_GENERATE_DICTIONARY(G__Point Point.h LINKDEF LinkDef.h)

#---Create a shared library with geneated dictionary
add_library(Point SHARED Point.cxx G__Point.cxx)
target_link_libraries(Point ${ROOT_LIBRARIES})

add_executable(Demonstrator Demonstrator.cpp)
target_link_libraries(Demonstrator ${ROOT_LIBRARIES} Point)
