# Compiling the main file, using external flags to ensure that the file is compiled as needed. Additionally,
# using the `-I` flag to add an additional search path for header files.
#
# Using the `lpcre` flag to use the library to access regex in the program.
gcc encryptor.c -o "result_debug" -fms-extensions -I /src/header -lpcre -I /src/implementations

# Running the output file - also, passing any arguments passed to this bash file to the main file.
./"result_debug" $@