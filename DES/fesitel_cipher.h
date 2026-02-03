//
// Created by bradley masciotra on 2026-01-27.
//

#ifndef A2_FESITEL_CIPHER_H
#define A2_FESITEL_CIPHER_H

#include <string>
#include <vector>

using namespace std;

string substitute(string &plaintext);

string initial_permutation(string &bits);

string des_encrypt(string &key, string &message);

string des_decrypt(string &cipher_text);

vector<string> convert_binary_string_to_blocks(string &binary);

string create_key(string &key);

void f_box();


#endif //A2_FESITEL_CIPHER_H
