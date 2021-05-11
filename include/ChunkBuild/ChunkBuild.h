#pragma once

// Class which will build up chunks as documents are added
// Keep track of bytes required as in-memory index grows, have upper bound on index size once optimized()
class ChunkBuild {
public:
    ChunkBuild();

private:
};