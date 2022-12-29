add_library(tsmuxer STATIC
  H264Utils.cpp
  H264Utils.h
  TsMuxer.cpp
  include/TsMuxer.h
  TsMuxerUtils.cpp
  TsMuxerUtils.h
)
set_target_properties(tsmuxer PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(tsmuxer INTERFACE .)

add_subdirectory(tests)
