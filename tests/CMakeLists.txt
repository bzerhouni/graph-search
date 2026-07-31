include(CTest)

file(GLOB_RECURSE GS_TEST_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/unit/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/integ/*.cpp")

if(GS_TEST_SOURCES)
  add_executable(graph_search_tests ${GS_TEST_SOURCES})

  target_include_directories(graph_search_tests
                             PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
  target_link_libraries(graph_search_tests PRIVATE graph_search
                                                   Boost::unit_test_framework)

  add_test(NAME graph_search_tests COMMAND graph_search_tests)
endif()
