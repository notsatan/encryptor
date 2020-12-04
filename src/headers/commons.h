// A simple header to define all common methods that will be used at various places across the project. This header
// file will contain declarations of all common methods/objects.

#ifndef __encryptor_commons
#define __encryptor_commons

#include <string.h>
#include <stdarg.h>
#include <pcre.h>

typedef short bool;
typedef char *string;

extern bool compare(char *, char *, bool);

extern inline pcre *regex_compile(char *);

extern inline bool l_compare(string, string);

extern string convert_lower(string message);

extern inline string gen_str(string);

extern inline string raw_gen_str(string, unsigned int);

extern inline string gen_str_pad(string, unsigned int);

bool validate(string pattern, string message);

string extract_data(string pattern, string message);

extern inline string scan_str(string destination, unsigned int length);

/**
 * An enum to define the types of ciphers being used. Will be used to keep a track of the cipher selected
 * by the user.
 *
 * @note
 * 		If a new crypt technique is being added, the relevant changes are to be made in:
 * 		`commons.c/map_cipher`;
 * 		`data_input.c/interactive`;
 * 		`data_input.c/fetch_cli_args`
 */
enum crypt {
	PLAYFAIR,
	HILL_CIPHER,
	RAILFENCE,
	UNDEFINED
};

extern inline enum crypt map_cipher(string cipher_name);

#endif //__encryptor_commons
