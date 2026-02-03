cmake_minimum_required(VERSION 4.0)
project(A2)

set(CMAKE_CXX_STANDARD 20)

add_executable(A2 main.cpp
        DES/fesitel_cipher.h
        DES/feistel_cipher.cpp)
