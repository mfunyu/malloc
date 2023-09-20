cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(DGTEST_CREATE_SHARED_LIBRARY 1)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/ft_printf)
include_directories(${CMAKE_SOURCE_DIR}/libft)
add_executable(test_malloc
  test/test_malloc.cc
  ft_printf/ft_printf.h
  libft/libft.h
)

add_library(ft STATIC IMPORTED)
set_target_properties(ft PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libft/libft.a"
#  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/libft"
)

add_library(ftprintf STATIC IMPORTED)
set_target_properties(ftprintf PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ft_printf/libftprintf.a"
 # INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/ft_printf"
)

target_link_libraries(test_malloc
  ft
  ftprintf
  GTest::gtest_main
  ${CMAKE_DL_LIBS}
)

include(GoogleTest)
gtest_discover_tests(test_malloc)