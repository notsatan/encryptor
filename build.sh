# A simple shell script to build the project and generate an executable using
# CMake. Will simply call the relevant CMake commands to build the project.

# Configurations inside `CMakeLists.txt` ensure that the built executable is
# placed at the right path - and includes the appropriate headers/libraries.

cmake -H. -Bbuild
cmake --build build -- -j3