#include "network.h"
#include "scheduled_trip.h"
#include <iostream>

// war bereits aus aufgaben-vorlage vorgegeben (nur die tripId habe ich geändert (damit ist es mit GTFSTest kompatibel))

int main(int argc, char **argv) {
  bht::Network n{argv[1]};
  bht::NetworkScheduledTrip trip = n.getScheduledTrip("230353670");
  for (bht::NetworkScheduledTrip::iterator iter = trip.begin(); iter != trip.end(); iter++) 
  {
    std::cout << iter->stopSequence << ": " << n.stops[iter->stopId].name << std::endl;
  }
}
