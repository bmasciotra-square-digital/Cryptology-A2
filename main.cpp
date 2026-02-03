#include <iostream>
#include "DES/feistel_cipher.h"

int main() {

    string key = "0f1571c947d9e859";
    string message = "02468aceeca86420";

    string cipher = des_encrypt(key, message);
    cout << "Message: " << message << endl;
    cout << "Cipher Text: " << cipher << endl;

    string decrypted = des_decrypt(key, cipher);

    cout << "Decrypted: " << decrypted << endl;
    return 0;
}