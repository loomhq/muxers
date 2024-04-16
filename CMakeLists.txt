cmake_minimum_required(VERSION 3.24)

# Not ideal to use this global variable, but necessary to make sure
# that tooling and projects use the same version
set(CMAKE_CXX_STANDARD 20)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

project(
  muxers
  VERSION 0.0.1
  DESCRIPTION "🧩 muxer, muxer, pants👖 on fire 🔥"
  HOMEPAGE_URL "https://github.com/loomhq/muxers"
  LANGUAGES CXX C
)

if(APPLE)
  # Set this up earlier so dependency are built properly
  set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14)
endif()

if(WIN32)
  add_definitions(-DNOMINMAX)
  add_definitions(-D_WIN32_WINNT=_WIN32_WINNT_WIN10)
endif()

if(PROJECT_IS_TOP_LEVEL)

include(CTest)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP YES
)
if(NOT googletest_POPULATED)
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  if(APPLE)
    set(CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)
  endif()
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

endif()

add_subdirectory(tsmuxer)