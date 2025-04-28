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

    std::cout << "agencies.size: " << example.agencies.size() << std::endl;   // was es printen soll (entsprechend zu jeder datei)
    std::cout << "calendarDates.size: " << example.calendarDates.size() << std::endl;
    std::cout << "calendars.size: " << example.calendars.size() << std::endl;
    std::cout << "levels.size: " << example.levels.size() << std::endl;
    std::cout << "pathways.size: " << example.pathways.size() << std::endl;
    std::cout << "routes.size: " << example.routes.size() << std::endl;
    std::cout << "shapes.size: " << example.shapes.size() << std::endl;
    std::cout << "stopTimes.size: " << example.stopTimes.size() << std::endl;
    std::cout << "stops.size: " << example.stops.size() << std::endl;
    std::cout << "transfers.size: " << example.transfers.size() << std::endl;
    std::cout << "trips.size: " << example.trips.size() << std::endl;

    return 0;
}
