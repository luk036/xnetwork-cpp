cpmaddpackage(
  NAME
  fmt
  GIT_TAG
  7.1.3
  GITHUB_REPOSITORY
  fmtlib/fmt
  OPTIONS
  "FMT_INSTALL YES" # create an installable target
)

cpmaddpackage(
  NAME
  Py2Cpp
  GIT_TAG
  1.2
  GITHUB_REPOSITORY
  luk036/py2cpp
  OPTIONS
  "INSTALL_ONLY YES" # create an installable target
)

find_package(Boost REQUIRED)
if(Boost_FOUND)
  message(STATUS "Found boost: ${Boost_INCLUDE_DIRS}")
  # add_library(Boost::boost INTERFACE IMPORTED GLOBAL)
  target_include_directories(Boost::boost SYSTEM
                             INTERFACE ${Boost_INCLUDE_DIRS})
  # Disable autolink
  target_compile_definitions(Boost::boost INTERFACE BOOST_ALL_NO_LIB=1)
endif()

find_package(Boost REQUIRED)

cpmaddpackage(NAME cppcoro GIT_TAG 1.0 GITHUB_REPOSITORY luk036/cppcoro)
# print_target_properties(cppcoro)
if(cppcoro_ADDED)
  message(STATUS "Found cppcoro: ${cppcoro_SOURCE_DIR}")
  add_library(cppcoro::cppcoro INTERFACE IMPORTED GLOBAL)
  target_include_directories(cppcoro::cppcoro SYSTEM
                             INTERFACE ${cppcoro_SOURCE_DIR}/include)
endif(cppcoro_ADDED)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  add_compile_options(-fcoroutines)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using clang
  add_compile_options(-fcoroutines-ts -stdlib=libc++)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  add_compile_options(/std:c++latest /await)
endif()

set(SPECIFIC_LIBS Py2Cpp::Py2Cpp fmt::fmt Boost::boost cppcoro::cppcoro)
