cmake_minimum_required(VERSION 3.14...3.22)

project(XNetworkTests LANGUAGES CXX)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)
include(../specific.cmake)

CPMAddPackage("gh:doctest/doctest@2.4.11")
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.3")

if(TEST_INSTALLED_VERSION)
  find_package(XNetwork REQUIRED)
else()
  CPMAddPackage(NAME XNetwork SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} doctest::doctest XNetwork::XNetwork ${SPECIFIC_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(XNetwork INTERFACE -Wall -Wpedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(XNetwork INTERFACE /W4 /WX /wd4819 /wd4996)
    target_compile_definitions(${PROJECT_NAME} INTERFACE DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Add XNetworkTests ----

enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME ${PROJECT_NAME} COMMAND
# ${PROJECT_NAME})

include(../cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_NAME})

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(XNetwork INTERFACE -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(XNetwork INTERFACE -fprofile-arcs -ftest-coverage)
endif()
