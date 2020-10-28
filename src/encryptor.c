// The main file that will be used to build the base of the project. Will call all the other files/headers and
// use them as needed.

#include <pcre.h>
#include <stdio.h>
#include <commons.h>

#define true 1
#define false 0

typedef short bool;
typedef char *string;

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
	// Checking the compiled pattern against input string and returning the result.
	int result = pcre_exec(
		regex_compile(regex_pattern),  // The compiled pattern against which string is to be checked.
		NULL,                          // No associated structure required.
		input,                        // The string which is to be matched to the compiled pattern.
		(int) strlen(input),          // Basically, the length up to which the pattern is to be checked.
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
char *extract_data(string regex_pattern, string input_string) {
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

	return 0;
}