#ifndef CSVREADER_H
#define CSVREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class CSVReader {                               // class definition with its attributes
public:
    std::vector<std::string> column_titles; 
    std::string line;  
    std::ifstream opened_file; 
    std::string path;

public:
    CSVReader(std::string path);            // each function (method) declaration

    std::string getField(std::string key);

    std::string getField(std::string key, std::string defaultValue);

    bool next();

    bool hasNext();

    void reset();
};

#endif
