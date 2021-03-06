/**
 * Source file defining the Hill Cipher algorithm being used. This cipher
 * supports lower-case alphabets only. Space and/or any other character
 * needs to be stripped out of the contents before-hand.
 *
 * Notes:
 *  Regarding the stupid naming scheme used in this source file, since
 *  the implementations of the various ciphers are linked to a single
 *  header file - and all these source files are populating the same
 *  header. As a result, all these source files are linked/attached
 *  to the same header and so, names of functions/objects can't be
 *  overlapped - really missing the syntactic sugar that C++ offers,
 *  namespaces for one :/
 */

#include <stdio.h>

#include "commons.h"
#include "ciphers.h"

// The size of the matrix - can alternatively be though of as the graph
// to use.
//
// Example; A size of three, will form a `trigraph` and use 3x3 matrix.
#define MATRIX_SIZE 3

// The base number that is used to calculate the modulo while formulating
// the final result in the matrix.
#define BASE_MOD 26

// The character that is to be used to pad a string in-case length of the
// string is shorter that expected.
#define PAD_NULL 'x'

// Defining the key matrix that is to be used by this cipher algorithm.
// Global because doesn't matter since the program can only run a single
// cipher during its lifetime.
char hc_key_matrix[MATRIX_SIZE][MATRIX_SIZE];

/**
 * Populates the key matrix being used in this cipher algorithm. The string
 * key supplied will be used to populate the key matrix.
 *
 * @remarks
 * 	If the key string contains more alphabets than needed in the matrix, only
 * 	the first `n` elements will be used.
 *
 * 	If the key string is not long enough to populate the key matrix by itself,
 * 	normal alphabets will be used after the key string to populate the matrix.
 *
 * @param key: String containing the key used to populate the matrix with value.
 */
void hc_populate_key(string key) {
	// Using the original key if it is long enough to populate the key matrix,
	// if not, filling the rest of the space with alphabetical characters.
	unsigned int string_length = strlen(key);
	unsigned int counter = 0;
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++)
		hc_key_matrix[i / MATRIX_SIZE][i % MATRIX_SIZE] =
			(i < string_length) ? key[i] : (char) counter++ + 97;
}

int mod(int a, int b) {
	int r = a % b;
	return r < 0 ? r + b : r;
}

/**
 *
 *
 * @param key
 */
void hc_populate_inverse(string key) {
	// Generating the key matrix for the algorithm - this matrix will then
	// be inverted for the inverse matrix.
	hc_populate_key(key);

	// Creating a temporary matrix as the augmented matrix.
	int augmented_matrix[MATRIX_SIZE][MATRIX_SIZE];

	float determinant = 0;
	for (int i = 0; i < 3; i++)
		determinant = determinant + (
			(hc_key_matrix[0][i] - 97) * (
				(hc_key_matrix[1][(i + 1) % 3] - 97) *
				(hc_key_matrix[2][(i + 2) % 3] - 97) -
				(hc_key_matrix[1][(i + 2) % 3] - 97) *
				(hc_key_matrix[2][(i + 1) % 3] - 97)
			)
		);

	// Getting the result and the multiplicative inverse at once.
	int result = (int) determinant % 26;
	int multi_inverse = 0;
	while ((multi_inverse * result) % 26 != 1)
		multi_inverse++;

	// Populating augmented matrix with initial set of values - will contain
	// the co-factor matrix by the end of this step.
	bool negative = false;
	for (unsigned int row = 0; row < MATRIX_SIZE; row++) {
		for (unsigned int column = 0; column < MATRIX_SIZE; column++) {
			int first = 1;
			int second = 1;

			unsigned int first_row = 0;
			for (unsigned int i = 0; i < MATRIX_SIZE; i++)
				for (unsigned int j = 0; j < MATRIX_SIZE; j++)
					if (i != row && j != column) {
						if (first_row == 0 || first_row == 3)
							first *= hc_key_matrix[i][j] - 97;
						else
							second *= hc_key_matrix[i][j] - 97;

						first_row++;
					}

			augmented_matrix[row][column] = ((negative) ? -1 : 1) * (first - second);
			negative = !negative;
		}
	}

	// Converting the co-factor matrix into transpose - to get adjoint matrix.
	for (unsigned int i = 0; i < MATRIX_SIZE; i++)
		for (unsigned int j = 0; j < MATRIX_SIZE; j++)
			if (i <= j) {
				int temp = mod(augmented_matrix[i][j], 26);

				augmented_matrix[i][j] = mod(augmented_matrix[j][i], 26);
				augmented_matrix[j][i] = temp;
			}

	// Multiplying by the multiplicative inverse, and storing the result in the
	// original matrix, from where it will be used to get the result.
	for (unsigned int i = 0; i < MATRIX_SIZE; i++)
		for (unsigned int j = 0; j < MATRIX_SIZE; j++)
			hc_key_matrix[i][j] = mod(augmented_matrix[i][j] * multi_inverse, 26) + 97;
}

