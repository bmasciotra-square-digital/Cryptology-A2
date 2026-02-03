cmake_minimum_required(VERSION 3.14)
project(A2)

set(CMAKE_CXX_STANDARD 20)

# Fetch Catch2
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.10
)
FetchContent_MakeAvailable(Catch2)

# Main application
add_executable(A2 main.cpp
    DES/fesitel_cipher.h
    DES/feistel_cipher.cpp
)

# Test executable (links Catch2 and DES code)
add_executable(A2tests tests/des_tests.cpp
    DES/fesitel_cipher.h
    DES/feistel_cipher.cpp
)
target_include_directories(A2tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(A2tests PRIVATE Catch2::Catch2)

# Enable CTest so you can run: ctest
enable_testing()
add_test(NAME A2tests COMMAND A2tests)


