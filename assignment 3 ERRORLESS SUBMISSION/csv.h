#pragma once            // header-file merkmal
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

class CSVReader {   // die klasse "CSVReader"
public:                 // ihre methoden (mit access specifier "public" --> good practice)
    explicit CSVReader(std::string path);       // konstruktor mit "explicit" (für präziseres aufrufen des konstruktors) ("explicit" durch selbst-recherche !!!)
    bool next();            // methode fürs checken (ob eine zeile noch kommt)
    bool hasNext();         // methode fürs eigentliche "gehen-zur-nächsten-zeile"
    void reset();           // methode fürs "resetten" der leseposition (quasi bringt uns wieder nach oben)
    std::string getField(std::string fieldName, std::string defaultValue = "") const;   // methode fürs kriegen des werts nach einem bestimmten fieldname (bzw. "key" aus der aufgabe), der defaultValue ist automatisch schon ein leerer string (const hilft auch dagegen, damit sich der wert den man ausliest niemals geändert wird)

private:    // die privaten attributen
    std::vector<std::string> headers;       // vector "headers" fürs speichern der tabellen-"titel" bzw. headers
    std::vector<std::string> currentRow;    // vector fürs speichern der aktuellen zeile
    std::ifstream file;                     // das eigentlich ifstream objekt (entspricht später der "geöffneten datei")
    std::string filePath;                   // string variable für den datenpfad (der die GTFS daten enthält)

    std::vector<std::string> readLine(std::string line) const;  // zusätzliche methode fürs lesen einer zeile und stecken (der zeile) in einen string vector
};
