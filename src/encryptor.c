// The main file that will be used to build the base of the project. Will call all the other files/headers and
// use them as needed.

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "commons.h"
#include "ciphers.h"
#include "data_input.h"

#define true 1
#define false 0

typedef short bool;
typedef char *string;
typedef const char *const_str;

/**
 * A test method made for debugging/editing the code without making any
 * breaking changes to the main method. Designed so that the flow-of-control
 * ends the program instead of going back to the main method.
 */
void subroutine(int argc, string *argv) {
	// Debug/Test code here.

	// Printing a message to avoid any confusion.
	printf("\n\nDebug run terminated.\n");
	exit(0);
}

int main(int argc, string *argv) {
//	subroutine(argc, argv);

	// Declaring a structure to accept/process user input.
	struct user_data data;

	// Reading user input - either from stdin, or in interactive mode with the user.
	populate_data(&data, argc, argv);

	// Printing the input received so far as a part of the result.
	printf("\nOriginal Key: `%s` \n\tProcessed Key: `%s`"
		   "\n\nOriginal Message: \n\t%s \n\nProcessed Message: \n\t%s\n",
		   data.cipher_key,
		   data.processed_key,
		   data.cipher_message,
		   data.processed_message
	);

	string result = NULL;

	// Depending on the values selected by the user, using the appropriate
	// cipher algorithm with relevant data.
	switch (data.cipher) {
		case PLAYFAIR:
			if (data.encrypt) {
				result = crypt_play_fair(
					data.processed_message,
					data.processed_key,
					data.verbose
				);
			} else {
				result = decrypt_play_fair(
					data.processed_message,
					data.processed_key,
					data.verbose
				);
			}

			break;

		case HILL_CIPHER:
			result = crypt_hill_cipher(
				data.processed_message,
				data.processed_key,
				data.verbose
			);

			break;

		case RAILFENCE:
			if (data.encrypt)
				result = crypt_railfence(
					data.processed_key,
					data.processed_message,
					data.verbose
				);

			break;

		default:
			printf("No state found in the main switch :(");
	}

	// Printing the result. Since the original message loses its formatting before being
	// ciphered (spaces being removed, capitals being lowered), undo the appropriate changes
	// while printing the output string.
	printf("\nCipher Result: \n\t");
	for (unsigned int i = 0; data.cipher_message[i] != '\0'; i++)
		printf("%c",
			   isalpha(data.cipher_message[i]) ?
			   (isupper(data.cipher_message[i]) ? toupper(result[i]) : result[i]) :
			   data.cipher_message[i]
		);

	printf("\n\n");

	return 0;
}