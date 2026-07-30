cmake_minimum_required(VERSION 3.10)

if(DEFINED ENV{GS_PROJECT_VERSION})
    set(GS_PROJECT_VERSION "$ENV{GS_PROJECT_VERSION}")
else()
    set(GS_PROJECT_VERSION "0.0.0")   # fallback for local builds
endif()

project(graph_search VERSION ${GS_PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# ======================================================================================================================
#                                             Graph search library
# ======================================================================================================================

add_library(graph_search)

file(GLOB_RECURSE GS_HEADERS CONFIGURE_DEPENDS "include/*.hpp")
file(GLOB_RECURSE GS_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

target_sources(graph_search PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "include"
    FILES
        ${GS_HEADERS}
    PRIVATE
        ${GS_SOURCES}
)

# ======================================================================================================================
#                                                 Examples
# ======================================================================================================================

if(DEFINED ENV{BUILD_EXAMPLES})
    set(BUILD_EXAMPLES "$ENV{BUILD_EXAMPLES}")
else()
    set(BUILD_EXAMPLES OFF)
endif()

if (${BUILD_EXAMPLES} AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
    add_subdirectory(examples)
endif ()

# ======================================================================================================================
#                                                  Tests
# ======================================================================================================================

if(DEFINED ENV{BUILD_TESTS})
    set(BUILD_TESTS "$ENV{BUILD_TESTS}")
else()
    set(BUILD_TESTS OFF)
endif()

if(${BUILD_TESTS} AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
endif()
