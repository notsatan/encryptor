# Using CMake to build the project - Make was another alternative, but decided
# to forgo it in favour of CMake because why not
#
#                   ¯\_(ツ)_/¯

# Won't bother to support older versions of cmake.
cmake_minimum_required(VERSION 3.10.2)

# Define the project and specify that it is C language only - the default C compiler on
# the system will be used.
project(encryptor)

# Declaring the project.
add_executable(
    encryptor

    # Adding contents from individual header files - and their definitions.
    # Apparently, adding the header file before the source file would result
    # in warnings, switching them seems to fix this problem.
    ${PROJECT_SOURCE_DIR}/src/implementations/commons.c
    ${PROJECT_SOURCE_DIR}/src/headers/commons.h

    ${PROJECT_SOURCE_DIR}/src/implementations/play_fair.c
    ${PROJECT_SOURCE_DIR}/src/implementations/hill_cipher.c
    ${PROJECT_SOURCE_DIR}/src/implementations/railfence.c
    ${PROJECT_SOURCE_DIR}/src/headers/ciphers.h

    ${PROJECT_SOURCE_DIR}/src/implementations/data_input.c
    ${PROJECT_SOURCE_DIR}/src/headers/data_input.h

    # Adding the main project file as an executable once everything else has been compiled.
    ${PROJECT_SOURCE_DIR}/src/encryptor.c
)

# Adding the compile flags in all modes. These flags ensure that external libraries such
# as PCRE as available during the build.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_DEBUG} -lpcre -fms-extensions -lm")

# Setting paths to generate binaries, and executables in.
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Add the folder containing headers to be included in the project.
target_include_directories(
    encryptor
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/headers
    ${PROJECT_SOURCE_DIR}/src/implementations
)

# Adding individual header files to the list of source files to the current target.
set(
    ${PROJECT_SOURCE_DIR}/src/headers/commons.h
)
