#include "../../include/libraries/unit_test_framework/include/unit_test_framework/unit_test_framework.h"

#include "../../include/ChunkBuilder/ChunkBuilder.h"
#include <memory>

std::vector<std::string> document1 = {
    "pig",
    "the",
    "cow",
    "the",
    "pig",
    "and",
    "all",
    "of",
    "the",
    "animals",
};

std::vector<std::string> document2 = {
    "this",
    "is",
    "a",
    "and",
    "test",
};

std::vector<std::string> document3 = {
    "hello", "here", "is", "an", "entire", "new", "doc", "more"
};

std::vector<std::string> document4 = {
    "these", "are", "more", "friends", "romans", "countrymen"
};

std::vector<std::string> document5 = {
    "google", "search", "is", "a", "bit", "bigger"
};

std::vector<std::string> document6 = {
    "the", "home", "depot", "is", "a", "hardware", "store"
};

std::vector<std::string> urls = {
    "https://eecs440.com",
    "https://eecs441.com",
    "https://eecs442.com",
    "https://eecs443.com",
    "https://eecs444.com",
    "https://homedepot.com"
};

std::vector<std::vector<IndexEntry>> documentListBuilder(std::vector<std::vector<std::string>> &docList) {
    std::vector<std::vector<IndexEntry>> docs;
    for (auto doc: docList) {
        docs.push_back(std::vector<IndexEntry>());
        for (auto entry : doc) {
            docs.back().push_back(IndexEntry(entry));
        }
    }
    return docs;
}

std::unique_ptr<ChunkBuilder> buildSmallIndex(bool print) {
    std::unique_ptr<ChunkBuilder> index = std::make_unique<ChunkBuilder>();
    std::vector<std::vector<std::string>> words = {document1, document2};
    std::vector<std::vector<IndexEntry>> documents = documentListBuilder(words);

    std::vector<AnchorText> aText;
    index->addDocument(urls[0], documents[0], aText);
    index->addDocument(urls[1], documents[1], aText);

    return index;
}

std::unique_ptr<ChunkBuilder> buildMediumIndex(bool print) {
    std::unique_ptr<ChunkBuilder> index = std::make_unique<ChunkBuilder>();
    std::vector<std::vector<std::string>> words = {document1, document2, document3, document4, document5, document6};
    std::vector<std::vector<IndexEntry>> documents = documentListBuilder(words);

    std::vector<AnchorText> aText;
    for (int i = 0; i < urls.size(); ++i) {
        index->addDocument(urls[i], documents[i], aText);
    }
    
    return index;
}

TEST(test_index_non_null) {
    std::unique_ptr<ChunkBuilder> index = std::move(buildSmallIndex(false));
    ASSERT_TRUE(index != nullptr);
}

TEST(test_index_size) {
    std::unique_ptr<ChunkBuilder> index = std::move(buildSmallIndex(false));
    ChunkStats stats = index->getStats();
    ASSERT_EQUAL(stats.numDocuments, 2);
    ASSERT_EQUAL(stats.maxLocation, 16);
    ASSERT_EQUAL(stats.maxEstimatedBytes, 256)

    auto docEndList = index->getEndDocList();
    ASSERT_EQUAL(docEndList->value->posts.size(), stats.numDocuments)
    ASSERT_EQUAL(docEndList->value->posts[0]->loc, document1.size())
    ASSERT_EQUAL(docEndList->value->posts[1]->loc, document1.size() + document2.size() + 1);
}

TEST(test_locations) {
    std::unique_ptr<ChunkBuilder> index = std::move(buildSmallIndex(false));
    std::vector<Location> expectedLocations = { 1, 3, 8 };

    PostingList * posts = index->getPostingList("the");

    ASSERT_EQUAL(posts->Seek(0)->loc, expectedLocations[0]);
    ASSERT_EQUAL(posts->Seek(expectedLocations[0]+1)->loc, expectedLocations[1]);
    ASSERT_EQUAL(posts->Seek(expectedLocations[1]+1)->loc, expectedLocations[2]);
}

TEST(test_medium_index) {
    std::unique_ptr<ChunkBuilder> index = std::move(buildMediumIndex(false));
}

