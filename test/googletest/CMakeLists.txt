cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_COMPILER g++ CACHE STRING "C++ compiler" FORCE)
project(ft_malloc)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(DGTEST_CREATE_SHARED_LIBRARY 1)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(ft_printf ../../ft_printf)
include_directories(includes ../../includes)

add_executable(test_malloc
  test_malloc.cc
  test_realloc.cc
)

target_link_libraries(test_malloc
  GTest::gtest_main
  ${CMAKE_DL_LIBS}
)

include(GoogleTest)
gtest_discover_tests(test_malloc)
