#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "csv.h"

CSVReader::CSVReader(std::string path) : path(path) {
    opened_file.open(path);
    if (!opened_file.is_open()) { // [Added] Check if file opened successfully to avoid invalid reads
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }

    if (std::getline(opened_file, line)) { // Read first line to extract column names
        if (line.empty()) { // [Added] Check if header line is empty
            std::cerr << "Warning: Header line is empty in file: " << path << std::endl;
            return;
        }
        std::stringstream first_line(line);
        std::string column_name;
        while (std::getline(first_line, column_name, ',')) {
            column_titles.push_back(column_name);
        }
    } else {
        std::cerr << "Warning: Could not read header line from: " << path << std::endl; // [Added] Warn if header missing
    }
}


// Get a field by column title
std::string CSVReader::getField(std::string key) {
    for (int i = 0; i < column_titles.size(); ++i) {
        if (column_titles[i] == key) {
            std::stringstream singularline(line);
            std::string column_member;
            int column_position = 0;
            while (std::getline(singularline, column_member, ',')) {
                if (column_position == i) {
                    return column_member;
                }
                column_position++;
            }
        }
    }
    return ""; // Return empty string if field not found
}

            
 // Get a field by column title with default value if not found
std::string CSVReader::getField(std::string key, std::string defaultValue) {
    for (int i = 0; i < column_titles.size(); ++i) {
        if (column_titles[i] == key) {
            std::stringstream singularline(line);
            std::string column_member;
            int column_position = 0;
            while (std::getline(singularline, column_member, ',')) {
                if (column_position == i) {
                    return column_member;
                }
                column_position++;
            }
        }
    }
    return defaultValue; // Return provided default if field not found
}

        
// Move to the next line in CSV
bool CSVReader::next() {
    if (std::getline(opened_file, line)) { // Move to next line
        if (line.empty()) { // [Added] Skip empty lines to avoid processing empty content
            std::cerr << "Warning: Empty line encountered. Skipping.\n";
            return false;
        }
        return true;
    } else {
        return false; // End of file reached
    }
}


        bool CSVReader::hasNext()      
        {
            return opened_file.peek() != EOF;       // just checks if the next "coming" character is NOT the end of file, so basically just checks if "there is a character left", if yes, then returns true, if not, then returns false
        }   

        void CSVReader::reset()
        {
            opened_file.clear();             // resets error flags for when going through the file
            opened_file.seekg(0);               // the "resetter" of the reading position back to the top
            std::getline(opened_file, line);    // after being moved back to the top, it reads the header titles again
        }

