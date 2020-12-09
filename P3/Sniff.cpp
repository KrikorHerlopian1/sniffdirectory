#include "tools.hpp"
#include "Sniff.hpp"
#include <algorithm>
#include <unistd.h>
#include <map>
#include <vector>

// ----------------------------------------------------------------------------- 
// Sniff constructor
Sniff::Sniff(int argc, char* argv[]) {
    if (argc < 2)
        fatal("\n*** ERROR: At least one parameter required: sniff directory ***\n");
    param = new Params(argc, argv);
}

// -----------------------------------------------------------------------------
// print output and run the sniff
void Sniff::run() {	
    if (param->is_outputFile)
        out.open(param->oFile,fstream::app);

    fbanner(out);

    // print header info
    if (param->is_outputFile)
        printHeader(out);
    printHeader(cout);
            
    // normalize pathname
    // path = param->directory;
    // searchpath defaults to current dir
    path = convertToAbsolute(param->searchpath);
        
    oneDir();  // the actual sniff
        
    // print the file stat info
    if (param->is_outputFile)
        printStats(out);
    printStats(cout);
    out.close();
    delete param;
}

// -----------------------------------------------------------------------------
// echo command line args
void Sniff::printHeader(ostream& outstream) {
    outstream << "Verbose?\t\t" << (param->is_verbose ? "Yes" : "No") << endl;
    outstream << "Help?\t\t\t" << (param->is_help ? "Yes" : "No") << endl;
    outstream << "Case sensitive?\t\t" << (param->is_casesensitive ? "Yes" : "No") << endl;
    outstream << "Recursive\t\t" << (param->is_recursive ? "Yes" : "No") << endl;
    outstream << "Output file name:\t" << (param->oFile) << endl;
    outstream << "Directory:\t\t" << param->searchpath << endl;
    outstream << endl << "Search string words: " << param->searchString << endl;
    for(unsigned int t=0;t<param->nWords.size();++t){
        outstream << "\t\t\tWord "  << t << ": " << param->nWords.at(t) << endl;
    }
    outstream << "---------------------------------------------------------------";
}

int Sniff::case_insensitive_match(string s1, string s2) {
    //convert s1 and s2 into lower case strings
    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
    transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
    if (s1.compare(s2) == 0)
        return 1;   // strings are the same
    return 0;       // not matched
}

// Print stats for all output
void Sniff::printStats(ostream& outstream) {
    outstream << endl;
    map<ino_t, vector<FileID>> fileMap;
    for (unsigned int i=0; i < files.size(); ++i) {
        fileMap[files[i].inodeNum].push_back(files[i]);
    }
	
    for (map<ino_t, vector<FileID>>::iterator it=fileMap.begin(); it != fileMap.end(); ++it) {
        outstream << "I-Node " << it->first << " links " << it->second[0].nLinks << endl;
        for (unsigned int k = 0; k < it->second.size(); k++) {
            outstream << "\tWords found:";
            for (unsigned int m=0; m < it->second[k].nWords.size(); ++m) {
                outstream << " " << it->second[k].nWords[m];
            }
            outstream << endl;
            outstream << "\t" << "File dir:    " << it->second[k].pathName << endl;
            outstream << "\t" << "File name:   " << it->second[k].fileName << endl << endl;
        }
    }
}

// -----------------------------------------------------------------------------
// Convert to absolute file path
string Sniff::convertToAbsolute(string path) {
    char cwd[255];
    string absolutePath;
    string relativePath;
    if(chdir(param->searchpath.c_str()) !=0) {
       cout << "\nInvalid directory, exitting...\n" << endl;
       out << "\nInvalid directory, exitting...\n" << endl;
       exit(1);
    }
    path = "./"; 
    relativePath = path.substr(2, string::npos);

    getcwd(cwd, 255); // magic number because getcwd won't let me omit it
    absolutePath = strcat(cwd, "/");
    absolutePath = absolutePath + relativePath;

    return absolutePath;
}

// -----------------------------------------------------------------------------
// Open, read, and process the directory entries
void Sniff::oneDir() {
    Direntry *dir;
    DIR *d;

    d=opendir(path.c_str());
    if (d == NULL)
        errorCheck(path);

    // read all the entries in the directory
    while ((dir = static_cast<Direntry*>(readdir(d))) != NULL) {
        if (param->is_verbose)
            dir->printVerbose(out);	// Print all the verbose output up front

        if (dir->type() == DT_REG) {
            FileID f(oneFile(dir));	// oneFile chks for words in the file "dir"
            if (f.is_flagged)
                files.push_back(f); // process regular files using FileIDs, add flagged files to vector
        }
    }
    closedir(d);
}

// Construct the file ID using lstat info of the object
FileID Sniff::oneFile(Direntry *dir) {
    // absolute path of the file
    string filePath = path + "/" + dir->name();
    struct stat s;

    // store the file stats in the struct
    if (lstat(filePath.c_str(), &s) < 0)
        errorCheck(filePath);

    vector <string> words_found;
    string word;
    fstream file;
    // opening file
    file.open(filePath.c_str());
    FileID f(dir->name(),path, s.st_ino, s.st_size, s.st_nlink);
    f.is_flagged = false;
    // extracting words from the file
    while (file >> word) {
        for(unsigned int t=0;t<param->nWords.size();++t) {
            if (param->is_casesensitive && case_insensitive_match(param->nWords.at(t),word)) {
                f.is_flagged  = true;
                f.insert(word,param->is_casesensitive);
            } else if (param->nWords.at(t).compare(word) == 0) {
                f.is_flagged = true;
                f.insert(word,param->is_casesensitive);
            }
        }
    }
    return f;
}

// Check errno and print a useful comment, chk if file failed to be read
void Sniff::errorCheck(string failPath) {
    string errorMessage;
    cout << "Directory or file failed to be read: " << failPath << endl;
    switch (errno) {
        case EOVERFLOW:
            errorMessage = "File size too large.";
            break;
        case ELOOP:
            errorMessage = "Symbolic link loop.";
            break;
        case ENAMETOOLONG:
            errorMessage = "Path name too long.";
            break;
        case EIO:
            errorMessage = "I/O read error.";
            break;
        case EACCES:
            errorMessage = "Permission denied.";
            break;
        case EBADF:
        case ENOENT:
        case ENOTDIR:
            errorMessage = "Bad path name.";
            break;
        default:
            errorMessage = "Unknown error.";
            break;
    }
    cout << errorMessage << endl;
    exit(1);
}

