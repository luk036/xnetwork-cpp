cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(XNetworkTests LANGUAGES CXX)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage("gh:onqtam/doctest#2.4.5")
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.0")

find_package(Boost REQUIRED)
if(Boost_FOUND)
  message(STATUS "Found boost: ${Boost_INCLUDE_DIRS}")
  # add_library(Boost::boost INTERFACE IMPORTED GLOBAL)
  target_include_directories(Boost::boost SYSTEM INTERFACE ${Boost_INCLUDE_DIRS})
  # Disable autolink
  target_compile_definitions(Boost::boost INTERFACE BOOST_ALL_NO_LIB=1)
endif()

include(../cmake/specific.cmake)

if(TEST_INSTALLED_VERSION)
  find_package(XNetwork REQUIRED)
else()
  CPMAddPackage(NAME XNetwork SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_executable(XNetworkTests ${sources})
target_link_libraries(
  XNetworkTests doctest::doctest XNetwork::XNetwork Boost::boost cppcoro::cppcoro
)
set_target_properties(XNetworkTests PROPERTIES CXX_STANDARD 20)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(XNetwork INTERFACE -fcoroutines -Wall -Wpedantic -Wextra -Werror)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(
      XNetwork INTERFACE -fcoroutines-ts -stdlib=libc++ -Wall -Wpedantic -Wextra -Werror
    )
  elseif(MSVC)
    target_compile_options(XNetwork INTERFACE /std:c++latest /await /W4 /WX)
    target_compile_definitions(XNetworkTests PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Add XNetworkTests ----

enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME xnTests COMMAND XNetworkTests)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(XNetworkTests)

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(XNetwork INTERFACE -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(XNetwork INTERFACE -fprofile-arcs -ftest-coverage)
endif()
