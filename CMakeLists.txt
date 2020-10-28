# Won't bother to support older versions of cmake.
cmake_minimum_required(VERSION 3.10.2)

# Define the project and specify that it is C language only - the default C compiler on the system will be used.
project(encryptor C)

# Declaring the project.
add_executable(
    encryptor

    # Adding contents from individual header files - and their definitions.
    src/headers/commons.h
    src/implementations/commons.c

    src/headers/ciphers.h
    src/implementations/play_fair.c

    # Adding the main project file as an executable once everything else has been compiled.
    src/encryptor.c
)

# Adding the compile flags in debug mode. Will have to add these compile-time flags to the final release
# mode as well.
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -lpcre -fms-extensions")

# Add the folder containing headers to be included in the project.
target_include_directories(
    encryptor
    PRIVATE
    src/headers
    src/implementations
)

# Adding individual header files to the list of source files to the current target.
set(
    src/headers/commons.h
)
