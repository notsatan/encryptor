
#include "data_input.h"
#include "ciphers.h"


/**
 * Internal method to extract arguments passed to the program from the
 * console-interface.
 *
 * @remarks
 * 		In case of an unrecognized parameter, this method will directly kill the
 * 		program while printing the error.
 *
 * @param this: Pointer to the original object that is to be populated
 * @param count: Integer containing the number of argument(s) that have been passed.
 * @param args: An array of strings containing the strings passed.
 */
void fetch_cli_args(struct user_data *this, unsigned int count, string *args) {
	if (count <= 1) {
		// Force-stop the script in case there is one or less arguments - converting a logical
		// bug into a run-time error.
		printf(
			"\n\nERROR: Found %d arguments while attempting to scrape arguments from "
			"the console.\n\n",
			count
		);

		exit(0);
	}

	// Ignoring the first arguments - this would be the path to the main script.
	for (unsigned int i = 1; i < count; i++) {
		string arg = args[i];
		// Iterating through each argument and validating them based on regex pattern.
		if (validate("^--verbose$", arg)) {
			// Flipping the verbose flag.
			this->verbose = true;
		} else if (validate("^--(?i)encrypt$", arg)) {
			this->encrypt = true;
		} else if (validate("--(?i)decrypt$", arg)) {
			this->encrypt = false;
		} else if (validate("^--message=(([a-zA-Z ]+))$", arg)) {
			// Extracting the contents of the message.
			this->cipher_message = extract_data("^--message=(([A-Za-z ]+))$", arg);
		} else if (validate("^--key=(([A-Za-z ]+|\\d+))$", arg)) {
			// Extracting the key used with the cipher.
			this->cipher_key = extract_data("^--key=(([A-Za-z ]+|\\d+))$", arg);
		} else if (validate("^--cipher=((playfair|hill|railfence))$", arg)) {
			this->cipher = map_cipher(extract_data("^--cipher=((.*))$", arg));

			if (this->cipher == UNDEFINED) {
				// If a cipher type could not be detected, throwing an error.
				printf("\nError: Undefined cipher type detected.\n");
				exit(-10);
			}
		} else {
			// Direct exit with an error if the parameter passed cannot be recognized.
			printf("\n\nError: Unexpected argument detected `%s`\n", arg);

			exit(-10);
		}
	}
}

/**
 * Populates all the internal fields of the structure with their initial values as needed.
 *
 * @notes
 * 		This method is required since C does not allow differentiating between initialized
 * 		and non-initialized values. This becomes a problem when such a check is to be made.
 * 		This method will internally assign (fixed) sentinel values to each variable - these
 * 		sentinel values can then be used with the structure to check if a variable has
 * 		actually been initialized or not.
 *
 * @remarks
 * 		Any changes being made to the initial (sentinel) values of the variable inside this
 * 		method will also need to be reflected in the rest of the program.
 *
 * @param this: A pointer to the structure.
 */
void initialize(struct user_data *this) {
	// Start by setting all parameters to be null.
	this->cipher_message = NULL;
	this->cipher_key = NULL;

	// Random values for specific types.
	this->encrypt = -1;
	this->cipher = -2;
	this->verbose = -1;

	this->processed_key = NULL;
	this->processed_message = NULL;
}

/**
 * The main method to implement the interactive mode to fetch data from the user.
 *
 * @note
 * 		This method is to be kept in sync with `initialize` any changes made in
 * 		this method *might* require some corresponding changes to be made in
 * 		`initialize` and vice-versa.
 *
 * @param this: Pointer to the structure.
 * @param cli_used: Boolean indicating if any argument has been passed through console
 * 	or not. If set to true, then only the essential values will be requested from the
 * 	user, as opposed to requesting for all values.
 */
