# ---- Dependencies ----

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 3.0.0
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME}Standalone ${sources})

set_target_properties(
  ${PROJECT_NAME}Standalone PROPERTIES CXX_STANDARD 20 OUTPUT_NAME ${PROJECT_NAME}
)

target_link_libraries(
  ${PROJECT_NAME}Standalone ${PROJECT_NAME}::${PROJECT_NAME} cxxopts::cxxopts ${SPECIFIC_LIBS}
)
