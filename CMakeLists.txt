# index/CMakeLists.txt

#

cmake_minimum_required(VERSION 3.20.2)
project(index)
enable_testing()

file(GLOB CHUNK_BUILD_SRC src/ChunkBuilder/*.cpp)

# set(variable : value), defining useful stuff for later
set(CMAKE_CXX_STANDARD 17)
# set(CHUNK_BUILD_SRC src/ChunkBuilder/ChunkBuilder.cpp)
set(POSTING_LIST_SRC src/ChunkBuilder/PostingList.cpp)
set(CHUNK_BUILD_H_FILES include/ChunkBuilder/*.h)

set(CHUNK_BUILDER_SRC drivers/chunkBuilder.cpp)

# add the tests subdirectory (also a CMake project, everything is a project)
add_subdirectory(tests bin)

# add_executable(ChunkBuild ${CHUNK_BUILD_SRC})
add_executable(driver ${CHUNK_BUILDER_SRC} ${CHUNK_BUILD_SRC})