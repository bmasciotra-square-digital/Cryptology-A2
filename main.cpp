#include <iostream>
#include "DES/fesitel_cipher.h"

int main() {

    string key = "0f1571c947d9e859"; // assumed to be HEX
    string message = "02468aceeca86420";// assumed to be HEX

    string cipher = des_encrypt(key, message);
    cout << "Message: " << message << endl;
    cout << "Cipher Text: " << cipher << endl;

    return 0;
}