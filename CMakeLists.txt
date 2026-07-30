cmake_minimum_required(VERSION 3.10)

if(DEFINED ENV{GS_PROJECT_VERSION})
    set(GS_PROJECT_VERSION "$ENV{GS_PROJECT_VERSION}")
else()
    set(GS_PROJECT_VERSION "0.0.0")   # fallback for local builds
endif()

project(graph_search VERSION ${GS_PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

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

if(BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
    add_subdirectory(test)
endif()
