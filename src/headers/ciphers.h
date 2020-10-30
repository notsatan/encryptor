#ifndef __encryptor_ciphers
#define __encryptor_ciphers

#define true 1
#define false 0

typedef short bool;
typedef char *string;
typedef const char *const_str;

string crypt_play_fair(string message, string key, bool noob_friendly);

#endif //__encryptor_ciphers