/**
 * Internal function to print the matrix. Defined as an inner-level API,
 * that can either be accessed directly, or using the friendly-function.
 *
 * @param pad_char: String containing characters that are to be used as a padding.
 * @param end_line: String to be printed after the final line of the matrix.
 * 		Ideally, will be one or more new-line characters.
 */
void _hc_print_key(string pad_char, string end_line) {
	for (unsigned int i = 0; i < MATRIX_SIZE; i++) {
		// Avoiding printing a new line before the start of the matrix. While
		// ensuring that the first line is actually padded with the character.
		printf("%c%s", (i != 0) ? '\n' : '\0', pad_char);
		for (unsigned int j = 0; j < MATRIX_SIZE; j++)
			printf("%c  ", hc_key_matrix[i][j]);
	}

	// Printing the end-line character.
	printf("%s", end_line);
}

/**
 * Just a convenience method to be able to print the 2d matrix as needed. Exists
 * to complement the debugger, and have a graphical representation of the ciphers.
 *
 * @remarks
 * 		A friendly-method of the actual internal method - to conveniently print the
 * 		matrix with zero padding, and pointing the cursor to the next line below the
 * 		matrix. The internal method can be directly accessed in case custom
 * 		padding/end-line character is needed.
 */
extern inline void hc_print_key() {
	_hc_print_key("", "\n");
}

/**
 * Convenience method to simply map a character to its integer position
 * as required by this cipher. For example, `a` maps to 0 and so on.
 *
 * @param c: The character that is to be mapped.
 *
 * @return
 * 		Unsigned integer character indicating the mapped value of the character.
 */
extern inline unsigned int map(char c) {
	if ((int) c < 97) {
		// Raise an error if an invalid mapping is attempted - reduces the possibility
		// of logical bugs, instead converts them into runtime errors.
		printf("Error: Attempt to map character `%c` in Hill Cipher.\n", c);
		exit(-10);
	}

	return (int) c - 97;
}

/**
 * The reverse of the `map` method - reverses the mapping - takes an integer
 * input and maps it back to a character.
 *
 * @param i: Integer value that maps to a character - preferably generated by `map(char)`.
 *
 * @return
 * 		Character containing the char that the integer value maps to.
 */
extern inline char rev_map(int i) {
	if (i > 26) {
		// Raise an error if an invalid mapping is attempted - reduces the possibility
		// of logical bugs, instead converts them into runtime errors.
		printf("Error: Attempt to reverse map integer `%d` in Hill Cipher.\n", i);
		exit(-10);
	}

	return (char) i + 97;
}

/**
 * A customized version of the `_hc_print_key` method to suit the particular
 * demands of this cipher algorithm. Prints the key matrix, the matrix being
 * generated by this method in an intermediate step, and the result of matrix
 * multiplication.
 *
 * @param multiplier: String containing the key map, i.e. the matrix being multiplied
 * 		to the key matrix.
 * @param result: String containing the result, i.e. result of the multiplication
 * 		between the key matrix and the key map.
 * @param padding: String used to pad an individual row of the matrix, will be
 * 		pre-appended to each row of the matrix.
 * @param end_line: String printed at the end of the matrix. Warning: If set to null,
 * 		the cursor will be present at the end of matrix (won't be moved to next line).
 */
void hc_current_mapping(string multiplier, string result, const_str padding, const_str end_line) {
	bool mid_line_found = false;
	for (unsigned int i = 0; i < MATRIX_SIZE; i++) {
		// Printing on a new line if this isn't the first row of the matrix.
		printf("%c%s", (i == 0) ? '\0' : '\n', padding);
		for (unsigned int j = 0; j < MATRIX_SIZE; j++)
			printf(
				// The first character will be the alphabet being multiplied, and the string following it
				// will be the padding as needed (removed in case of last column).
				"%c%s",
				hc_key_matrix[i][j],
				(j + 1 == MATRIX_SIZE) ? "" : "  "
			);

		// Separating the matrices with space, printing a multiplication/equality
		// sign between the two if the current line is a mid-line.
		if (i >= MATRIX_SIZE / 2 && !mid_line_found) {
			mid_line_found = true;
			printf("%s%c%s%c", "   x   ", multiplier[i], "   =   ", result[i]);
		} else {
			printf("%s%c%s%c", "       ", multiplier[i], "       ", result[i]);
		}
	}

	printf("%s", end_line);
}

/**
 * Public method to implement the Hill Cipher algorithm to encrypt text.
 *
 * @param message: String containing the message to be encrypted.
 * @param key: String containing the message to be used as a key.
 * @param verbose: Boolean indicating if verbose mode is to be used.
 *
 * @return
 * 		A string containing the encrypted version of the original text message.
 * 		Will be devoid of all spaces, can be mapped to the input string to
 * 		be able to add back spaces as needed.
 */
