cmake_minimum_required(VERSION 3.24)

# Not ideal to use this global variable, but necessary to make sure
# that tooling and projects use the same version
set(CMAKE_CXX_STANDARD 20)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

# Note: by default ENABLE_DEVELOPER_MODE is True
# This means that all analysis (sanitizers, static analysis)
# is enabled and all warnings are treated as errors
# if you want to switch this behavior, change TRUE to FALSE
set(ENABLE_DEVELOPER_MODE
    TRUE
    CACHE BOOL "Enable 'developer mode'")

# Change this to false if you want to disable warnings_as_errors in developer mode
set(OPT_WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT TRUE)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP YES
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

project(
  muxers
  VERSION 0.0.1
  DESCRIPTION "🧩 muxer, muxer, pants👖 on fire 🔥"
  HOMEPAGE_URL "https://github.com/loomhq/muxers"
  LANGUAGES CXX C
)

if(WIN32)
  add_definitions(-DNOMINMAX)
  add_definitions(-D_WIN32_WINNT=_WIN32_WINNT_WIN10)
endif()

enable_testing()
add_subdirectory(tsmuxer)