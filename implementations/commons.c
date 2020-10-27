// Implementation of the structures defined in the common headers file.

#include "../headers/commons.h"
#include <ctype.h>
#include <string.h>

#define true 1
#define false 0

typedef short bool;

/**
 * An inline method made to replace the `strlen` call being made in loops - returns the result of a simple comparison
 * instead of calling a method that traverses the entire string to calculate its length.
 *
 * @param c - The current character being read from the string. Will be used to check if this character is at the end
 * of the string or not.
 *
 * @return
 * 		Boolean value indicating if the character is placed at the end of the string or not. A value of true indicates
 * 		that the character is placed at the end of the string.
 */
inline bool str_end(char c) {
	return c == '\0';
}

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
	for (unsigned int i = 0; !str_end(string01[i]) && !str_end(string02[i]); i++)
		if (tolower(string01[i]) != tolower(string02[i]))
			// Converting each character to lower case - returning false directly in case a match fails.
			return false;

	return true;
}