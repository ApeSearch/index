#include "../../include/ChunkBuilder/ChunkBuilder.h"

#include <memory>
#include <iostream>

ChunkBuilder::ChunkBuilder() : currentChunk(new Chunk()), LocationsInIndex(0), MaximumLocation(0), numDocs(0), calcBytes(0),
                                bytes(0), uniqueWords(0), upperBoundBytes(0) {
    }

ChunkBuilder::~ChunkBuilder(){
    // Delete the dictionary buckets here
    auto vecOfBuckets = currentChunk->dict.vectorOfBuckets();

    for(size_t i = 0; i < vecOfBuckets.size(); ++i){
        for(size_t sameChain = 0; sameChain < vecOfBuckets[i].size(); ++sameChain){
            delete vecOfBuckets[i][sameChain]->tuple.value;
        }
    }
} 

ChunkBuilder::ChunkBuilder(ChunkBuilder* other) {
    
}

void ChunkBuilder::resetChunk() {
    currentChunk = std::make_unique<Chunk>();
    MaximumLocation = 0;
    upperBoundBytes = 0;
    numDocs = 0;
}

// Adds a document to 
void ChunkBuilder::addDocument(std::string &url, const std::vector<IndexEntry> &text, const std::vector<AnchorText> &anchorText) {
    // Check that the upper bound of bytes required matches the size of the current index
    // assert(MaxBytesRequired() == upperBoundBytes);

    // stores absolute location of last doc
    // so current tokens can add it to their
    // relative location for determining absolute
    // location in regards to a whole index chunk

    // Add the URL to the index and determine the start Location of this document in the index
    size_t documentStart = addURL(url, text.size(), anchorText);

    auto entry = currentChunk->dict.Find(std::string(":")); // just here so as to not reconstruct in for loop
    for(Location indexLoc = 0; indexLoc < text.size(); ++indexLoc) {
        const std::string &word = text[indexLoc].word;
        if(text[indexLoc].plType == TitleText) {
            const std::string &newWord = '$' + text[indexLoc].word;
            entry = currentChunk->dict.Find(newWord);
        } else {
            entry = currentChunk->dict.Find(word);
        }

        if(!entry){
            entry = currentChunk->dict.Find(text[indexLoc].word, new WordPostingList());
            uniqueWords++;
        }

        WordPostingList * wordList = (WordPostingList *) entry->value;
        wordList->appendToList(indexLoc, static_cast<size_t>(text[indexLoc].attribute), documentStart);
    }

    // Add Anchor text to Index
    for(size_t anchor = 0; anchor < anchorText.size(); ++anchor) {
        const std::string &text = '#' + anchorText[anchor].text;

        entry = currentChunk->dict.Find(text);

        if(!entry)
            entry = currentChunk->dict.Find(text, new AnchorPostingList());
        
        AnchorPostingList * anchorList = (AnchorPostingList * ) entry->value;
        anchorList->appendToList(anchorText[anchor].freq, currentChunk->urls.size() - 1);
    }
}

// Adds a URL to the index, which consists of adding the end doc with an index to map back to the URL into the index 
size_t ChunkBuilder::addURL(std::string &url, size_t endDocLoc, const std::vector<AnchorText> &anchorText) {
    // Add URL to url vector
    currentChunk->urls.emplace_back(url);

    size_t documentStart = (numDocs > 0) ? currentChunk->endDocList->value->posts.back()->loc + 1 : 0;

    // Update Counters
    MaximumLocation = documentStart + endDocLoc; // Keep Track of Maximum Location, location of end of last doc
    LocationsInIndex = endDocLoc + 1; // Add 1 for end doc location + number of tokens in doc
    numDocs++; // Keeps track of the number of documents

    // Calculate the new required bytes upper bound
    upperBoundBytes += AddBytes(endDocLoc + anchorText.size());
    
    DocEndPostingList * docEndList = (DocEndPostingList *) currentChunk->endDocList->value;
    docEndList->appendToList(endDocLoc, currentChunk->urls.size() - 1, documentStart);

    return documentStart;
}

// #include "../../include/libraries/HashTable/include/HashTable/HashBlob.h"

uint32_t ChunkBuilder::BytesRequired() {
    if ( calcBytes )
        return bytes; 
    // uint32_t bytesRequired = sizeof( IndexBlob );
    uint32_t bytesRequired = sizeof( uint32_t ) * currentChunk->dict.table_size( );

    std::vector< std::vector< hash::Bucket< std::string, PostingList*> *> > vec = currentChunk->dict.vectorOfBuckets();

    for(size_t index = 0; index < vec.size(); ++index){
        for(size_t sameChain = 0; sameChain < vec[index].size(); ++sameChain){
           hash::Bucket<std::string, PostingList*> * bucket = vec[index][sameChain];
           bytesRequired += bucket->tuple.value->bytesRequired(bucket->tuple.key);
        }

        // Signifies end of the chained posting lists...
        bytesRequired += sizeof( uint32_t ); 
    }

    // Bytes for the url vector
    for(size_t i = 0; i < currentChunk->urls.size(); ++i)
       bytesRequired += currentChunk->urls[i].size() + 1;

    calcBytes = true;
    bytes = bytesRequired;

    return bytes;
}