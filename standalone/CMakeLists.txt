cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(XNetworkStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 2.2.1
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

CPMAddPackage(NAME XNetwork SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

find_package(Boost REQUIRED COMPONENTS coroutine context system)
if(Boost_FOUND)
  message(STATUS "Found boost: ${Boost_INCLUDE_DIRS}")
  include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(XNetworkStandalone ${sources})

set_target_properties(XNetworkStandalone PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "XNetwork")

target_link_libraries(XNetworkStandalone XNetwork::XNetwork cxxopts ${Boost_LIBRARIES})
