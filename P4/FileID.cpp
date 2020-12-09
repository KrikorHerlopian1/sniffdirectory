#include "tools.hpp"
#include "FileID.hpp"
#include <algorithm>	// contains transform function to lowercase string

// -------------------------------------------------------------------
// FileID functions
FileID::FileID(string name, string path, ino_t num, int len, int links) {
    fileName = name;
    inodeNum = num;
    fileLen = len;
    nLinks = links;
    pathName = path;
}

void FileID::insert(string word, bool is_casesensitive) {
    bool is_found = false;
    for (unsigned int n=0; n<nWords.size(); n++) {
        if (is_casesensitive && case_insensitive_match(word,nWords[n])) {
            is_found = true;
            break;
        }
        else if (!is_casesensitive && word.compare(nWords[n]) == 0) {
            is_found = true;
            break;
        }
    }
    if (!is_found)
        nWords.push_back(word);
}

int FileID::case_insensitive_match(string s1, string s2) {
   //convert s1 and s2 into lower case strings
   transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
   transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
   if(s1.compare(s2) == 0)
       return 1; // strings are the same
   return 0;     // not matched
}

// -----------------------------------------------------------------------------
// Stats functions
void Stats::print(ostream& out) {
    out << "I-node " << inode() << " links " << links() << endl;
}

// -------------------------------------------------------------------
// Direntry functions
void Direntry::printVerbose(ostream& out) {
    string typeString;
    switch (type()) {
        case DT_DIR:
            typeString = "directory"; break;
        case DT_REG:
            typeString = "file"; break;
        case DT_LNK:
            typeString = "link"; break;
        default:
            typeString = "unknown"; break;
    }
    out << setw(12) << typeString << setw(12) << inode() << setw(30) << name() << endl;
    cout << setw(12) << typeString << setw(12) << inode() << setw(30) << name() << endl;
}

