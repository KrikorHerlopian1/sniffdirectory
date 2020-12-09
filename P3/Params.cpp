#include "tools.hpp"
#include "Params.hpp"
#include <getopt.h>
using namespace std;

void print_help() {
    cout <<
    "-d, --dir <searchpath>    set search directory (defaults to current dir)\n"
    "-i, --caseinsensitive     case-insensitive search\n"
    "-R, --recursive           recursive search\n"
    "-o, --outputFile <fname>  output filename (defaults to std out)\n"
    "-v, --verbose             print every file opened\n"
    "-h, --help                show help\n";
}

Params::Params(int argc, char* argv[]) {
    struct option long_options[] = {
        /* NAME          ARGUMENT           FLAG  SHORTNAME */
        {"dir",          required_argument,  0,   'd'},
        {"insensitive",  no_argument,        0,   'i'},
        {"recursive",    no_argument,        0,   'R'},
        {"outputfile",   required_argument,  0,   'o'},
        {"verbose",      no_argument,        0,   'v'},
        {"help",         no_argument,        0,   'h'},
        {0,              0,                  0,   0}
    };
    
    while ((opt = getopt_long(argc, argv,"d:iRo:vh", long_options, &long_index )) != -1) {
        switch (opt) {
            case 'd':
                searchpath = string(optarg);
                break;
                
            case 'i':
                is_casesensitive = true;
                break;
                
            case 'R':
                is_recursive = true;
                break;
                
            case 'o':
                oFile = string(optarg);
                is_outputFile = true;
                break;
                
            case 'v':
                is_verbose = true;
                break;
                
            default:
                is_help = true;
                print_help();
                exit(-1);
        }
    }

    int n = 0;
    string mystr;
    while (n < argc) {
        mystr += argv[n];
        mystr += " ";
        n++;
    }

    if ((optind + 1) > argc) {
        cout << "No search string provided..." << endl;
        exit(-1);
    }

    if (optind < argc) {
        while (optind < argc) {
            string word;
            string wlist(argv[optind]); // convert C string to C++ string
            istringstream instr(wlist); // wrap stream around the string

            // When parsing argv, eof occurs when last string is read,
            // not the next loop iteration
            for(;;) {
                instr >> word;
                nWords.push_back(word);
                if (instr.eof()) break;
            }

//            while (instr >> word) {
//                nWords.push_back(word);
//            }

            optind++;
        }
    }
}

