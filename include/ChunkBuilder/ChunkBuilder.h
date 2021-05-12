#pragma once

#include "../libraries/HashTable/include/HashTable/HashTable.h"

#include "PostingList.h"

struct Chunk {
    Chunk() : dict(), urls(), endDocList(dict.Find("%", new DocEndPostingList())) {}
    hash::HashTable<std::string, PostingList *> dict;
    APESEARCH::vector<std::string> urls;
    hash::Tuple<std::string, PostingList *> * endDocList;
};

// Class which will build up chunks as documents are added
// Keep track of bytes required as in-memory index grows, have upper bound on index size once optimized()
class ChunkBuilder {
public:
    ChunkBuilder();
    ~ChunkBuilder();

    void addDocument(std::string &url, const std::vector<IndexEntry> &text, const std::vector<AnchorText> &anchorText);

private:
    size_t addURL(std::string &url, size_t endDocLoc, const std::vector<AnchorText> &anchorText);

    Chunk currentChunk;
    
    // Trackers for location numbers and number of documents
    size_t LocationsInIndex, MaximumLocation, numDocs;
    size_t uniqueWords;

    // use upperBoundBytes to determine when to write the index chunk to file/call BytesRequired
    size_t upperBoundBytes;

    size_t bytes;
    bool calcBytes;

    int POST_BYTES_UPPER = 16;
    int ENDDOC_BYTES_UPPER = 8;
    size_t MaxBytesRequired() const { return MaximumLocation * POST_BYTES_UPPER + numDocs * ENDDOC_BYTES_UPPER; }
    size_t AddBytes(int newLocations) const { return newLocations * POST_BYTES_UPPER + ENDDOC_BYTES_UPPER; }
};