string crypt_hill_cipher(string message, string key, bool verbose) {
	// Generating the key matrix.
	hc_populate_key(key);

	if (verbose) {
		printf("\nKey Matrix:\n");
		_hc_print_key("\t", "\n\n");
		printf("Original Message: \n\t`%s`\n", message);
	}

	// Temporary string(s) to hold `n` characters in the string at the time.
	string temp = (string) malloc(MATRIX_SIZE * sizeof(char));
	string temp_result = (string) malloc(MATRIX_SIZE * sizeof(char));

	// Calculating the length of the original message, and of the result - they might
	// not always match. If the message length is not a multiple of `MATRIX_SIZE`, it
	// will be padded with extra characters to make it fit.
	unsigned int message_length = strlen(message);
	unsigned int result_length = message_length + (message_length % MATRIX_SIZE);

	// The result string - will be used to contain the result as it is being generated.
	string result = (string) malloc(result_length * sizeof(char));

	// Starting a loop to iterate between every `MATRIX_SIZE` elements. If a
	// tri-graph is selected for example, iterating between every three elements.
	for (unsigned int i = 0; i < result_length; i += MATRIX_SIZE) {
		for (unsigned int counter = 0; counter < MATRIX_SIZE; counter++)
			// Picking up the first `n` characters from the current position - if the
			// message has ran out of characters, padding with null character.
			temp[counter] = (i + counter < message_length) ? message[i + counter] : PAD_NULL;

		// Matrix multiplication - treat the contents of the temp string as a matrix, and perform
		// multiplication with the key matrix.
		for (unsigned int j = 0; j < MATRIX_SIZE; j++) {
			unsigned int val = 0;

			for (unsigned int k = 0; k < MATRIX_SIZE; k++)
				val += map(hc_key_matrix[j][k]) * map(temp[k]);

			// Adding the results to the temp result string - to make sure that the contents of this
			// multiplication can be printed in verbose mode. Calculating the modulus using the macro.
			temp_result[j] = rev_map(val % BASE_MOD);
		}

		strcat(result, temp_result);

		if (verbose) {
			printf("\n\nIteration %d:\n", (i / 3) + 1);

			hc_current_mapping(
				temp,
				temp_result,
				"\t",
				"\n\n"
			);

			printf("Current Result: \n\t`%s`\n", result);
		}
	}

	// Freeing up space from the temporary string - just a good practice.
	free(temp);

	return gen_str(result);
}

string decrypt_hill_cipher(string message, string key, bool verbose) {
	// Generating the key matrix.
	hc_populate_inverse(key);

	if (verbose) {
		printf("\nKey Matrix:\n");
		_hc_print_key("\t", "\n\n");
		printf("Original Message: \n\t`%s`\n", message);
	}

	// Temporary string(s) to hold `n` characters in the string at the time.
	string temp = (string) malloc(MATRIX_SIZE * sizeof(char));
	string temp_result = (string) malloc(MATRIX_SIZE * sizeof(char));

	// Calculating the length of the original message, and of the result - they might
	// not always match. If the message length is not a multiple of `MATRIX_SIZE`, it
	// will be padded with extra characters to make it fit.
	unsigned int message_length = strlen(message);
	unsigned int result_length = message_length + (message_length % MATRIX_SIZE);

	// The result string - will be used to contain the result as it is being generated.
	string result = (string) malloc(result_length * sizeof(char));

	// Starting a loop to iterate between every `MATRIX_SIZE` elements. If a
	// tri-graph is selected for example, iterating between every three elements.
	for (unsigned int i = 0; i < result_length; i += MATRIX_SIZE) {
		for (unsigned int counter = 0; counter < MATRIX_SIZE; counter++)
			// Picking up the first `n` characters from the current position - if the
			// message has ran out of characters, padding with null character.
			temp[counter] = (i + counter < message_length) ? message[i + counter] : PAD_NULL;

		// Matrix multiplication - treat the contents of the temp string as a matrix, and perform
		// multiplication with the key matrix.
		for (unsigned int j = 0; j < MATRIX_SIZE; j++) {
			unsigned int val = 0;

			for (unsigned int k = 0; k < MATRIX_SIZE; k++)
				val += map(hc_key_matrix[j][k]) * map(temp[k]);

			// Adding the results to the temp result string - to make sure that the contents of this
			// multiplication can be printed in verbose mode. Calculating the modulus using the macro.
			temp_result[j] = rev_map(val % BASE_MOD);
		}

		strcat(result, temp_result);

		if (verbose) {
			printf("\n\nIteration %d:\n", (i / 3) + 1);

			hc_current_mapping(
				temp,
				temp_result,
				"\t",
				"\n\n"
			);

			printf("Current Result: \n\t`%s`\n", result);
		}
	}

	// Freeing up space from the temporary string - just a good practice.
	free(temp);

	return gen_str(result);
}
