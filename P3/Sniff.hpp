#ifndef SNIFF_H
#define SNIFF_H
#include "Params.hpp"
#include "FileID.hpp"

class Sniff {
private:
    Params* param;
    vector<FileID> files;
    string path;
    ofstream out;

    string convertToAbsolute(string);
    FileID oneFile(Direntry*);
    void oneDir();
    void printHeader(ostream&);
    void printStats(ostream&);
    void errorCheck(string);
    int case_insensitive_match(string, string );

public:
    Sniff(int, char**);
    ~Sniff() {}	// destructor

    void run();
};

#endif
