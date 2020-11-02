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
bool compare(char *string01, char *string02, const bool case_sensitive) {
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
 * Internal method to create a copy of a string while also specifying the
 * length of the new string.
 *
 * @remarks
 * 		This method is a slight-modified version of `gen_str`, in case edge
 * 		cases it is required to copy a string while leaving space for a few
 * 		extra characters in the new string - this method is intended to be
 * 		used in such scenario(s).
 *
 * @note
 *		If the length required is less than the length of the original
 * 		string, only the first `len-1` characters from the original string will
 * 		be copied over.
 *
 * @note
 * 		A length of `0` indicates that this method will create a new string
 * 		with an exact copy of the original string - without leaving any
 * 		extra space(s).
 *
 * @note
 * 		Caution: New string(s) are created by this method using `malloc`
 * 		internally - as such, they need to be manually destroyed in order to
 * 		avoid memory leaks (or overflows).
 *
 * @param message: String containing the message that is to be copied over.
 * @param len: Unsigned integer indicating the required length of the new string.
 *
 * @return
 * 		String that is a copy of the original string and has the required length.
 */
inline string raw_gen_str(string message, unsigned int len) {
	if (len == 0)
		// If a length of zero is supplied, modifying it to containing the length
		// of the source string + 1 (extra space for string terminator)
		len = strlen(message) + 1;

	// Creating a new string.
	string temp = (string) malloc(len * sizeof(char));

	for (unsigned int i = 0; i < len - 1; i++)
		temp[i] = message[i];

	// Adding the string terminator to the end of the string.
	temp[len] = '\0';
	return temp;
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
 * @note
 * 		This method delegates to `raw_gen_str` method internally. The length
 * 		of the string created will be equal to the source string (+1 for
 * 		string terminator).
 *
 * 	@note
 * 		Any call to this method will result in a call to `malloc`, as such,
 * 		strings returned by this method should be destroyed once they're used
 * 		or it could lead to a potential memory leak.
 *
 * @return
 * 		A string containing a copy of the contents of the original string.
 */
inline string gen_str(string message) {
	return raw_gen_str(message, 0);
}

/**
 * Internal method to create a copy of the source string with additional space to
 * concatenate extra text as needed.
 *
 * @remarks
 * 		Internally delegates to the `raw_str_gen` method - as such uses `malloc`
 * 		to assign space to the new string being created.
 *
 * @note
 * 		Strings created by this method will have to be manually destroyed - or it could
 * 		lead to a potential memory leak.
 *
 * @note
 * 		Strings created by this message will have a total length of `strlen(message)` +
 * 		`pad_length` + 1 (for the string terminator).
 *
 * @param message: Source string that is to be copied over.
 * @param pad_length: Unsigned integer containing the extra size required in the resultant
 * 		string.
 *
 * @return
 * 		String containing a copy of the contents of the original string, and the extra space
 * 		as required.
 */
inline string gen_str_pad(string message, unsigned int pad_length) {
	return raw_gen_str(message, strlen(message) + pad_length);
}


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
