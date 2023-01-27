add_executable(
  tsmuxer_tests
  H264UtilsTests.cpp
  TsMuxerTests.cpp
  TsMuxerUtilsTests.cpp
)
target_include_directories(tsmuxer_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/tsmuxer
)
include(GoogleTest)

target_link_libraries(
  tsmuxer_tests
  tsmuxer
  GTest::gtest_main
)

gtest_discover_tests(tsmuxer_tests)