void interactive(struct user_data *this, bool cli_used) {
	// If the data present in a variable is not the same as the original, assuming that
	// the data has been provided by the user, and skipping asking for the same.

	// Alternatively, if command-line mode is not used or if value of a variable is the
	// same as initialization - having the user enter the value in interactive mode.
	// Should be separate `if` blocks instead of an if-else ladder.

	if (!cli_used || this->cipher == -2) {
		// Creating a new string to take input from the user.
		string temp_str = new_str(STRING_SMALL);

		// An infinite loop - break out only when correct input is detected. Can be emulated
		// with a goto statement too - avoiding to prevent cluttering the global namespace.
		while (true) {
			printf("\n\nCipher technique to be used (playfair/hill/railfence)");
			printf("\ncipher> ");

			// Scanning the string.
			scan_str(temp_str, STRING_SMALL);

			// Validating the value of the string - using regex for readability.
			if (validate("^((playfair|hill|railfence))$", temp_str)) {
				// The flow-of-control reaches here only when the user picks up a valid cipher type.
				// Mapping the cipher type to an enum to store it.
				this->cipher = map_cipher(temp_str);

				if (this->cipher == UNDEFINED)
					// If the cipher cannot be mapped accurately, jumping out of this statement,
					// printing an error message and continuing with next iteration of the loop.
					goto cipher_map_error;

				// Breaking out of the infinite loop if the string matches.
				break;
			} else {
				// Label to directly jump to printing an error message and continue with the
				// infinite loop - should be used *exclusively* in the if block above.
				cipher_map_error:

				// Printing an error message if the match fails.
				printf("Error: Unknown value `%s`\n", temp_str);
			}
		}

		// Clearing temporary string - good practice.
		free(temp_str);
	}

	if (!cli_used || this->cipher_key == NULL) {
		// Creating a new string - the variable was initialized as null
		this->cipher_key = new_str(STRING_MEDIUM);

		// Infinite loop to reject invalid input.
		while (true) {
			printf("\n\nKey to be used in the cipher (%d alphabets at max)", STRING_MEDIUM);
			printf("\nkey> ");

			scan_str(this->cipher_key, STRING_MEDIUM);

			if (validate("^(([a-zA-Z ]+|\\d+))$", this->cipher_key))
				// Break out of the infinite loop - pure numeric key used in Railfence.
				break;
			else
				// Print an error message - continue with the next iteration of the (infinite) loop.
				printf("Error: Invalid key. The key should consist of only alphabets\n");
		}
	}

	// Handling an edge-case check! In case of railfence cipher, the is supposed to be numeric.
	if (this->cipher == RAILFENCE)
		// Attempting to validate the key - if validation fails, the method-call will internally
		// terminate the program. If the flow-of-control returns back, assume that the key
		// is valid.
		validate_key_railfence(this->cipher_key);

	if (!cli_used || this->cipher_message == NULL) {
		// Creating a string - was initialized as null.
		this->cipher_message = new_str(STRING_LARGE);

		// Infinite loop to reject invalid input.
		while (true) {
			printf("\n\nMessage that is to be ciphered (alphabets only)");
			printf("\nmessage> ");

			scan_str(this->cipher_message, STRING_LARGE);
			if (validate("^[A-Za-z ]+$", this->cipher_message))
				// Break out of the loop if the input is valid.
				break;
			else
				// Print an error message - continue with an iteration of the loop.
				printf("Error: Invalid message. Should consist of alphabets only.\n");
		}
	}

	// A special case. The verbose flag is optional, and shall default to false. If the user
	// does not provide verbose flag through console, defaulting its value to false.
	if (!cli_used && this->verbose == -1) {
		string temp_input = new_str(STRING_SMALL);

		while (true) {
			printf("\n\nUse verbose mode (yes/no)?");
			printf("\nverbose> ");

			scan_str(temp_input, STRING_SMALL);

			// Validating the string input - case independent.
			if (validate("^(?i)((yes|no|true|false|y|n))$", temp_input)) {
				// Mapping the input to a boolean value.
				this->verbose = (
									l_compare(temp_input, "yes") ||
									l_compare(temp_input, "true") ||
									l_compare(temp_input, "y")
								) ? true : false;

				// Breaking out of the infinite loop.
				break;
			} else {
				printf("Error: Unexpected answer `%s`", temp_input);
			}
		}

		// Deleting the temporary string.
		free(temp_input);
	} else if (!cli_used || this->verbose == -1) {
		this->verbose = false;
	}

	if (!cli_used || this->encrypt == -1) {
		string cipher_val = new_str(STRING_SMALL);

		while (true) {
			printf("\n\nEncrypt the message (yes/no)?");
			printf("\nencrypt/decrypt> ");

			scan_str(cipher_val, STRING_SMALL);
			if (validate("^(?i)((yes|no|true|false|y|n))$", cipher_val)) {
				// Mapping the value to a boolean.
				this->encrypt = (
									l_compare(cipher_val, "true") ||
									l_compare(cipher_val, "yes") ||
									l_compare(cipher_val, "y")
								) ? true : false;

				// Break out of the infinite loop
				break;
			} else {
				printf("Error: Invalid input: `%s`\n", cipher_val);
			}
		}

		free(cipher_val);
	}
}

