// Header file containing the public API's available to use the different cipher
// algorithms available in the program. Used to reveal only a select amount of
// methods to the rest of the program while isolating the rest of it inside the
// source files.

#ifndef __encryptor_ciphers
#define __encryptor_ciphers

#define true 1
#define false 0

typedef short bool;
typedef char *string;
typedef const char *const_str;


string crypt_play_fair(string message, string key, bool verbose);

string decrypt_play_fair(string message, string key, bool verbose);

string crypt_hill_cipher(string, string, bool);

void validate_key_railfence(string key);

string crypt_railfence(string key, string message, bool verbose);

#endif //__encryptor_ciphers
