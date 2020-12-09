#ifndef FILEID_H	// prevent header file from being included mult times
#define FILEID_H
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
using namespace std;

// -----------------------------------------------------------------------------
// FileID class
class FileID {
public:
    string pathName;
    string fileName;
    ino_t inodeNum;
    int fileLen;
    int nLinks;
    bool is_flagged = false;
    vector<string> nWords;	// remove since it's in Params.cpp?

    FileID() {}
    ~FileID() {}	// deconstructor
    FileID(string, string, ino_t, int, int);
    void insert(string, bool);
    int case_insensitive_match(string, string);
};

// -----------------------------------------------------------------------------
// Stats class
class Stats : public stat {
public:
    ino_t inode() { return st_ino; }
    off_t size() { return st_size; }
    nlink_t links() { return st_nlink; }

    void print(ostream&);
};

// -----------------------------------------------------------------------------
// Direntry class
class Direntry : public dirent {
public:
    char* name() { return d_name; }
    ino_t inode() { return d_ino; }
    __uint8_t type() { return d_type; }

    void printVerbose(ostream&);
    ~Direntry() {}
};

#endif

