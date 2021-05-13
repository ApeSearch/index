#pragma once

#include "../libraries/HashTable/include/HashTable/HashTable.h"

#include "PostingList.h"

struct Chunk {
    Chunk() : dict(), urls(), endDocList(dict.Find("%", new DocEndPostingList())) {}
    Chunk(Chunk* other) {}

    hash::HashTable<std::string, PostingList *> dict;
    std::vector<std::string> urls;
    hash::Tuple<std::string, PostingList *> * endDocList;
};

struct ChunkStats {
    ChunkStats(int _numDoc, size_t _maxLoc, size_t _maxEst) : numDocuments(_numDoc), maxLocation(_maxLoc), maxEstimatedBytes(_maxEst) {}
    int numDocuments;
    size_t maxLocation;
    size_t maxEstimatedBytes;
};

// Class which will build up chunks as documents are added
// Keep track of bytes required as in-memory index grows, have upper bound on index size once optimized()
class ChunkBuilder {
public:
    ChunkBuilder();
    ~ChunkBuilder();
    ChunkBuilder(ChunkBuilder*);

    void addDocument(std::string &url, const std::vector<IndexEntry> &text, const std::vector<AnchorText> &anchorText);
    void resetChunk();
    
    // Returns an object containing stats about the current chunk in the process of being built
    ChunkStats getStats() const {
        return ChunkStats(numDocs, MaximumLocation, upperBoundBytes);
    }

    // Gets a posting list for a key (word). For tests
    PostingList* getPostingList(std::string key) {
        hash::Tuple<std::string, PostingList *> * val = currentChunk->dict.Find(key);
        return val->value;
    }

    hash::Tuple<std::string, PostingList *> * getEndDocList() const { return currentChunk->endDocList; }
    uint32_t BytesRequired();

private:
    size_t addURL(std::string &url, size_t endDocLoc, const std::vector<AnchorText> &anchorText);

    std::unique_ptr<Chunk> currentChunk;
    
    // Trackers for location numbers and number of documents
    size_t LocationsInIndex, MaximumLocation, numDocs;
    size_t uniqueWords;

    // use upperBoundBytes to determine when to write the index chunk to file/call BytesRequired
    size_t upperBoundBytes;

    size_t bytes;
    bool calcBytes;

    // TODO: double check these sizes so upper bound is calculated correctly
    int POST_BYTES_UPPER = 16;
    int ENDDOC_BYTES_UPPER = 8;
    size_t MaxBytesRequired() const { return MaximumLocation * POST_BYTES_UPPER + numDocs * ENDDOC_BYTES_UPPER; }
    size_t AddBytes(int newLocations) const { return newLocations * POST_BYTES_UPPER + ENDDOC_BYTES_UPPER; }
};