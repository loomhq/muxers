add_executable(
  tsmuxer_tests
  H264UtilsTests.cpp
  TsMuxerTests.cpp
  TsMuxerUtilsTests.cpp
)
include(GoogleTest)

target_link_libraries(
  tsmuxer_tests
  tsmuxer
  GTest::gtest_main
)

gtest_discover_tests(tsmuxer_tests)