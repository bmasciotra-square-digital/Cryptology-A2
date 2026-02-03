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
const int HEX_BIN_SIZE = 4;

string convert_str_to_binary(const string &hex) {
    string bin;
    bin.reserve(hex.size() * HEX_BIN_SIZE);

    // convert from ASCII to bin
    for (char c: hex) {
        int v;

        if (c >= '0' && c <= '9') v = c - '0';

        else if (c >= 'a' && c <= 'f') v = 10 + (c - 'a');

        else if (c >= 'A' && c <= 'F') v = 10 + (c - 'A');

        else throw runtime_error("Invalid hex char");

        bin += bitset<4>(v).to_string();
    }
    return bin;
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
    }

    return blocks;
}


string initial_permutation(string &bits) {

    const int IP[64] = {
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


string remove_parity_bits(string &key) {

    int PC1[56] = {
            57, 49, 41, 33, 25, 17, 9,
            1, 58, 50, 42, 34, 26, 18,
            10, 2, 59, 51, 43, 35, 27,
            19, 11, 3, 60, 52, 44, 36,
            63, 55, 47, 39, 31, 23, 15,
            7, 62, 54, 46, 38, 30, 22,
            14, 6, 61, 53, 45, 37, 29,
            21, 13, 5, 28, 20, 12, 4
    };

    string out;
    out.reserve(56);
    for (int i: PC1) out += key[i - 1];
    return out;
}

// Left circular shift by default 1
string circular_shift(string &side, int amount = 1) {
    size_t n = side.size();
    amount %= (int) n;

    return side.substr(amount) + side.substr(0, amount);
}

// Perform PC 2
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

    int shifts[16] = {
            1, 1, 2, 2, 2, 2, 2, 2,
            1, 2, 2, 2, 2, 2, 2, 1
    };

    vector<string> keys;

    // becomes 56 bit key
    string remove_parity = remove_parity_bits(key);

    // break down into left and right sides of the 56 bit key
    string left = remove_parity.substr(0, 28);
    string right = remove_parity.substr(28, 28);

    for (int shift: shifts) {

        // circular shift each side
        left = circular_shift(left, shift);
        right = circular_shift(right, shift);

        // combine the shifted left and right side
        string combined_key = left + right;

        // PC-2
        string pc_2_key = pc_2(combined_key);

        keys.push_back(pc_2_key);
    }

    return keys;
}

string permute(const string &bits, const int *table, int tableSize) {
    string out;
    out.reserve(tableSize);

    for (int i = 0; i < tableSize; i++) {
        out += bits[table[i] - 1];
    }
    return out;
}

string xor_bits(const string &a, const string &b) {
    string out;
    out.reserve(a.size());

    for (size_t i = 0; i < a.size(); i++) {
        out += (a[i] == b[i]) ? '0' : '1';
    }

    return out;
}

string f_function(const string &right32, const string &key48) {
    int const E_EXPANSION[48] = {
            32, 1, 2, 3, 4, 5,
            4, 5, 6, 7, 8, 9,
            8, 9, 10, 11, 12, 13,
            12, 13, 14, 15, 16, 17,
            16, 17, 18, 19, 20, 21,
            20, 21, 22, 23, 24, 25,
            24, 25, 26, 27, 28, 29,
            28, 29, 30, 31, 32, 1
    };

    int const P[32] = {
            16, 7, 20, 21,
            29, 12, 28, 17,
            1, 15, 23, 26,
            5, 18, 31, 10,
            2, 8, 24, 14,
            32, 27, 3, 9,
            19, 13, 30, 6,
            22, 11, 4, 25
    };

    string expanded = permute(right32, E_EXPANSION, 48);

    // --- 2) XOR with 48-bit round key
    string xor_value = xor_bits(expanded, key48);

    // --- 3) S-Boxes (8 of them)
    int S[8][4][16] = {
            // S1
            {
                    {14, 4,  13, 1,  2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0,  7},
                    {0,  15, 7,  4,  14, 2,  13, 1,  10, 6, 12, 11, 9,  5,  3,  8},
                    {4,  1,  14, 8,  13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5,  0},
                    {15, 12, 8,  2,  4,  9,  1,  7,  5,  11, 3,  14, 10, 0, 6,  13}
            },
            // S2
            {
                    {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
                    {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
                    {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
                    {13, 8, 10, 1, 3, 15, 4, 2, 11, 6,       7,  12, 0,  5, 14, 9}
            },
            // S3
            {
                    {10, 0,  9,  14, 6,  3,  15, 5,  1,  13, 12, 7,  11, 4,  2,  8},
                    {13, 7,  0,  9,  3,  4,  6,  10, 2,  8, 5,  14, 12, 11, 15, 1},
                    {13, 6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12, 5,  10, 14, 7},
                    {1,  10, 13, 0,  6,  9,  8,  7,  4,  15, 14, 3,  11, 5, 2,  12}
            },
            // S4
            {
                    {7,  13, 14, 3,  0,  6,  9,  10, 1,  2,  8,  5,  11, 12, 4,  15},
                    {13, 8,  11, 5,  6,  15, 0,  3,  4,  7, 2,  12, 1,  10, 14, 9},
                    {10, 6,  9,  0,  12, 11, 7,  13, 15, 1,  3,  14, 5,  2,  8,  4},
                    {3,  15, 0,  6,  10, 1,  13, 8,  9,  4,  5,  11, 12, 7, 2,  14}
            },
            // S5
            {
                    {2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0,  14, 9},
                    {14, 11, 2,  12, 4,  7,  13, 1,  5,  0, 15, 10, 3,  9,  8,  6},
                    {4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3,  0,  14},
                    {11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4, 5,  3}
            },
            // S6
            {
                    {12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3,  4,  14, 7,  5,  11},
                    {10, 15, 4,  2,  7,  12, 9,  5,  6,  1, 13, 14, 0,  11, 3,  8},
                    {9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1,  13, 11, 6},
                    {4,  3,  2,  12, 9,  5,  15, 10, 11, 14, 1,  7,  6,  0, 8,  13}
            },
            // S7
            {
                    {4,  11, 2,  14, 15, 0,  8,  13, 3,  12, 9,  7,  5,  10, 6,  1},
                    {13, 0,  11, 7,  4,  9,  1,  10, 14, 3, 5,  12, 2,  15, 8,  6},
                    {1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0,  5,  9,  2},
                    {6,  11, 13, 8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2, 3,  12}
            },
            // S8
            {
                    {13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7},
                    {1,  15, 13, 8,  10, 3,  7,  4,  12, 5, 6,  11, 0,  14, 9,  2},
                    {7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15, 3,  5,  8},
                    {2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5, 6,  11}
            }
    };

    string sbox_out;
    sbox_out.reserve(32);

    for (int box = 0; box < 8; box++) {
        string chunk = xor_value.substr(box * 6, 6);

        int row = (chunk[0] - '0') * 2 + (chunk[5] - '0');
        int col = (chunk[1] - '0') * 8 + (chunk[2] - '0') * 4 + (chunk[3] - '0') * 2 + (chunk[4] - '0');

        int val = S[box][row][col]; // 0..15

        sbox_out += bitset<4>(val).to_string();
    }

    string final = permute(sbox_out, P, 32);

    return final;
}


void feistel_round(string *left, string *right, string &key) {
    string original_left = *right;

    string f_out = f_function(*right, key);
    string new_right = xor_bits(*left, f_out);

    *left = original_left;
    *right = new_right;
}

string ip_inverse(string &bits) {
    int IP_INV[64] = {
            40, 8, 48, 16, 56, 24, 64, 32,
            39, 7, 47, 15, 55, 23, 63, 31,
            38, 6, 46, 14, 54, 22, 62, 30,
            37, 5, 45, 13, 53, 21, 61, 29,
            36, 4, 44, 12, 52, 20, 60, 28,
            35, 3, 43, 11, 51, 19, 59, 27,
            34, 2, 42, 10, 50, 18, 58, 26,
            33, 1, 41, 9, 49, 17, 57, 25
    };

    string output;
    output.reserve(64);

    for (int i: IP_INV) {
        output += bits[i - 1];
    }

    return output;
}

string convert_to_hex(string &bits) {
    const char *hex_values = "0123456789ABCDEF";

    string converted;
    size_t hex_amount = bits.size() / 4;

    // hex values will be bits / 4
    converted.reserve(hex_amount);


    for (int i = 0; i < hex_amount; i++) {
        string bin_value_to_convert = bits.substr(i * HEX_BIN_SIZE, HEX_BIN_SIZE);

        int val = 0;
        for (int j = 0; j < HEX_BIN_SIZE; j++) {
            // 0 == 2^3 , 1 = 2^2, 2 == 2^1 and 3 == 2^0
            if (bin_value_to_convert[j] == '1') {
                val += (int) pow(2, (3 - j));
            }
        }

        converted += hex_values[val];
    }

    return converted;

}

string des_encrypt(string &key, string &message) {

    // convert string to binary representation for operations
    string message_to_bin = convert_str_to_binary(message);
    string key_to_bin = convert_str_to_binary(key);

    // convert the binary string to 64 bit blocks to accommodate a possibly larger message
    vector<string> blocks = convert_binary_string_to_blocks(message_to_bin);

    // Generate round keys
    vector<string> keys = generate_round_keys(key_to_bin);

    string cipher_text;

    for (string &block: blocks) {
        // Initial permutation for each block
        string permuted_bits = initial_permutation(block);

        string left = permuted_bits.substr(0, LEFT_BLOCK_SIZE);
        string right = permuted_bits.substr(LEFT_BLOCK_SIZE, RIGHT_BLOCK_SIZE);

        for (int round = 0; round < 16; round++) {
            feistel_round(&left, &right, keys[round]);
        }

        string combined = right + left;
        string bin_inverse = ip_inverse(combined);
        cipher_text += convert_to_hex(bin_inverse);
    }


    return cipher_text;
}

string des_decrypt(string &cipher_text) {

}


