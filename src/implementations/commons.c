// Implementation of the structures defined in the common headers file.

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "../headers/commons.h"

#define true 1
#define false 0

/**
 * Compares two strings and returns their result as a boolean. Can be used for case-insensitive or sensitive
 * comparisons as needed.
 *
 * @param string01: One of the two strings to be compared.
 * @param string02: Another one of the two strings to be compared.
 * @param case_sensitive: Boolean indicating if comparison is to be made with case-sensitivity or not. A value of true
 * indicates case-sensitive comparison.
 *
 * @return
 * 		Boolean indicating if the two strings match or not. True indicates a match.
 */
bool compare(const char *string01, const char *string02, const bool case_sensitive) {
	if (case_sensitive)
		// Using an in-built method if a case-sensitive check is to be made - directly returning the value of the
		// method call.
		return strcmp(string01, string02);

	// If flow-of-control reaches here, assume that the check being made is case-insensitive.
	for (unsigned int i = 0; string01[i] != '\0' && string02[i] != '\0'; i++)
		if (tolower(string01[i]) != tolower(string02[i]))
			// Converting each character to lower case - returning false directly in case a match fails.
			return false;

	return true;
}

/**
 * Compiles a string into a regex pattern - a simple convenience method to compile a
 * regex pattern while internally handling any errors as needed.
 *
 * @param regex_pattern: String containing the regex pattern that is to be compiled.
 *
 * @remarks
 *		Will force-stop the execution of the program in-case the regex pattern fails to
 *		compile - known trigger being placed to reduce the possibility of a runtime bug.

 * @return
 * 		A pointer to the compiled regex pattern. This compiled pattern can then be used
 * 		on a string as needed.
 */
inline pcre *regex_compile(string regex_pattern) {
	const char *error_message;
	int error_offset;

	pcre *compiled_pattern = pcre_compile(
		regex_pattern,
		PCRE_MULTILINE, // Something had to be used and docs are stupid
		&error_message,
		&error_offset,
		0
	);

	if (compiled_pattern == NULL || !compiled_pattern) {
		// Will end up here in case there was an error compiling the pattern.
		// Dumping the logs and force quitting the program in such a case.
		printf(
			"\nERROR: Failed to compile Regex Patter \n\nPattern: %s \nError Offset: %d \nError Message: %s",
			regex_pattern,
			error_offset,
			error_message
		);

		// Force-stop as a fail-safe feature. The application will directly dump logs and force-stop until the pattern
		// is fixed.
		exit(-10);
	}

	return compiled_pattern;
}

/**
 * Method to convert a string to its lower-case equivalent.
 *
 * @param message: String containing the message that is to be converted to lowercase.
 *
 * @return
 * 		A string with the lowercase version of every character in the original message.
 */
string convert_lower(string message) {
	for (int unsigned i = 0; message[i] != '\0'; i++)
		message[i] = (char) tolower(message[i]);

	return message;
}

/**
 * Copies the content of the original string and returns a copy of them
 * in a new string.
 *
 * @param message: Source string. Contents of this string will be copied.
 *
 * @remarks
 * 		Since modifications made to hard-coded string are a part of undefined
 * 		behaviour and can crash the program, this method can be used to get a
 * 		copy of the contents in a new string that can be modified.
 *
 * 		Note: This method will internally create a new pointer and copy -
 * 		in essence, the string returned back is a new pointer. Using this
 * 		method will lead to memory leaks unless the string is cleared off
 * 		the memory once used.
 *
 * @return
 * 		A string containing a copy of the contents of the original string.
 */
inline string gen_str(string message) {
	// Creating temporary string of the required size.
	string temp_message = (string) malloc(strlen(message) * sizeof(char));
	strcpy(temp_message, message);

	return temp_message;
};