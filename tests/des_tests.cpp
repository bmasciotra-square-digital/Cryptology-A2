//
// Created by bradley masciotra on 2026-02-02.
//
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "string"
#include "DES/fesitel_cipher.h"

using namespace std;

TEST_CASE("DES - Main Case") {

    string key = "0f1571c947d9e859"; // assumed to be HEX
    string message = "02468aceeca86420";// assumed to be HEX

    string cipher = des_encrypt(key, message);

    REQUIRE(cipher == "DA02CE3A89ECAC3B");
}

