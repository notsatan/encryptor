// Defines the algorithm to be used for RailFence cipher. In case of railfence cipher,
// the key to be used should be an integer key (unlike the other ciphers).
// In order to accommodate this with as few changes to the rest of the script as possible,
// unlike the rest of the ciphers, this cipher will reveal an additional method in its
// public inter-face to verify the authenticity of the key before-hand.
//
// Apart from this, this cipher method will have to internally convert the string into an
// integer, to be able to use it - using a string to ensure that the rest of the interface
// does not require major structural changes for one cipher.

#include <stdio.h>
#include <math.h>

#include "../headers/ciphers.h"
#include "commons.h"


/**
 * Boolean to indicate if a key has been validated.
 *
 * 		Value read while encrypting and/or decrypting a message.
 * 		Value written while validating the key.
 */
bool rf_key_validated = false;

/**
 * Public interface to validate a key - designed to be used to validate the input while
 * accepting it from the user.
 *
 * @param key: String containing the key that is to be used.
 *
 * @remarks
 * 		To be used once while validating the key if the user chooses to use RailFence
 * 		cipher - used since in case of RailFence cipher, only a numeric key is to be
 * 		accepted.
 *
 * @remarks
 * 		This method will internally terminate the program if the key happens to be invalid.
 * 		If flow-of-control returns back to the calling method, assume that the key is valid.
 *
 * @note
 * 		Since RailFence cipher is different from the existing ciphers (the key should be a number
 * 		instead of a string) - in order to accommodate RailFence cipher with minimal changes to
 * 		the rest of the codebase, revealing this method to validate a key before attempting to
 * 		encrypt/decrypt.
 */
void validate_key_railfence(string key) {
	// Marking the cipher as validated.
	rf_key_validated = true;

	// Using regex pattern to validate the key and return the output of the regex match.
	// The pattern is designed to accept any positive number with a length of one or more
	// characters as a valid key.
	if (!validate("^[1-9]\\d*$", key)) {
		// If the validation fails, i.e. the key entered by the user cannot be used with
		// railfence, rejecting the input force-stop the program.
		printf(
			"\n Error: Invalid key. The key entered `%s` cannot be used with RailFence cipher"
			"\n Please re-enter a valid key.\n",
			key
		);

		exit(-10);
	}
}

/**
 * Converts a string into a number.
 *
 * @param number: String containing the original number.
 *
 * @remarks
 * 		Can only be used with positive integers.
 *
 * @note
 * 		If the string does not contain a number, this method will raise an error.
 *
 * @return
 * 		Unsigned integer containing the number from the string.
 */
extern inline unsigned int convert(string number) {
	// Validating to ensure the string contains a number - force stop if validation fails.
	if (!validate("^\\d+$", number)) {
		printf("\nError: Attempt to convert non-numeric string into a number (Railfence)\n");
		exit(-10);
	}

	// Initializing with a value of zero.
	unsigned int result = 0;

	// Iterate from the end of the loop to the front.
	for (unsigned int i = 0; number[i] != '\0'; i++)
		// Converting the character into an integer and multiplying with a power of 10.
		result = result * pow(10, i) + (number[i] - 48);

	return result;
}

extern inline unsigned int get_length(unsigned int str_len, unsigned int rows) {
	unsigned int row = 0;
	unsigned int column = 0;

	bool dir_down = false;

	unsigned int new_len = 0;

	for (unsigned int i = 0;; i++) {
		if (row == 0 || row == rows - 1)
			dir_down = !dir_down;

		new_len++;
		if (new_len > str_len && row == rows - 1)
			break;

		column++;
		dir_down ? row++ : row--;
	}

	return new_len;
}

/**
 * Method to encrypt a message using the RailFence cipher algorithm.
 *
 * @param key: String containing the key to be used. Should be validated beforehand.
 * @param message: String containing the message to be encrypted.
 * @param verbose: Boolean indicating if verbose output is to be printed.
 *
 * @return
 * 		String containing the encrypted result of the original message - encrypted using
 * 		the railfence cipher algorithm.
 */
string crypt_railfence(string key, string message, bool verbose) {
	// Checking if the key has been validate before. In case this fails, raising an error
	// (Converting a logical bug into runtime error)
	if (!rf_key_validated) {
		printf("\nError: Attempt to encrypt a message without validating the key!\n");
		exit(-10);
	}

	// Converting the string into an integer.
	unsigned int row_count = convert(key);

	// Calculating the current length of the string - and the extra length needed
	// to pad the string such that it fits the diagonal.
	unsigned int message_length = strlen(message);
	unsigned int total_length = get_length(message_length, row_count);

	// Pad the existing string to ensure it contains enough characters.
	if (message_length < total_length) {
		unsigned int extra_len = total_length - message_length;

		// Creating a temporary string of the required length.
		string temp_str = gen_str_pad(message, extra_len);

		// Appending new characters at the end of the temporary string.
		for (unsigned int i = 0; i < extra_len; i++)
			// Padding with an `X` character.
			temp_str[i + message_length] = 'X';

		// Using this temp string as the new message.
		message = temp_str;
	}

	if (verbose)
		// Printing the padded version of the message.
		printf("\nPadded message:\n\t%s\n\n", message);

	// Create a new string to store the result into.
	string result = (string) malloc(total_length * sizeof(char));

	// Modifying each element of the result string to contain string terminator (hack-y approach)
	for (unsigned int i = 0; i < total_length; i++)
		result[i] = '\0';

	// Creating a 2d matrix which will be populated using characters of the message
	char matrix[row_count][total_length];

	printf("Message Length: %d\n", message_length);

	for (unsigned int i = 0; i < row_count; i++)
		for (unsigned int j = 0; j < total_length; j++)
			matrix[i][j] = ' ';

	// Populating the 2d matrix.
	bool dir_down = false;
	unsigned int row = 0;
	unsigned int column = 0;

	for (unsigned int i = 0; i < total_length; i++) {
		if (row == 0 || row == row_count - 1)
			dir_down = !dir_down;

		matrix[row][column++] = message[i];
		dir_down ? row++ : row--;
	}

	// Printing the matrix in verbose mode.
	if (verbose) {
		printf("\n\nMatrix: \n\n");

		for (
			unsigned int i = 0; i < row_count; i++) {
			printf("%c\t", (i == 0) ? '\0' : '\n');

			for (unsigned int j = 0; j < total_length; j++)
				printf("%c\t", matrix[i][j]);
		}
	}


	// Finally, fetching the result as needed.
	unsigned int counter = 0;
	for (unsigned int i = 0; i < row_count; i++)
		for (int j = 0; j < total_length; j++)
			if (matrix[i][j] != ' ')
				result[counter++] = matrix[i][j];


	if (verbose)
		printf("\n\n");

	return
		result;
}
