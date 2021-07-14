cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(XNCppTests LANGUAGES CXX)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage("gh:onqtam/doctest#2.4.5")
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.0")

if(TEST_INSTALLED_VERSION)
  find_package(XNCpp REQUIRED)
else()
  CPMAddPackage(NAME XNCpp SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

find_package (Boost REQUIRED COMPONENTS coroutine context system)
if (Boost_FOUND)
    message(STATUS "Found boost: ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
endif (Boost_FOUND)

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_executable(XNCppTests ${sources})
target_link_libraries(XNCppTests doctest::doctest XNCpp::XNCpp ${Boost_LIBRARIES})
set_target_properties(XNCppTests PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(XNCpp PUBLIC -Wall -Wpedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(XNCpp PUBLIC /W4 /WX)
    target_compile_definitions(XNCppTests PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Add XNCppTests ----

enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME xnTests COMMAND XNCppTests)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(XNCppTests)

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(XNCpp PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(XNCpp PUBLIC -fprofile-arcs -ftest-coverage)
endif()
