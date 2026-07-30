cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

project(XNetwork LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies (all installed by the parent build) ----

include(../cmake/CPM.cmake)

# Find dependencies first — XNetworkConfig.cmake references fmt::fmt and Py2Cpp::Py2Cpp
find_package(fmt REQUIRED)
find_package(Py2Cpp REQUIRED)
find_package(${PROJECT_NAME} REQUIRED)

set(SPECIFIC_LIBS fmt::fmt Py2Cpp::Py2Cpp)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../standalone ${CMAKE_BINARY_DIR}/test_installed)
