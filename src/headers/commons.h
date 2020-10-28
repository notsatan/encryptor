// A simple header to define all common methods that will be used at various places across the project. This header
// file will contain declarations of all common methods/objects.

#ifndef __encryptor_commons
#define __encryptor_commons

#include <string.h>
#include <stdarg.h>
#include <pcre.h>

typedef short bool;
typedef char *string;

extern bool compare(const char *, const char *, bool);

extern inline bool str_end(char);

extern inline pcre *regex_compile(char *);

/**
 * An enum to define the types of ciphers being used. Will be used to keep a track of the cipher selected
 * by the user.
 */
enum crypt {
	PLAYFAIR,
	HILL,
	RAILFENCE
};

extern inline char *map_crypt(enum crypt);

#endif //__encryptor_commons
