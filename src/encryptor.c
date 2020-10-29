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
 * Compares a regex pattern against a string. Just a convenience module to segregate the
 * part of code interacting with the regex engine.
 *
 * @param regex_pattern: String containing the regex pattern that is to be compiled.
 * @param input: Input which is to be verified against the regex pattern.
 *
 * @remarks
 * 		Will simply perform a check as to whether a string matches the given pattern. Does not
 * 		extract groups and/or return them back to the calling method. Exists as a simple
 * 		litmus test.
 *
 * @return
 * 		Boolean value containing true if the pattern compiles successfully and matches the input
 * 		string. False increase the string does not match with the pattern.
 */
bool validate(string regex_pattern, string input) {
	if (input == NULL || regex_pattern == NULL)
		// Prevent random errors
		return false;

	// Checking the compiled pattern against input string and returning the result.
	int result = pcre_exec(
		regex_compile(regex_pattern),  // The compiled pattern against which string is to be checked.
		NULL,                          // No associated structure required.
		input,                         // The string which is to be matched to the compiled pattern.
		(int) strlen(input),           // Basically, the length up to which the pattern is to be checked.
		0,                             // Matching the pattern from the start of the string - no offset required.
		0,                             // No special options needed
		0,                             // No result offsets to be acquired - used if groups are to be extracted.
		0                              // Again, no result offsets to be acquired.
	);

	if (result < 0)
		// If the result is negative - the string failed does not match the pattern
		return false;

	return true;
}

/**
 * Extracts a value from a string by compiling a valid regex pattern.
 *
 * @remarks
 * 		Used to extract a single value from a string using a regex pattern. The
 * 		internal implementation of the header file is botch-y at best, due to
 * 		which the output obtained by extracting the value of the regex expression
 * 		from the string can be faulty.
 *
 * @param regex_pattern: String containing the regex pattern that is to be compiled and extracted from.
 * @param input_string: The input string against which the regex pattern is to be compiled and tested.
 *
 * @return
 * 		Value obtained by extracting the value of the regex pattern by matching it against the input
 * 		string.
 */
string extract_data(string regex_pattern, string input_string) {
	int vector_size = 48;
	int result_vector[vector_size];

	int rc = pcre_exec(
		regex_compile(regex_pattern),
		0,
		input_string,
		(int) strlen(input_string),
		0,
		0,
		result_vector,
		vector_size
	);

	if (rc >= 3)
		return input_string + result_vector[2];
	else
		return "";
}

int main(int argc, string *argv) {
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
				exit(10);
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
		printf("\nDefaulting message to `cipher text in here`\n");
		original_message = gen_str("cipher text in here");
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
		result = play_fair(cipher_temp_message, modified_key);
	}

	printf("Cipher Type: %s \nCipher Key: %s \nEncrypted Message: %s", cipher_type, cipher_key, result);

	return 0;
}