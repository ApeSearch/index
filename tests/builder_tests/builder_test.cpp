#include "../../include/libraries/unit_test_framework/include/unit_test_framework/unit_test_framework.h"

#include "../../include/ChunkBuild/ChunkBuild.h"

TEST(test_build) {
    ChunkBuild builder;
    ASSERT_EQUAL(1, 2);
}

TEST_MAIN();