cmake_minimum_required(VERSION 3.14...3.22)

project(XNetworkDocs)

# ---- Doxygen variables ----

# Get project version from root CMakeLists.txt
file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/../CMakeLists.txt" _ver REGEX "^  VERSION ")
string(REGEX REPLACE "^  VERSION (.+)" "\\1" _version "${_ver}")
string(STRIP "${_version}" _version)

set(DOXYGEN_PROJECT_NAME XNetwork)
set(DOXYGEN_PROJECT_VERSION "${_version}")
set(DOXYGEN_PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")

configure_file(${CMAKE_CURRENT_LIST_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
add_custom_target(
  GenerateDocs
  COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
  COMMAND echo "Docs written to: ${DOXYGEN_OUTPUT_DIRECTORY}/html"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
else()
  add_custom_target(
    GenerateDocs
    COMMAND ${CMAKE_COMMAND} -E echo "Doxygen not found - skipping documentation generation"
  )
endif()
