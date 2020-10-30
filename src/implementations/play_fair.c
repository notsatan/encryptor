
#include "ciphers.h"
#include "commons.h"

#include <string.h>
#include <stdio.h>

// Additional character used to pad a string if the message has odd character count.
#define PAD_CHAR "z"

// The character that is to be ignored from the key matrix being formed.
#define IGNORE_CHAR 'j'

// If `IGNORE_CHAR` is present in the key, should be replaced by this character.
#define REPLACE_CHAR 'i'

// Length of edge of a single side in the key matrix.
#define MATRIX_EDGE 5

// Placeholder string used to define the values being replaced at each iteration
// over the matrix.
# define RULE_MESSAGE "  Replacement String:- \"%c%c\" %s\n"

char key_matrix[MATRIX_EDGE][MATRIX_EDGE];

/**
 * Returns the location of the character in the matrix.
 *
 * @remarks
 * 		The location returned by this method will be as a single integer that
 * 		can be mapped to a 2d matrix of edge `MATRIX_EDGE`.
 *
 * @param c: The character that is to be found in the matrix. Guaranteed to be unique.
 *
 * @return
 * 		An integer that can be mapped mapped to a 2d matrix.
 */
int find_position(char c) {
	if (c == IGNORE_CHAR)
		// If the ignorable character is being searched for, replacing it.
		c = REPLACE_CHAR;

	for (int i = 0; i < MATRIX_EDGE * MATRIX_EDGE; i++)
		if (key_matrix[i / MATRIX_EDGE][i % MATRIX_EDGE] == c)
			return i;


	// Returning negative index to indicate not found.
	return -1;
}

/**
 * Internal function to print the matrix. Defined as an inner-level API,
 * that can either be accessed directly, or using the friendly-function.
 *
 * @param pad_char: String containing characters that are to be used as a padding.
 * @param end_line: String to be printed after the final line of the matrix.
 * 		Ideally, will be one or more new-line characters.
 */