/**
 * Creates a modified copy of the first string in the second string.
 *
 * @remarks
 * 		The second string *should* be null - or this method will raise an error.
 *
 * @remarks
 * 		Designed to modify the input arguments to suit the needs of the cipher
 * 		techniques - performs background chores such as removing spaces, changing
 * 		case and more to the result string. Doesn't modify the source string.
 *
 * @param source: Source string. Should not be null.
 *
 * @return
 * 		The destination string after modification that is a mutated version of the
 * 		source string.
 */
extern inline string mutate(string source) {

	// Raise an error if any
	if (source == NULL || strlen(source) == 0) {
		printf("\n\n\tERROR: Null source detected during mutation in data-input\n");
		exit(-10);
	}

	unsigned int counter = 0;
	unsigned int source_len = strlen(source);

	// Creating a destination string of required length.
	string dest = new_str(source_len * sizeof(char));

	for (unsigned int i = 0; i < source_len; i++)
		if (isalpha(source[i]))
			dest[counter++] = tolower(source[i]);

	return dest;
}

/**
 *	Populates data into the structure variables as needed.
 *
 *	@remarks
 *		This method internally falls back to calling various internal
 *		methods - starts off by populating the variables with (fake)
 *		sentinel values, then attempt to extract values for the variables
 *		from the command-line.
 *
 *	@remarks
 *		If even after this, any compulsory variable remains without a value,
 *		this method will switch to interactive mode and force the user to
 *		enter a value for the variable.
 *
 * @param this: Pointer to the structure that is to be modified.
 * @param arg_count: Integer containing a count of the arguments passed from console
 * @param argv: String array with each string being an argument passed through the console.
 */
void populate_data(struct user_data *this, int arg_count, string *argv) {
	// Starting by initializing all the values present in the structure.
	initialize(this);

	// Boolean to indicate if command-line arguments have been used by the user.
	bool cli_used = false;

	// Fetching all the parameters passed in to the script if more than a
	// single argument is detected.
	if (arg_count > 1) {
		cli_used = true;
		fetch_cli_args(this, arg_count, argv);
	}

	// Running the interactive session regardless of whether console line provided arguments
	// This method will ask for the missing values in-case console arguments have been provided
	// or, other, ask the user for each value iteratively.
	interactive(this, cli_used);

	// Once all the argument(s) have their required values, modifying them to suit conditions
	// includes converting characters to lower-case, and stripping off spaces and more.

	if (this->cipher == PLAYFAIR || this->cipher == HILL_CIPHER) {
		// Creating mutated copies of the original values - devoid of non-alphabetical
		// characters as well as spaces and numbers - this is what will be used in case
		// of playfair and hill cipher - they cannot work with different cases and/or
		// spaces being involved in the source(s).
		this->processed_message = mutate(this->cipher_message);
		this->processed_key = mutate(this->cipher_key);
	} else if (this->cipher == RAILFENCE) {
		// RailFence can work with capitalization and/or spaces in between source(s),
		// creating a copy of the original strings in this case.
		this->processed_message = mutate(this->cipher_message);
		this->processed_key = gen_str(this->cipher_key);
	}
}
