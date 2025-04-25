#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "csv.h"
        CSVReader::CSVReader(std::string path): path(path)  // constructor, we're doing the "classname::"-constructor syntax BECAUSE our class is only defined in the header file, so we need to do this syntax to refer to our class for this constructor
        {
            opened_file.open(path);             // open-method to open the file (we're not using "ifstream name(path)" because the class is in the header file)
            std::getline(opened_file, line);       // gets the first line of the text file and saves it in "line" (defined in the class aka header file)
            std::stringstream first_line(line);     // we recently went from input-file-to-line, but now we need to go to line-to-individual-characters, so we use "stringstream"
            std::string column_name;                // string variable for column

            while(std::getline(first_line, column_name, ','))   // while loop for iterating through ONE line, in this case just the column names (everytime it sees a comma, its gonna save the string it read so far in "column_name")
            {
                column_titles.push_back(column_name);       // this pushes each "column name" that was saved temporarily in column_name in the vector "column_titles"
            }
        }

        std::string CSVReader::getField(std::string key)    // getField method (with column name as parameter)
        {
            for(int i = 0; i < column_titles.size(); ++i)   // for loop for iterating through the vector "column_titles", so it can give each column name a number
            {
                if(column_titles[i] == key)                 // if the number(aka the column name inside of the vector) matches the column name parameter
                {
                    std::stringstream singularline(line);   // within ONE line, we create a stringstream object "singularline"
                    std::string column_member;              // string variable "column_member"
                    int column_position = 0;                // integer variable "column_position" initialized to 0
                    while(std::getline(singularline, column_member, ','))   // while for iterating through ONE line (reads up to a comma and whatever it read so far it saves temporarily in "column_member")
                    {
                        if(column_position == i)    // if the column_position equals "i" (aka the number of a header)
                        {
                            return column_member;      // print the column_member (no cout here because we need to return a STRING)
                        }
                        column_position++;              // if the if-condition didn't go through then increment column_position, so the if-condition will go through
                    }
                }
            }
            return "";      // if the "key" isnt one of the headers, then return empty space
        }
            
        std::string CSVReader::getField(std::string key, std::string defaultValue)  // exact same method as above just that "defaultValue" should be returned if "key" isn't one of the headers
        {
            for(int i = 0; i < column_titles.size(); ++i) 
            {
                if(column_titles[i] == key)
                {
                    std::stringstream singularline(line);
                    std::string column_member;
                    int column_position = 0;
                    while(std::getline(singularline, column_member, ','))
                    {
                        if(column_position == i)
                        {
                            return column_member;
                        }
                        column_position++;
                    }
                }
            }
            return defaultValue;    // here print defaultValue
        }
        
        bool CSVReader::next()
        {
            if (std::getline(opened_file, line))    // go to and read the next line and save it "line"
            {
                return true;                    // if that worked (so there's a line and it has been read), return true
            }
            return false;                   // no line and therefore nothing to be read, return false
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

