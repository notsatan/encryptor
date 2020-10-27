// The main file that will be used to build the base of the project. Will call all the other files/headers and
// use them as needed.

#include <stdio.h>

int main(int argc, char **argv) {
    for (unsigned int i = 1; i < argc; i++)
        printf("Argument Entered at index %d: %s \n", i, argv[i]);

    printf("Hello World \n");
    return 0;
}