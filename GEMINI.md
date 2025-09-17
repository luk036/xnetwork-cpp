
# GEMINI.md

## Project Overview

This project is a header-only C++ library for graph data structures, inspired by Python's NetworkX library. It aims to provide a modern C++ implementation of graph algorithms and data structures. The project is named `xnetwork-cpp`.

The library is designed to be header-only, making it easy to integrate into other projects. It uses `py2cpp` for Python interoperability, which suggests that it can be used in C++ projects that need to interact with Python code or data structures.

The project uses modern CMake practices and also provides an `xmake.lua` file, indicating that it can be built using either CMake or xmake.

## Building and Running

The project can be built using either CMake or xmake.

### CMake

**Build and run the standalone target:**

```bash
cmake -S standalone -B build/standalone
cmake --build build/standalone
./build/standalone/XNetwork --help
```

**Build and run test suite:**

```bash
cmake -S test -B build/test
cmake --build build/test
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test
```

**Build everything at once:**

```bash
cmake -S all -B build
cmake --build build
```

### xmake

**Build the project:**

```bash
xmake
```

**Run tests:**

```bash
xmake run test_xnetwork
```

## Development Conventions

*   **Code Formatting:** The project uses `clang-format` and `cmake-format` to enforce a consistent code style.
*   **Testing:** Unit tests are written using the `doctest` framework and are located in the `test/source` directory.
*   **Dependencies:** The project uses `CPM.cmake` for dependency management. The main dependencies are `py2cpp`, `doctest`, and `fmt`.
*   **Contributions:** The `README.md` file provides detailed instructions for contributors, including how to build the project, run tests, and format code.