void _print_matrix(string pad_char, string end_line) {
	for (unsigned int i = 0; i < MATRIX_EDGE; i++) {
		// Avoiding printing a new line before the start of the matrix. While
		// ensuring that the first line is actually padded with the character.
		printf("%c%s", (i != 0) ? '\n' : '\0', pad_char);
		for (unsigned int j = 0; j < MATRIX_EDGE; j++)
			printf("%c  ", key_matrix[i][j]);
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
extern inline void print_matrix() {
	_print_matrix("", "\n");
}


/**
 * Convenience method to populate the key matrix using a given key.
 *
 * @remarks
 * 		Note: This method will populate data inside the global matrix, which is why
 * 		it does not need to return anything by itself.
 *
 * @param key: The key used to populate the key matrix.
 */
void populate_key_matrix(string key) {
	// Boolean array - each value represents the alphabet at that index. Used to keep a track
	// of alphabets included in the matrix - ensures no repetitions. Initializing all values as false.
	bool chars[26] = {false};

	// Using this variable to keep a track of the last cell filled in the matrix - will be used to
	// determine the next cell that is to be filled as needed.
	unsigned int matrix_counter = 0;

	// Populating the matrix with the unique elements from the key.
	for (unsigned int i = 0; key[i] != '\0'; i++) {
		if (chars[key[i] - 97])
			// Skip repetitions in the matrix if this character is already in it.
			continue;
		else if (key[i] == IGNORE_CHAR)
			// If the key contains the ignorable character, replacing.
			key[i] = REPLACE_CHAR;

		// Add this key to the appropriate position in the matrix. Mark the character as
		// taken.
		key_matrix[matrix_counter / MATRIX_EDGE][matrix_counter % MATRIX_EDGE] = key[i];
		chars[key[i] - 97] = true;

		// Finally incrementing the counter to directly fill the next cell regardless of
		// how many duplicates occur in the key.
		matrix_counter++;
	}

	// Iterating over all remaining characters, and adding the unique ones to the matrix.
	for (char c = 'a'; c <= (char) 'z'; c++) {
		if (chars[c - 97] || c == IGNORE_CHAR)
			// Skip iteration if the element already exists - or is a `j`.
			continue;

		// If the element doesn't exist, adding it to the matrix and marking this addition.
		key_matrix[matrix_counter / MATRIX_EDGE][matrix_counter % MATRIX_EDGE] = c;
		chars[c - 97] = true;
		matrix_counter++;
	}
}

/**
 * Public method to implement the play-fair cipher algorithm.
 *
 * @param message: String containing the original message that is to be ciphered.
 * 		Should contain only lower-cased alphabets - no other characters.
 * @param key: String containing the key that is to be ciphered.
 * @param noob_friendly: Boolean indicating if verbose output is needed.
 *
 * @return
 * 		String containing a cipher of the original text message.
 */
string crypt_play_fair(string message, string key, bool noob_friendly) {
	// Pad message with an additional character if needed - the result will have an even length.
	message = (strlen(message) % 2 == 0) ? message : strcat(message, PAD_CHAR);

	// Declaring a 2d char array to contain the key matrix, populating it with the key and
	// remaining characters (except `IGNORE_CHAR`).
	populate_key_matrix(key);        // Will internally populate `key_matrix`.

	if (noob_friendly) {
		printf("Key Matrix: \n");
		_print_matrix("\t", "\n\n"); // Padding the matrix with a space.

		printf("Original Message: \n\t`%s`\n\n\n", message);
	}

	// Performing the actual cipher.
	// Taking alphabets from the message, two characters at a time.
	for (int i = 1; message[i] != '\0'; i += 2) {
		printf("PASS %d:\n", (i / 2) + 1);
		char first = message[i - 1];
		char second = message[i];

		if (noob_friendly)
			printf("  Original Sub-string: \"%c%c\"\n", first, second);

		//Finding the location of the two characters in the matrix.
		unsigned int pos_first = find_position(first);
		unsigned int pos_second = find_position(second);

		if ((pos_first % MATRIX_EDGE) == (pos_second % MATRIX_EDGE)) {
			// If both letters are from the same column, taking the character from one row below them.

			if ((pos_first / MATRIX_EDGE) == MATRIX_EDGE - 1)
				// If the element is present in the last row, using the element from the first one,
				// otherwise taking the character from the next row.
				first = key_matrix[0][pos_first % MATRIX_EDGE];
			else
				first = key_matrix[(pos_first / MATRIX_EDGE) + 1][pos_first % MATRIX_EDGE];

			// Doing the same for the second character.
			if ((pos_second / MATRIX_EDGE) == MATRIX_EDGE - 1)
				second = key_matrix[0][pos_second % MATRIX_EDGE];
			else
				second = key_matrix[(pos_second / MATRIX_EDGE) + 1][pos_second % MATRIX_EDGE];

			if (noob_friendly)
				printf(RULE_MESSAGE, first, second, "(Rule-01)");

		} else if ((pos_first / MATRIX_EDGE) == (pos_second / MATRIX_EDGE)) {
			// If both the characters are in the same row, taking the character from the adjacent column.

			if ((pos_first % MATRIX_EDGE) == MATRIX_EDGE - 1)
				// If the element is at the last column, picking the element from the first column.
				first = key_matrix[pos_first / MATRIX_EDGE][0];
			else
				first = key_matrix[pos_first / MATRIX_EDGE][(pos_first % MATRIX_EDGE) + 1];

			if ((pos_second % MATRIX_EDGE) == MATRIX_EDGE - 1)
				second = key_matrix[pos_second / MATRIX_EDGE][0];
			else
				second = key_matrix[pos_second / MATRIX_EDGE][(pos_second % MATRIX_EDGE) + 1];

			if (noob_friendly)
				printf(RULE_MESSAGE, first, second, "(Rule-02)");
		} else {
			// If both the above rules fail, forming a rectangle, and replacing the characters
			// from the diagonally-opposite corner of the matrix.

			// This means that the row for both entries needs to be modified without changing
			// the column.
			first = key_matrix[pos_first / MATRIX_EDGE][pos_second % MATRIX_EDGE];
			second = key_matrix[pos_second / MATRIX_EDGE][pos_first % MATRIX_EDGE];

			if (noob_friendly)
				printf(RULE_MESSAGE, first, second, "(Rule-03)");
		}

		// Adding these characters to the result string.
		message[i - 1] = first;
		message[i] = second;

		printf("  Resultant String; \n\t`%s`\n\n", message);
	}

	return message;
}
