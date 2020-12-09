#ifndef SNIFF_H
#define SNIFF_H
#include "Params.hpp"
#include "FileID.hpp"

class Sniff {
private:
    ofstream out;
    string relativePath;
    string absolutePath;
    Params* param;
    vector<FileID> files;

    void setPathNames(string);
    FileID oneFile(Direntry*,string);
    void travel(string,string);
    void errorCheck(string);
    int case_insensitive_match(string, string );
    void printHeader(ostream&);
    void printStats(ostream&);

public:
    Sniff(int, char**);
    ~Sniff() {}	// destructor

    void run();
};

#endif
