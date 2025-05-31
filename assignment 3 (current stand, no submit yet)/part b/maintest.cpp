#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "network.h"

int main(int argc, char **argv) {
    Network example{argv[1]};
     
    
    auto allroutes = example.getRoutes();

    for (const auto& route : allroutes) 
    {
    //std::cout << route.id << "," << route.agencyId << "," << route.shortName << "," << route.longName << "," << static_cast<int>(route.type) << "," << route.color << "," << route.textColor << "," << route.description << "\n";
    std::cout << example.getRouteDisplayName(route) << std::endl;
    }
    return 0;
}
