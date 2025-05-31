#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "network.h"

int main(int argc, char **argv) {
    Network example{argv[1]};
    
    std::string routeIdtest = "21140_700";

    std::vector<bht::Trip> fahrten = example.getTripsForRoute(routeIdtest);

    for (const auto& trip : fahrten) 
    {
    std::string tripName = example.getTripDisplayName(trip);
    std::cout << tripName << std::endl;
    }

    //std::vector<Trip> gefundeneTrips = example.getTripsForRoute(routeIdtest);
    /*
    std::cout << "zur der routeId " << routeIdtest << " wurden folgende trips gefunden: " << std::endl;
    for (const auto& trip : gefundeneTrips)
    {
        std::cout << trip.routeId << " " << trip.id << " " << trip.serviceId << " " << trip.headsign << " " << trip.shortName << " " << static_cast<int>(trip.direction) << " " << trip.blockId << " " << trip.shapeId << " " << static_cast<int>(trip.wheelchairAccessible) << " " << static_cast<int>(trip.bikesAllowed) << "\n";    
    }
    */
    /*
    auto allroutes = example.getRoutes();

    for (const auto& route : allroutes) 
    {
    //std::cout << route.id << "," << route.agencyId << "," << route.shortName << "," << route.longName << "," << static_cast<int>(route.type) << "," << route.color << "," << route.textColor << "," << route.description << "\n";
    //std::cout << example.getRouteDisplayName(route) << std::endl;
    }
    */
    return 0;
}
