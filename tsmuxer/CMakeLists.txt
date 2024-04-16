set(TSMUXER_SOURCE
  H264Utils.cpp
  H264Utils.h
  TsMuxer.cpp
  include/TsMuxer.h
  TsMuxerUtils.cpp
  TsMuxerUtils.h
)

add_library(tsmuxer STATIC
  ${TSMUXER_SOURCE}
)
target_include_directories(tsmuxer PUBLIC
  ${CMAKE_SOURCE_DIR}/tsmuxer
  "$<TARGET_FILE_DIR:tsmuxer>"
)

add_library(tsmuxer_shared SHARED
  ${TSMUXER_SOURCE}
)
target_include_directories(tsmuxer_shared PUBLIC
  ${CMAKE_SOURCE_DIR}/tsmuxer
  "$<TARGET_FILE_DIR:tsmuxer_shared>"
)

if(PROJECT_IS_TOP_LEVEL)
  add_subdirectory(tests)
endif()
