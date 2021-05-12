#include "../../include/ChunkBuilder/PostingList.h"

void DocEndPostingList::appendToList(Location loc_, size_t urlIndex, size_t lastDocIndex){
    if(posts.size())
        loc_ += lastDocIndex;
    posts.push_back(new EODPost(loc_, urlIndex));

}

void WordPostingList::appendToList(Location loc_, size_t attribute, size_t lastDocIndex){
    loc_ += lastDocIndex;
    posts.push_back(new WordPost(loc_, attribute));
}

void AnchorPostingList::appendToList(Location freq, size_t urlIndex, size_t lastDocIndex){
    posts.push_back(new AnchorPost(freq, urlIndex));
}

uint32_t WordPostingList::bytesRequired(const std::string &key) {

    // if(calcBytes)
    //     return bytesList;
    // uint32_t numBytes = key.size() + 1;
    // Location absoluteLocation = 0;
    // numBytes += sizeof( SerializedPostingList );


    // for(size_t i = 0; i < posts.size(); ++i) {
    //     WordPost* wp = (WordPost * ) posts[i];

    //     Location temp = absoluteLocation;
    //     absoluteLocation = wp->loc;
    //     std::vector<uint8_t> bytes = encodeDelta(wp->loc - temp);

    //     for(size_t byte = 0; byte < bytes.size(); ++byte)
    //         deltas.push_back(bytes[byte]);

    //     //std::cout << decodeDelta(bytes) << std::endl;

    //     //encode attributes
    //     //deltas.push_back(static_cast<uint8_t>(wp->tData));
    // }

    // numBytes += deltas.size();
    // calcBytes = true;
    // bytesList = numBytes;

    return 0;
}


uint32_t DocEndPostingList::bytesRequired(const std::string &key) { //implement

    // if(calcBytes)
    //     return bytesList;

    // uint32_t numBytes = key.size() + 1;
    // Location absoluteLocation = 0;
    // numBytes += sizeof( SerializedPostingList );

    // for(size_t i = 0; i < posts.size(); ++i) {
    //     EODPost* docEndp = (EODPost * ) posts[i];

    //     Location temp = absoluteLocation;
    //     absoluteLocation = docEndp->loc;
    //     std::vector<uint8_t> bytes = encodeDelta(docEndp->loc - temp);

    //     for(size_t byte = 0; byte < bytes.size(); ++byte)
    //         deltas.push_back(bytes[byte]);
        
    //     bytes = encodeDelta(docEndp->tData);

    //     for(size_t byte = 0; byte < bytes.size(); ++byte)
    //         deltas.push_back(bytes[byte]);
    // }

    // numBytes += deltas.size();
    // calcBytes = true;
    // bytesList = numBytes;

    return 0;
}

uint32_t AnchorPostingList::bytesRequired( const std::string &key ) {
    // if(calcBytes)
    //     return bytesList;

    // //std::cout << "BR: " << key << std::endl;

    // uint32_t numBytes = key.size() + 1;
    // numBytes += sizeof( SerializedAnchorText );

    // for(size_t i = 0; i < posts.size(); ++i) {
    //     AnchorPost* aPost = (AnchorPost * ) posts[i];

    //     std::vector<uint8_t> bytes = encodeDelta(aPost->loc); //loc refers to freq

    //     for(size_t byte = 0; byte < bytes.size(); ++byte)
    //         deltas.push_back(bytes[byte]);
        
    //     bytes = encodeDelta(aPost->tData);

    //     for(size_t byte = 0; byte < bytes.size(); ++byte)
    //         deltas.push_back(bytes[byte]);
    // }

    // numBytes += deltas.size();
    // calcBytes = true;
    // bytesList = numBytes;

    return 0;
}

// For Testing Purposes only
Post *PostingList::Seek(Location l) {
    Location index = 0;
    for(; index < posts.size() && (l > posts[index]->loc && posts[index]->loc != l); ++index);

    if(index == posts.size())
        return nullptr;

    return posts[index];
}