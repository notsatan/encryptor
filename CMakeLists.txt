# Won't bother to support older versions of cmake.
cmake_minimum_required(VERSION 3.17.5)

# Define the project and specify that it is C language only - the default C compiler on the system will be used.
project(encryptor C)

# Declaring the project.
add_executable(
    encryptor
    encryptor.c

    # Adding contents from individual header files - and their definitions.
    headers/commons.h
    implementations/commons.c

)

add_definitions(
    -fms-extensions
    -lpcre
)

# Add the folder containing headers to be included in the project.
target_include_directories(
    encryptor
    PRIVATE
    headers
    implementations
)

# Adding individual header files to the list of source files to the current target.
set(
    headers/commons.h
    headers/base.h
)
