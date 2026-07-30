cmake_minimum_required(VERSION 3.10)

if(DEFINED ENV{GS_PROJECT_VERSION})
  set(GS_PROJECT_VERSION "$ENV{GS_PROJECT_VERSION}")
else()
  set(GS_PROJECT_VERSION "0.0.0") # fallback for local builds
endif()

project(
  graph_search
  VERSION ${GS_PROJECT_VERSION}
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(DEFINED BUILD_EXAMPLES)
  # respects a -DBUILD_EXAMPLES=... passed on the cmake command line
elseif(DEFINED ENV{BUILD_EXAMPLES})
  set(BUILD_EXAMPLES "$ENV{BUILD_EXAMPLES}")
else()
  set(BUILD_EXAMPLES OFF)
endif()

if(DEFINED BUILD_TESTS)
  # respects a -DBUILD_TESTS=... passed on the cmake command line
elseif(DEFINED ENV{BUILD_TESTS})
  set(BUILD_TESTS "$ENV{BUILD_TESTS}")
else()
  set(BUILD_TESTS OFF)
endif()

message("graph_search - build examples : ${BUILD_EXAMPLES}")
message("graph_search - build tests : ${BUILD_TESTS}")

# ==================================================================================================
# Dependencies
# ==================================================================================================

include(FetchContent)

# property_tree is used by the GraphML parser; test is only needed to build the
# test suite.
set(BOOST_INCLUDE_LIBRARIES property_tree)
if(${BUILD_TESTS})
  list(APPEND BOOST_INCLUDE_LIBRARIES test)
endif()
set(BOOST_ENABLE_CMAKE ON)

FetchContent_Declare(
  Boost
  URL "https://github.com/boostorg/boost/releases/download/boost-1.91.0-1/boost-1.91.0-1-cmake.tar.xz"
  URL_HASH
    SHA256=cc5dc5006ecbdf0051f90979be31b4eee5987d9ae14ae9fb9c03cfa43fa3cdad
  DOWNLOAD_EXTRACT_TIMESTAMP ON)
FetchContent_MakeAvailable(Boost)

# ==================================================================================================
# Graph search library
# ==================================================================================================

add_library(graph_search)

file(GLOB_RECURSE GS_HEADERS CONFIGURE_DEPENDS "include/*.hpp")
file(GLOB_RECURSE GS_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

target_sources(
  graph_search
  PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES ${GS_HEADERS}
  PRIVATE ${GS_SOURCES})

target_link_libraries(graph_search PUBLIC Boost::property_tree)

# ==================================================================================================
# Examples
# ==================================================================================================

if(${BUILD_EXAMPLES} AND EXISTS
                         "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
  add_subdirectory(examples)
endif()

# ==================================================================================================
# Tests
# ==================================================================================================

if(${BUILD_TESTS} AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
  add_subdirectory(tests)
endif()
