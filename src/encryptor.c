// The main file that will be used to build the base of the project. Will call all the other files/headers and
// use them as needed.

#include <pcre.h>
#include <stdio.h>
#include <ctype.h>

#include "commons.h"
#include "ciphers.h"

#define true 1
#define false 0

#define INPUT_LIMIT 20
#define LONG_INPUT 1000

#define scan(c, limit) fgets(c, limit, stdin)

typedef short bool;
typedef char *string;
typedef const char *const_str;

struct user_input {
	string name;
};

/**
 * A test method made for debugging/editing the code without making any
 * breaking changes to the main method. Designed so that the flow-of-control
 * ends the program instead of going back to the main method.
 */
void subroutine() {
	// Debug/Test code here.
//	init();

	// Printing a message to avoid any confusion.
	printf("\n\nDebug run terminated.\n");
	exit(0);
}

int main(int argc, string *argv) {
	subroutine();

	string cipher_type = NULL;

	string original_message = NULL;
	string cipher_key = NULL;

	string p_cipher_val = "(?i)((playfair))";
	string p_message_val = "[a-zA-Z_ ]+";

	bool cli_detected = false;

	if (argc > 1) {
		// Block to extract values from command-line parameters being passed.
		// The first parameter is the path to the script being run - useless.

		cli_detected = true; // Marking that command-line arguments have been detected.

		// Regex patterns being used to extract values
		string p_cipher_type = "^--cipher=(?i)((playfair))$";
		string p_cipher_key = "^--key=(([a-zA-Z ]+))$";
		string p_message = "^--message=((.+))$";

		// Iterating over all arguments except the first one.
		for (int i = 1; i < argc; i++) {
			string argument = argv[i];

			// Validating if a pattern matches the argument - extracting values if so.
			if (validate(p_cipher_type, argument)) {
				cipher_type = extract_data(p_cipher_type, argument);
			} else if (validate(p_cipher_key, argument)) {
				cipher_key = extract_data(p_cipher_key, argument);
			} else if (validate(p_message, argument)) {
				original_message = extract_data(p_message, argument);
			} else {
				printf("Error: Unexpected argument detected `%s`\n\n", argument);
				exit(-10);
			}
		}
	}

	if (cipher_type == NULL) {
		printf("\nDefaulting cipher type to `playfair`\n");
		cipher_type = gen_str("playfair");
	}

	if (cipher_key == NULL) {
		printf("\nDefaulting cipher key to `encryptor`\n");
		cipher_key = gen_str("encryptor");
	}

	if (original_message == NULL) {
		printf("\nDefaulting message to `ciphers`\n");
		original_message = gen_str("ciphers");
	}

	// Performing replacements in cipher message.
	string cipher_temp_message = gen_str(original_message);
	unsigned int counter = 0;
	for (unsigned int i = 0; original_message[i] != '\0'; i++)
		if (original_message[i] != ' ')
			cipher_temp_message[counter++] = original_message[i];

	counter = 0;

	// Performing replacements in the key to be used.
	string modified_key = gen_str(cipher_key);
	for (unsigned int i = 0; cipher_key[i] != '\0'; i++)
		if (cipher_key[i] != ' ')
			modified_key[counter++] = cipher_key[i];

	string result = "No result found";

	if (compare(
		cipher_type,
		"PlayFair",
		false
	)) {
		result = crypt_play_fair(cipher_temp_message, modified_key, true);
	}

	printf("\n\nCipher Type: %s \nCipher Key: %s \nEncrypted Message: %s\n\n\n", cipher_type, cipher_key, result);

	return 0;
}