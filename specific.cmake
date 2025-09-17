CPMAddPackage(
  NAME fmt
  GIT_TAG 10.2.1
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

CPMAddPackage(
  NAME Py2Cpp
  GIT_TAG 1.5
  GITHUB_REPOSITORY luk036/py2cpp
  OPTIONS "INSTALL_ONLY YES" # create an installable target
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC

  # add_compile_options(-fcoroutines)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using clang
  add_compile_options(-stdlib=libc++)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  add_compile_options(/std:c++latest /await)
endif()

set(SPECIFIC_LIBS Py2Cpp::Py2Cpp fmt::fmt)
