# Index
Repository for Index Building structured using CMake

git submodule update...... yep 

1. Install CMake 3.20.2 (checkout index/docs/README.md
2. If there isn't a build folder in the top-level repository, make one 
3. cd into the build directory (should be "${GIT_REPO_DIR}/build")
4. run 'cmake ..'
5. run 'make check' to build and run unit tests

These are executable defined first in the 'index/tests/CMakeLists.txt', the passed as a subdirectory in the 'index/CMakeLists.txt', to be compiled into the index/build/bin directory
(index/build/bin because build is where CMake is generally run from) 
