#pragma once

#include "../libraries/HashTable/include/HashTable/HashTable.h"

#include "PostingList.h"

// Class which will build up chunks as documents are added
// Keep track of bytes required as in-memory index grows, have upper bound on index size once optimized()
class ChunkBuilder {
public:
    ChunkBuilder();
    ~ChunkBuilder();

    void addDocument(std::string url, const std::vector<IndexEntry> &text, const std::vector<AnchorText> &anchorText, size_t endDocLoc);

private:
    hash::HashTable<std::string, PostingList *> dict;
    APESEARCH::vector<std::string> urls;

    // Trackers for location numbers and number of documents
    size_t LocationsInIndex, MaximumLocation, numDocs;
    size_t uniqueWords;

    size_t bytes;
    bool calcBytes;
};