// The main file that will be used to build the base of the project. Will call all the other files/headers and
// use them as needed.

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#include "commons.h"
#include "ciphers.h"
#include "data_input.h"

#define true 1
#define false 0

typedef short bool;
typedef char *string;
typedef const char *const_str;

/**
 * Get the current system date as a string.
 *
 * @return
 * 	A string containing the system date in a pre-formatted manner.
 */
string get_date() {
	// Getting an instance of the timer - will be used to fetch system time
	time_t time_manager = time(NULL);

	// Getting the local time as a structure - relative to epoc time.
	struct tm time_container = *localtime(&time_manager);

	// Creating string to store and return the result.
	const unsigned int MAX_LENGTH = 20;
	string result = (string) malloc(sizeof(char) * MAX_LENGTH);

	// Formatting the string to contain the date.
	snprintf(
		result,
		MAX_LENGTH,
		"%02d/%02d/%02d",

		// Adjusting the values to be human-friendly.
		time_container.tm_mday,
		time_container.tm_mon + 1,
		time_container.tm_year + 1990
	);

	return result;
}

string get_time() {
	// Getting an instance of the timer - will be used to fetch system time
	time_t time_manager = time(NULL);

	// Getting the local time as a structure - relative to epoc time.
	struct tm time_container = *localtime(&time_manager);

	// Creating string to store and return the result.
	const unsigned int MAX_LENGTH = 20;
	string result = (string) malloc(sizeof(char) * MAX_LENGTH);

	// Formatting the string to contain the current date and time.
	snprintf(
		result,
		MAX_LENGTH,
		"%02d:%02d:%02d",
		time_container.tm_hour,
		time_container.tm_min,
		time_container.tm_sec
	);

	return result;
}

int main(int argc, string *argv) {
	// Opening a file to dump the output - the file will be stored along with its
	// executable.
	FILE *file = fopen("usage_logs.txt", "a");
	if (file == NULL)
		printf(
			"\n\nError: Ran into unexpected error while attempting "
			"to open a connection to the log file\n\n"
		);

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
			if (data.encrypt) {
				result = crypt_hill_cipher(
					data.processed_message,
					data.processed_key,
					data.verbose
				);
			} else {
				result = decrypt_hill_cipher(
					data.processed_message,
					data.processed_key,
					data.verbose
				);
			}

			break;

		case RAILFENCE:
			if (data.encrypt)
				result = crypt_railfence(
					data.processed_key,
					data.processed_message,
					data.verbose
				);

			else
				result = decrypt_railfence(
					data.processed_key,
					data.processed_message,
					data.verbose
				);

			break;

		default:
			printf("No state found in the main switch :(");
			exit(-10);
	}

	unsigned int counter = 0;
	unsigned int len_message = strlen(data.cipher_message);

	// Printing the result. Since the original message loses its formatting before being
	// ciphered (spaces being removed, capitals being lowered), undo the appropriate changes
	// while printing the output string.
	printf("\nCipher Result: \n\t");
	for (unsigned int i = 0; result[counter] != '\0'; i++)
		printf("%c",
			   (i < len_message) ?
			   isalpha(data.cipher_message[i]) ?
			   (isupper(data.cipher_message[i]) ? toupper(result[counter++]) : result[counter++]) :
			   data.cipher_message[i] : result[counter++]
		);

	// Logging the results of the current run into the log file.
	if (file != NULL) {
		fprintf(
			file,
			"%s %s \nOriginal Message: %s\nResult: %s\n\n",
			get_date(),
			get_time(),
			data.cipher_message,
			result
		);

		printf("\n\nLogged the result of the current run\n");
	}

	printf("\n\n");

	return 0;
}