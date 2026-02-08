//
// Created by bradley masciotra on 2026-01-27.
//

#ifndef A2_FEISTEL_CIPHER_H
#define A2_FEISTEL_CIPHER_H

#include <string>
#include <vector>

using namespace std;

string des_encrypt(string &key, string &message);
string des_decrypt(string &key, string &cipher_text);

#endif //A2_FEISTEL_CIPHER_H
