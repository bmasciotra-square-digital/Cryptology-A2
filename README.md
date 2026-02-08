# Cryptology-A2

DES (Data Encryption Standard) implementation with Catch2 unit tests.

## Prerequisites

- **CMake** (3.14 or newer)
- **C++20**-compatible compiler (e.g. GCC, Clang, MSVC)

Catch2 is fetched automatically by CMake when the project is configured and built.

---

## Building

From the project root (`A2`):

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

On Unix/macOS you can use `make` instead of `cmake --build .` if you prefer.

---

## Running the Main Program

From the `build` directory:

```bash
./A2
```

This runs the DES demo (e.g. SKAC analysis) defined in `main.cpp`.
This main file contains the main DES encryptions as well as some of the analysts for SKAC and SPAC

---

## Running the DES Tests (Catch2)

The main DES algorithm has a Catch2 test associated with it for the given test case provided and can be used to check the algorithm in conjunction with the main method.

From the `build` directory, run the test executable:

```bash
./A2tests
```

Or use CTest:

```bash
ctest
```

To see test output when using CTest:

```bash
ctest --output-on-failure
```
