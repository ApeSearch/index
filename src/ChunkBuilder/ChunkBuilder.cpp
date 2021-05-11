#include "../../include/ChunkBuilder/ChunkBuilder.h"

#include <iostream>

ChunkBuilder::ChunkBuilder() : dict(), urls(), LocationsInIndex(0), MaximumLocation(0), numDocs(0), calcBytes(0), bytes(0), uniqueWords(0){}

ChunkBuilder::~ChunkBuilder(){
    // auto vecOfBuckets = dict.vectorOfBuckets();

    // for(size_t i = 0; i < vecOfBuckets.size(); ++i){
    //     for(size_t sameChain = 0; sameChain < vecOfBuckets[i].size(); ++sameChain){
    //         delete vecOfBuckets[i][sameChain]->tuple.value;
    //     }
    // }
} 

void ChunkBuilder::addDocument(std::string url, const std::vector<IndexEntry> &text, const std::vector<AnchorText> &anchorText, size_t endDocLoc) {

}