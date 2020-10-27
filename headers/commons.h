// A simple header to define all common methods that will be used at various places across the project. This header
// file will contain declarations of all common methods/objects.

#ifndef __encryptor_commons
#define __encryptor_commons

#include <string.h>
#include <stdarg.h>

typedef short bool;

bool compare(const char *, const char *, bool);

extern inline bool str_end(char);

#endif //__encryptor_commons
