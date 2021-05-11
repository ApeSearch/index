# index/tests/CMakeLists

cmake_minimum_required(VERSION 3.20.2)
project(tests)

# Unit Test Framework Source
set(UNIT_TEST_SRC ../include/libraries/unit_test_framework/src/unit_test_framework.cpp)

set(CHUNK_BUILD_SRC ../src/ChunkBuild/ChunkBuild.cpp)

# Unit Test Sources
FILE(GLOB BUILDER_TESTS_SRC builder_tests/*.cpp)
FILE(GLOB SERIALIZE_TESTS_SRC serialize_tests/*.cpp)

# run 'make UnitTests' to compile these
add_executable(BuilderTest ${BUILDER_TESTS_SRC} ${CHUNK_BUILD_SRC} ${UNIT_TEST_SRC})
add_executable(SerializerTest ${SERIALIZE_TESTS_SRC} ${CHUNK_BUILD_SRC} ${UNIT_TEST_SRC})

add_test(NAME builder_test COMMAND BuilderTest)
add_test(NAME serialize_test COMMAND SerializerTest)
