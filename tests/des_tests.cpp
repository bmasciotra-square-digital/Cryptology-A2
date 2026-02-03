//
// Created by bradley masciotra on 2026-02-02.
//
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "string"
#include "DES/feistel_cipher.h"
#include <algorithm>

using namespace std;

TEST_CASE("DES - Main Case") {

    string key = "0f1571c947d9e859";
    string message = "02468aceeca86420";

    string cipher = des_encrypt(key, message);

    REQUIRE(cipher == "DA02CE3A89ECAC3B");
}


TEST_CASE("DES - Decrypted") {

    string key = "0f1571c947d9e859";
    string message = "02468aceeca86420";

    string cipher = des_encrypt(key, message);

    REQUIRE(cipher == "DA02CE3A89ECAC3B");

    string decrypted = des_decrypt(key, cipher);
    transform(message.begin(), message.end(), message.begin(), ::toupper);

    REQUIRE(decrypted == message);
}
