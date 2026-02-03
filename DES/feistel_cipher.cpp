//
// Created by bradley masciotra on 2026-01-27.
//
#include "fesitel_cipher.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

const int BLOCK_SIZE = 64;
const int LEFT_BLOCK_SIZE = 32;
const int RIGHT_BLOCK_SIZE = 32;

string convert_str_to_binary(string &message) {
    string binary;
    for_each(message.begin(), message.end(),
             [&binary](unsigned char c) {
                 binary += std::bitset<8>(c).to_string();
             }
    );

    return binary;
}

vector<string> convert_binary_string_to_blocks(string &binary) {
    vector<string> blocks;

    for (size_t i = 0; i < binary.size(); i += BLOCK_SIZE) {
        string b = binary.substr(i, BLOCK_SIZE);

        // if the block is less than 64 bits we need to pad
        if (b.size() < BLOCK_SIZE) {
            b.append(BLOCK_SIZE - b.size(), '0'); // simple zero padding for time being TODO check
        }

        blocks.push_back(b);

        return blocks;
    }

    string create_key(string &key) {
        string new_key;
        string converted = convert_str_to_binary(key);

        for (int i = 0; i < converted.size(); i++) {
            if (i % 8 == 0) {
                // skip the 8th bit
                continue;
            }

            new_key += converted[i];
        }

        cout << new_key.size() << endl;
        return new_key;
    }


    string initial_permutation(string &bits) {

        // initial permutation matrix
        int IP[64] = {
                58, 50, 42, 34, 26, 18, 10, 2,
                60, 52, 44, 36, 28, 20, 12, 4,
                62, 54, 46, 38, 30, 22, 14, 6,
                64, 56, 48, 40, 32, 24, 16, 8,
                57, 49, 41, 33, 25, 17, 9, 1,
                59, 51, 43, 35, 27, 19, 11, 3,
                61, 53, 45, 37, 29, 21, 13, 5,
                63, 55, 47, 39, 31, 23, 15, 7
        };

        string output;

        for (int i: IP) {
            output += bits[i - 1];
        }

        return output;
    }

    string feistel_round(string left, string right) {


        return "";


    }

    string remove_parity_bits(string &key) {

        string removed_parity;
        for (int i = 0; i < key.length(); i++) {
            if (i % 8 == 0) {
                removed_parity += key[i];
            }
        }

        return removed_parity;
    }

    string circular_shift(string &side, int amount = 1) {
        size_t n = side.size();
        amount %= (int) n;

        return side.substr(amount) + side.substr(0, amount);
    }


    string pc_2(string &combined_key) {
        int PC2[48] = {
                14, 17, 11, 24, 1, 5,
                3, 28, 15, 6, 21, 10,
                23, 19, 12, 4, 26, 8,
                16, 7, 27, 20, 13, 2,
                41, 52, 31, 37, 47, 55,
                30, 40, 51, 45, 33, 48,
                44, 49, 39, 56, 34, 53,
                46, 42, 50, 36, 29, 32
        };

        string roundKey;

        for (int i: PC2) {
            roundKey += combined_key[i - 1];
        }

        return roundKey;
    }

    vector<string> generate_round_keys(string &key) {


        vector<string> keys;
        for (int i = 0; i < 16; i++) {
            // becomes 56 bit key
            string remove_parity = remove_parity_bits(key);

            // break down into left and right sides of the 56 bit key
            string left = remove_parity.substr(0, 28);
            string right = remove_parity.substr(28, 28);

            // circular shift each side
            string circular_left = circular_shift(left);
            string circular_right = circular_shift(right);

            // combine the shifted left and right side
            string combined_key = circular_left + circular_right;

            // PC-2
            string pc_2_key = pc_2(combined_key);
            keys.push_back(pc_2_key);
        }
        return keys;
    }

    string des_encrypt(string &key, string &message) {
        int key_size = 128;

        // convert string to binary representation for operations
        string message_to_bin = convert_str_to_binary(message);
        string key_to_bin = convert_str_to_binary(key);

        // convert the binary string to 64 bit blocks to accommodate a possibly larger message
        vector<string> blocks = convert_binary_string_to_blocks(message_to_bin);

        // generate the keys
        string k = create_key(key);

        auto keys = generate_round_keys(key_to_bin);

        // int index = 1;
        string cipher_text;

        for (string &block: blocks) {

            // Initial permutation for each block
            string permuted_bits = initial_permutation(block);

            string left = block.substr(0, LEFT_BLOCK_SIZE);
            string right = block.substr(LEFT_BLOCK_SIZE, RIGHT_BLOCK_SIZE);


            cipher_text += feistel_round(left, right);

        }


        return cipher_text;
    }

    string des_decrypt(string &cipher_text) {

    }


