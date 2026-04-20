# AGENTS.md - xnetwork-cpp Development Guide

## Project Overview

This is a **header-only C++ graph library** inspired by Python's NetworkX. The project uses modern CMake, C++14, and the doctest framework.

---

## Build Commands

### Full Build
```bash
# Build everything (library + tests + standalone)
cmake -S all -B build
cmake --build build
```

### Test Build & Run
```bash
# Configure and build tests
cmake -S test -B build/test
cmake --build build/test

# Run all tests
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

# Or run directly
./build/test/XNetworkTests
```

### Run a Single Test
```bash
# Run a specific test case by name
./build/test/XNetworkTests -tc="Test xnetwork::Graph"
```

### Build Standalone Example
```bash
cmake -S standalone -B build/standalone
cmake --build build/standalone
./build/standalone/XNetwork --help
```

### Code Formatting
```bash
# Install dependencies first
pip install clang-format==18.1.2 cmake_format==0.6.13 pyyaml

# Check formatting
cmake --build build/test --target format

# Apply formatting
cmake --build build/test --target fix-format
```

### Enable Sanitizers
```bash
cmake -S test -B build/test -DUSE_SANITIZER="Address;Undefined"
cmake --build build/test
./build/test/XNetworkTests
```

### Enable Static Analysis
```bash
cmake -S test -B build/test -DUSE_STATIC_ANALYZER=clang-tidy
cmake --build build/test
```

---

## Code Style Guidelines

### General Rules
- **C++ Standard**: C++14 (enforced in test/CMakeLists.txt)
- **Header-only library**: All code in `include/xnetwork/`
- **Formatting**: clang-format with Google-based style (see `.clang-format`)

### Formatting Rules (.clang-format)
- **BasedOnStyle**: Google
- **ColumnLimit**: 100
- **IndentWidth**: 4
- **NamespaceIndentation**: All
- **BreakBeforeBraces**: Attach
- **BreakBeforeBinaryOperators**: All
- **AllowAllParametersOfDeclarationOnNextLine**: false
- **ConstructorInitializerAllOnOneLineOrOnePerLine**: true

### Naming Conventions
- **Classes**: PascalCase (e.g., `Graph`, `DiGraph`, `SimpleGraph`)
- **Functions**: snake_case (e.g., `add_edge`, `bfs_distances`)
- **Variables**: snake_case (e.g., `num_nodes`, `gra`)
- **Constants**: SCREAMING_SNAKE_CASE for compile-time constants
- **Files**: lowercase with underscores (e.g., `graph.hpp`, `test_xnGraph.cpp`)

### Include Order (grouped by clang-format)
1. Corresponding header (for .cpp files)
2. C++ standard library headers (`<vector>`, `<unordered_map>`, etc.)
3. External library headers (`<py2cpp/...>`, `<doctest/...>`)
4. Internal project headers (`<xnetwork/...>`)

Example:
```cpp
#include <cstdint>
#include <vector>
#include <py2cpp/py2cpp.hpp>
#include <xnetwork/classes/graph.hpp>
```

### Error Handling
- Use the `XNETWORK_THROW(exception, msg)` macro defined in `xnetwork_config.hpp`
- The macro respects exception compilation settings
- For errors that should abort: use `XNETWORK_THROW`
- Always include descriptive error messages

### Template Usage
- The library heavily uses templates for generic graph types
- Use `template <typename Graph>` for functions operating on any graph type
- Common template types: `Value_type<T>`, `Node`, `Edge`

### Namespace
- All library code lives in `namespace xnetwork { }`
- Use nested namespaces for organization (e.g., `xnetwork::classes`, `xnetwork::generators`)

---

## Project Structure

```
include/xnetwork/
  ├── classes/       # Graph data structures
  │   ├── graph.hpp
  │   ├── digraphs.hpp
  │   ├── coreviews.hpp
  │   └── reportviews.hpp
  ├── generators/    # Graph generators
  │   └── testcases.hpp
  ├── exception.hpp  # Custom exceptions
  └── xnetwork_config.hpp  # Configuration and version

test/source/
  ├── main.cpp
  ├── test_xnGraph.cpp
  ├── test_xnDiGraphS.cpp
  └── test_xnDiGraphS2.cpp
```

---

## Dependencies

- **py2cpp** (>= 1.4): Python-like C++ syntax utilities
- **doctest** (2.4.11): Testing framework
- **PackageProject.cmake**: For installable targets

---

## Testing Conventions

- Use doctest's `TEST_CASE("Test description")` macro
- Place tests in `test/source/` directory
- Test naming: `test_xn{ClassName}.cpp`
- Use `CHECK()` for assertions

Example:
```cpp
TEST_CASE("Test xnetwork::Graph") {
    auto gra = xnetwork::SimpleGraph{5};
    gra.add_edge(0, 1);
    CHECK(gra.number_of_nodes() == 5);
    CHECK(gra.number_of_edges() == 1);
}
```

---

## Common Tasks

### Adding a New Graph Class
1. Add header to `include/xnetwork/classes/`
2. Follow existing class patterns (inherit from base Graph/DiGraph)
3. Add test file in `test/source/test_xn{Name}.cpp`

### Adding a New Algorithm
1. Add to appropriate header in `include/xnetwork/`
2. Use templates for generic graph support
3. Add tests covering edge cases

### Building Documentation
```bash
cmake -S documentation -B build/doc
cmake --build build/doc --target GenerateDocs
```