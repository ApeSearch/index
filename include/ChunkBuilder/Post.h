#include <string>

using std::size_t;
typedef size_t Location; // The numbering of a token
typedef size_t FileOffset; 

enum WordAttributes {
    WordAttributeNormal, WordAttributeBold, WordAttributeHeading, NullPost
};

enum PostingListType {
    BodyText, TitleText
};

struct IndexEntry {
    IndexEntry(std::string _wordIn) : word(_wordIn), attribute(WordAttributeNormal), plType(BodyText) {}
    std::string word;
    WordAttributes attribute;
    PostingListType plType;
};

struct AnchorText {
    std::string text;
    size_t freq;
};

class Post { 
//in memory version of post, will replace loc with deltaPrev once written to disk
public:
    Post() : loc(0), tData(WordAttributeNormal){}
    Post(Location loc_, size_t tData_) : loc(loc_), tData(tData_) {}

    Location loc;
    size_t tData;
};

class WordPost: public Post {
public:
    WordPost(): Post() {}
     WordPost(Location loc_, size_t attribute_) : Post(loc_, attribute_) {}

};

class EODPost: public Post {
public:
    EODPost(): Post() {}
    EODPost(Location loc_, size_t urlIndex_) : Post(loc_, urlIndex_) {}
};

class AnchorPost : public Post {
public:
    AnchorPost(): Post() {}

    //loc in AnchorPost actually refers to its frequency
    AnchorPost(Location frequency, size_t urlIndex): Post(frequency, urlIndex) {}
};