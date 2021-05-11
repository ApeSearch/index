#include "Post.h"

#include <vector>
#include "stdint.h"

class PostingList {
    public:
        PostingList(): posts(), deltas(), bytesList(0), calcBytes(false) {}
        ~PostingList() {
            for (size_t i = 0; i < posts.size(); ++i) 
               delete posts[i];
        }

        std::vector<Post *> posts; //pointers to individual posts
        std::vector<uint8_t> deltas;
        uint32_t bytesList;
        bool calcBytes; //has the number of bytes already been calculated?
        
        Post *Seek( Location l );

        //pure virtual function to handle appending a new post to list. lastDocIndex is for 
        //word/token posts to determine absolute location based on location of last document
        virtual void appendToList(Location loc_, size_t tData, size_t lastDocIndex = 0) = 0;
        virtual uint32_t bytesRequired( const std::string &key ) = 0;
    };

class WordPostingList : public PostingList
    {
    public:
        WordPostingList(): PostingList() {}
        uint32_t bytesRequired( const std::string &key ) override;
        void appendToList(Location loc_, size_t attribute, size_t lastDocIndex = 0) override;   
    };

class DocEndPostingList : public PostingList {
public:

        DocEndPostingList(): PostingList() {}
        uint32_t bytesRequired( const std::string &key ) override;
        void appendToList(Location loc_, size_t urlIndex, size_t lastDocIndex = 0) override; 
    };

class AnchorPostingList : public PostingList
    {
    public:

        AnchorPostingList(): PostingList() {}
        uint32_t bytesRequired( const std::string &key) override;

        //loc_ is unused, only for clean override
        void appendToList(Location freq, size_t urlIndex, size_t lastDocIndex = 0) override;
    };