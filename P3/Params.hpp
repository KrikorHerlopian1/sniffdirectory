#ifndef PARAMS_H
#define PARAMS_H
#include <vector>

class Params {
public:
    int opt = 0;
    int long_index = 0;
    
    string searchpath = "./";		// default to current directory;
    bool is_casesensitive = false;
    bool is_recursive = false;
    bool is_verbose = false;
    bool is_outputFile = false;
    bool is_help = false;
    string oFile = "/dev/stdout";	// default to stdout
    string searchString = "";
    vector<string> nWords;		// sniff words found in file

    Params(int, char**);
    ~Params() {}			// deconstructor
};

#endif
