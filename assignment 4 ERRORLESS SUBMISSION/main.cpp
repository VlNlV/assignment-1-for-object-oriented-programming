#include <iostream>
#include "types.h"
#include "network.h"

int main(int argc, char **argv) {
  
bht::Network beispiel{argv[1]};     // bekommt zugriff auf die dateien

  auto moeglichenumstiege = beispiel.getStopsForTransfer(argv[2]);  // bekommt zugriff auf dateien + die untersuchte stopID für die umstiege

  std::cout << "hier sind alle zugehörige infos zum stop von: " << argv[2] << std::endl;

  for (auto moeglicherumstieg : moeglichenumstiege) 
  {
    std::cout << moeglicherumstieg.id << std::endl;     // printet die möglichen umstiege via getstopfortransfer
  }

  auto nachbarn = beispiel.getNeighbors(argv[2]);       // bekommt zugriff auf dateien + untersuchte stopID für die nachbarn

  std::cout << "die nachbarn vom stop " << argv[2] << " sind: " << std::endl;

  for (auto einzelnernachbar : nachbarn) 
  {
    std::cout << einzelnernachbar << " = " << beispiel.stops[einzelnernachbar].name << std::endl;   // printet jeden einzelnen nachbarn der stopID
  }

  auto travelpath = beispiel.getTravelPath(argv[2], argv[3]);   // bekommt zugriff auf die zwei gegebenen stopIDs für den travelpath

  std::cout << "die länge des travel paths besteht aus " << travelpath.size() << " trips" << std::endl;

  for (auto einzelnerpunkt : travelpath) 
  {
  std::cout << einzelnerpunkt.id << " aka " << einzelnerpunkt.name << std::endl; // printet jeden stop des travelpaths
  }

  bht::NetworkScheduledTrip trip = beispiel.getScheduledTrip("230353669");  // bekommt die tripID für den trip

  std::cout << "der scheduled trip ist: " << std::endl;

  for (bht::NetworkScheduledTrip::iterator iter = trip.begin(); iter != trip.end(); iter++) 
  {
    std::cout << iter->stopSequence << ": " << beispiel.stops[iter->stopId].name << " = " << iter->stopId << std::endl; // printet jede fahrt auf dem trip
  }

  std::distance(trip.begin(), trip.end());  // länge des trips wird bestimmt
}
