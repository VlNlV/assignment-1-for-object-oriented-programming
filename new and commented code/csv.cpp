#include "csv.h"
#include <iostream>
#include <sstream>
#include <algorithm>

CSVReader::CSVReader(std::string path) : filePath(path)         // konstruktor für wenn ein CSVReader objekt erstellt wird (path ist der pfad zu einer datei und der pfad wird vorerst der variable "filepath" zugewiesen)
{
    file.open(filePath);                                // öffnet die datei (via des pfads)
    if (!file.is_open())                        // kleiner check fürs überprüfen ob datei nicht geöffnet wurde
    {                      
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return;
    }
    std::string headerLine;                     // string variable für die header
    if (std::getline(file, headerLine))         // liest die erste zeile aus der datei (was bei allen die header sind) und speichert die header-zeile in "line"
    {   
        headers = readLine(headerLine);         // readline-funktion wird aufgerufen mit dem parameter "headerline" (also der header-zeile) und ihr return-wert (ein vector mit den ""-getrimmten headers) wird in den "headers"-vector gesteckt
    }
}

bool CSVReader::next()      // funktion fürs springen auf die nächste zeile
{
    if (!file.is_open() || file.eof())      // kleiner check ob datei geöffnet werden könnte oder wir am ende der datei sind
    {
        return false;                   // returne dann false
    }
    std::string line;               // string variable "line"
    if (std::getline(file, line))      // geht zur nächsten zeile, speichert sie in "line"
    {
        currentRow = readLine(line);       // currentrow wird dem return-wert von readline-funktion gegeben (wobei die nun aktuelle zeile in der datei der parameter dafür ist) (currentrow entspricht also einem vector mit den komma-getrennten values der aktuellen zeile)
        return true;                        // returne true
    }
    return false;                   // returne false
}

bool CSVReader::hasNext()           // funktion fürs checken ob überhaupt was nächstes kommt
{
    return file.is_open() && file.good();       // returned den wert von "ist-datei-offen" UND "hat-die-datei-inhalt"
}

void CSVReader::reset()                // reset funktion zum "resetten" der leseposition
{
    file.close();                       // schließt die datei
    file.open(filePath);                // öffnet sie wieder (wir sind also wieder ganz oben)
    if (!file.is_open())                // kleiner check ob datei richtig geöffnet wurde
    {
        std::cerr << "Error: Could not reopen file " << filePath << std::endl;  // custom error message
        return;
    }
    std::string headerLine;             // string variable für die headers
    if (std::getline(file, headerLine))     // holt sich die erste zeile (die komma-getrennten headers) und steckt sie in "headerline"
    {
        headers = readLine(headerLine);     // return wert von "readline" ist ein vector mit den ""-getrennten headers und dieser wird dem vector "headers" zugewiesen
    }
}

std::string CSVReader::getField(std::string fieldName, std::string defaultValue) const  // methode fürs finden eines wertes zu dem "key" bzw. fieldname
{
    auto it = std::find(headers.begin(), headers.end(), fieldName); // iterator (selbst-recherche!!!) --> suche durch den ""-getrennten headers vector nach dem fieldName ( = eines der headers), "it" wird zu der position des fieldNames im header-vector
    if (it == headers.end())    // wenn kein fieldName eines der header entspricht, dann zeigt der iterator auf das ende (und dies checkt das)
    {
        return defaultValue;        // also wenn keine überstimmung gefunden wurde, dann returne den default value
    }
    size_t index = std::distance(headers.begin(), it);  // findet den index des fieldNames heraus mit "distance" (selbst-recherche!!!) wobei der "item"-abstand vom anfang des vektors zu fieldname (im vector) gefunden wird
    if (index >= currentRow.size())         // kleiner check ob die die position von fieldname größer gleich der zeilenmenge ist (also wenn kein index einem wert entspricht)
    {
            return defaultValue; // returne den default value
    }
    return currentRow[index];   // return den wert der aktuellen zeile vom index des fieldnames
}

std::vector<std::string> CSVReader::readLine(std::string line) const // funktion fürs lesen einer zeile und stecken (der zeile) in einen string vector
{
    std::vector<std::string> result;        // string vector "result"
    std::stringstream ss(line);             // erstellen eines string stream objekts "ss" (mit der "header-zeile") als parameter (mit ifstream waren wir auf "opened-file"-ebene aber mit stringstream sind wir jetzt auf "specific-line-of-opened-file"-ebene)
    std::string field;                      // string variable "field"
    while (std::getline(ss, field, ','))       // geht durch die "header-zeile" und sucht die komma-getrennten headers mit dem delimiter "," und speichert sie in "field"
    {
        if (!field.empty() && field.front() == '\"' && field.back() == '\"') // da die header mit "" umgeben sind, wird gecheckt ob ein header überhaupt existiert und sie in "" umgeben sind
        {
            field = field.substr(1, field.length() - 2);        // field wird neu zugewiesen (mit removal der "") (der nicht-""-teil der header wird einzelnt entfernt)
        }
        result.push_back(field);            // der neu "getrimmte" header wird zum "result"-vector hinzugefügt
    }
    return result;      // returne den vector
}
