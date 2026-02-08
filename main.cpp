#include <iostream>
#include "DES/feistel_cipher.h"

int main() {
    // DES Analysis ----------------------------------------------------------------------
    
    string key = "0f1571c947d9e859";
    string message = "02468aceeca86420";
    
    string cipher = des_encrypt(key, message);
    
    cout << "Message: " << message << endl;
    cout << "Cipher Text: " << cipher << endl;
    
    string decrypted = des_decrypt(key, cipher);
    
    // Show the decrypted message
    cout << "Decrypted: " << decrypted << endl;
    
    cout << endl;
    
    // SPAC and SKAC Analysis -------------------------------------------------------------
    
    // SPAC Key
    string spac_key = "a3f1c9e4872d5b6c";
    
    // The messages differ by one bit, ie the last bit 0 to 1
    string spac_message_one = "02468aceeca86420";
    string spac_message_two = "02468aceeca86421";
    
    string spac_cipher_one = des_encrypt(spac_key, spac_message_one);
    string spac_cipher_two = des_encrypt(spac_key, spac_message_two);
    
    cout << "SPAC Message One: " << spac_message_one << endl;
    cout << "SPAC Cipher One: " << spac_cipher_one << endl;
    
    cout << "SPAC Message Two: " << spac_message_two << endl;
    cout << "SPAC Cipher Two: " << spac_cipher_two << endl;
    

    cout << endl;

    // SKAC Analysis

    string skac_key_one = "b3f1c9e4872d5b6c";
    string skac_key_two = "a3f1c9e4872d5b6c"; // This key differs by one bit from the first key

    // The messages differ by one bit, ie the last bit 0 to 1
    string skac_message = "02468aceeca86420";

    string skac_cipher_one = des_encrypt(skac_key_one, skac_message);
    string skac_cipher_two = des_encrypt(skac_key_two, skac_message);
    
    // Show the SKAC keys and ciphers
    cout << "SKAC Key One: " << skac_key_one << endl;
    cout << "SKAC Cipher One: " << skac_cipher_one << endl;

    cout << "SKAC Key Two: " << skac_key_two << endl;
    cout << "SKAC Cipher Two: " << skac_cipher_two << endl;

    return 0;
}
