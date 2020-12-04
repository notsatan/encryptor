
#ifndef __encryptor_data_input
#define __encryptor_data_input

#define scan(c, limit) fgets(c, limit, stdin)

#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include "commons.h"

#define false 0
#define true 1

typedef short bool;
typedef char *string;

// Macro for fast creation of new strings.
#define new_str(size) (string) malloc(size * sizeof(char))

// Size of a small string - 15 characters.
#define STRING_SMALL 15

// Size of a medium string - 50 characters.
#define STRING_MEDIUM 50

// Size of a large string - 500 characters.
#define STRING_LARGE 500


struct user_data {
	// String containing the message that is to be ciphered.
	string cipher_message;

	// String containing the key that is to be used in the cipher
	string cipher_key;

	// Boolean indicating if the message is to be encrypted/decrypted.
	// A value of `true` indicates that the message is to be encrypted.
	bool encrypt;

	// Boolean indicating if verbose mode is to be used with the cipher
	// If set to true, the process followed after each step will be printed.
	bool verbose;

	// The final version of the message string. Will be stripped of spaces,
	// and any excess characters as needed by the algorithm.
	string processed_message;

	// The final version of the cipher key, will be stripped of spaces and any
	// excess characters as needed by the cipher algorithm.
	string processed_key;

	// An enum indicating the type of cipher that is to be used.
	enum crypt cipher;
};

void populate_data(struct user_data *self, int argc, string *argv);


#endif //__encryptor_data_input
