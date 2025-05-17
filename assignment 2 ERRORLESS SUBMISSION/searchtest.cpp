#include <iostream>
#include "csv.h"
#include "network.h"

int main(int argc, char **argv)   // testen des endgültigen programms
{
    if (argc != 2)  // kleiner check, ob man den GTFS Ordner angegeben hat
    {
        std::cerr << "bruh, please give the GTFS folder as the argument!" << std::endl;   // custom error message falls kein argument gegeben wird
        return 1;
    }

    Network example(argv[1]);   // erzeugen des objekts "example" (parameter wäre dann der datenpfad zu den GTFS sachen )

    std::string haltestelle;        // string variable fürs speichern der user-input suchwort
    std::cout << "wassup! nach welcher haltestelle suchst du bro? ";    // nach user-input/suchwort wird gefragt
    std::getline(std::cin, haltestelle);        // der user-input wird zur "halstestelle"

    std::vector<Stop> haltstellen_gefunden = example.search(haltestelle);   // an dem objekt (was die structs mit den GTFS zeug enthält) wird die search-methode aufgerufen, was einen ergebnisvector ausgibt, der die haltstellen enthält, welche zum suchwort passen

    std::cout << "es gibt " << haltstellen_gefunden.size() << " haltestellen die zu \"" << haltestelle << "\" passen:" << std::endl;    // zeile wird geprintet, bei der die anzahl an ergebnissen zum suchwort erscheint

    for (const Stop& einzelne_haltestelle : haltstellen_gefunden)   // range based for schleife fürs printen der einzelnen items des ergebnisvectors
    {
        std::cout << einzelne_haltestelle.id << ": " << einzelne_haltestelle.name << std::endl; // printe die haltestellen-ID mit ihrem entsprechenden halte-stellen namen
    }                                                                                           // printen der zusätzlichen ID ist ideal, da viele haltestellen mehrmals vorkommen (z.B. S+U Rathaus Steglitz)

    return 0;
}
