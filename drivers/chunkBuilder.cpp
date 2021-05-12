#include "../include/ChunkBuilder/ChunkBuilder.h"

std::vector<IndexEntry> document1 = {
    {"pig", WordAttributeNormal, BodyText}, 
    {"the", WordAttributeNormal, BodyText}, 
    {"cow", WordAttributeNormal, BodyText}, 
    {"the", WordAttributeNormal, BodyText}, 
    {"pig", WordAttributeNormal, BodyText}, 
    {"and", WordAttributeNormal, BodyText}, 
    {"all", WordAttributeNormal, BodyText}, 
    {"of", WordAttributeNormal, BodyText}, 
    {"the", WordAttributeNormal, BodyText}, 
    {"animals", WordAttributeNormal, BodyText},
};

std::vector<IndexEntry> document2 = {
    {"this", WordAttributeNormal, BodyText}, 
    {"is", WordAttributeNormal, BodyText}, 
    {"a", WordAttributeNormal, BodyText}, 
    {"and", WordAttributeNormal, BodyText}, 
    {"test", WordAttributeNormal, BodyText}
};

std::vector<std::string> urls = {
    "https://eecs440.com",
    "https://eecs441.com"
};

std::unique_ptr<ChunkBuilder> buildIndex(bool print) {

    char const *filename = "./tests/indexChunk.ape";
    std::unique_ptr<ChunkBuilder> index(new ChunkBuilder());

    std::vector<AnchorText> aText;
    index->addDocument(urls[0], document1, aText);
    index->addDocument(urls[1], document2, aText);

    // IndexFile hashFile( filename, index.get() );
    return index;
}

int main() {
    buildIndex(false);
}