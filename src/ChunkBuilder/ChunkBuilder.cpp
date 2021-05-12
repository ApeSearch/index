#include "../../include/ChunkBuilder/ChunkBuilder.h"

#include <memory>
#include <iostream>

ChunkBuilder::ChunkBuilder() : currentChunk(), LocationsInIndex(0), MaximumLocation(0), numDocs(0), calcBytes(0), bytes(0), uniqueWords(0), upperBoundBytes(0)
    {

    }

ChunkBuilder::~ChunkBuilder(){
    // Delete the dictionary buckets here

    // auto vecOfBuckets = dict.vectorOfBuckets();

    // for(size_t i = 0; i < vecOfBuckets.size(); ++i){
    //     for(size_t sameChain = 0; sameChain < vecOfBuckets[i].size(); ++sameChain){
    //         delete vecOfBuckets[i][sameChain]->tuple.value;
    //     }
    // }
} 

void ChunkBuilder::addDocument(std::string &url, const std::vector<IndexEntry> &text, const std::vector<AnchorText> &anchorText) {
    // Check that the upper bound of bytes required matches the size of the current index
    assert(MaxBytesRequired() == upperBoundBytes);

    // stores absolute location of last doc
    // so current tokens can add it to their
    // relative location for determining absolute
    // location in regards to a whole index chunk

    // Add the URL to the index and determine the start Location of this document in the index
    size_t documentStart = addURL(url, text.size(), anchorText);

    auto entry = currentChunk.dict.Find(std::string(":")); // just here so as to not reconstruct in for loop
    for(Location indexLoc = 0; indexLoc < text.size(); ++indexLoc) {
        std::string word = text[indexLoc].word;
        if(text[indexLoc].plType == TitleText)
            word = '$' + word;

        entry = currentChunk.dict.Find(word);

        //Different actions for anchorText vs regularText
        if(!entry){
            entry = currentChunk.dict.Find(word, new WordPostingList());
            uniqueWords++;
        }

        WordPostingList * wordList = (WordPostingList *) entry->value;
        wordList->appendToList(indexLoc, static_cast<size_t>(text[indexLoc].attribute), documentStart);
    }

    
    for(size_t anchor = 0; anchor < anchorText.size(); ++anchor) {
        std::string text = anchorText[anchor].text;
        text = '#' + text;

        entry = currentChunk.dict.Find(text);

        if(!entry)
            entry = currentChunk.dict.Find(text, new AnchorPostingList());
        
        AnchorPostingList * anchorList = (AnchorPostingList * ) entry->value;
        anchorList->appendToList(anchorText[anchor].freq, currentChunk.urls.size() - 1);
    }
}

// Adds a URL to the index, which consists of adding the end doc with an index to map back to the URL into the index 
size_t ChunkBuilder::addURL(std::string &url, size_t endDocLoc, const std::vector<AnchorText> &anchorText) {
    // Add URL to url vector
    currentChunk.urls.emplace_back(url);

    // hash::Tuple<std::string, PostingList *> * entry = (currentChunk.endDocList) ? currentChunk.endDocList->value dict.Find("%");
    size_t documentStart = (numDocs > 0) ? currentChunk.endDocList->value->posts.back()->loc + 1 : 0;\

    // Update Counters
    MaximumLocation = documentStart + endDocLoc; // Keep Track of Maximum Location, location of end of last doc
    LocationsInIndex = endDocLoc + 1; // Add 1 for end doc location + number of tokens in doc
    numDocs++; // Keeps track of the number of documents

    // Calculate the new required bytes upper bound
    upperBoundBytes += AddBytes(endDocLoc + anchorText.size());
    
    DocEndPostingList * docEndList = (DocEndPostingList *) currentChunk.endDocList->value;
    docEndList->appendToList(endDocLoc, currentChunk.urls.size() - 1, documentStart);

    return documentStart;
}

// uint32_t IndexHT::BytesRequired() {

    // if(calcBytes)
    //     return bytes;

    // uint32_t bytesRequired = sizeof( IndexBlob );
    // bytesRequired += sizeof( uint32_t ) * dict.table_size( );

    // APESEARCH::vector< APESEARCH::vector< Bucket< APESEARCH::string, PostingList*> *> > vec = dict.vectorOfBuckets();
    // std::cout << "BytesRequired" << std::endl;
    // for(size_t index = 0; index < vec.size(); ++index){
    //     for(size_t sameChain = 0; sameChain < vec[index].size(); ++sameChain){
            
    //         hash::Bucket<APESEARCH::string, PostingList*> * bucket = vec[index][sameChain];
    //         bytesRequired += bucket->tuple.value->bytesRequired(bucket->tuple.key);
    //     }
    //     bytesRequired += sizeof( uint32_t ); // Signifies end of the chained posting lists...
    // }

    // // Bytes for the url vector
    // for(size_t i = 0; i < urls.size(); ++i)
    //     bytesRequired += urls[i].size() + 1;

    // calcBytes = true;
    // bytes = bytesRequired;
    // return 0;
// }