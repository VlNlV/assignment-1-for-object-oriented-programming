#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "csv.h"
#include "network.h"

int main(int argc, char **argv) {
/*CSVReader reader{argv[1]};     // object "reader" gets created and its path is "agency.txt" (constructer execution follows)
    reader.hasNext();   // checks IF there is a next line
    reader.next();      // then actually goes to the NEXT line (and reads it)
    std::cout << reader.getField("\"agency_name\"") << std::endl; //prints the first "field" under agency_id (BEWARE, the "\ ... \" are needed because the line needs to search for the actual quotation marks AND the column)
    reader.next();                              // goes to the next line and reads it
    std::cout << reader.getField("\"agency_name\"") << std::endl;     //prints the second "field" under agency_id
*/

if (argc != 2) {
  std::cerr << "Parameter wird gebraucht" << std::endl;
  return 1;
}

Network example{argv[1]};
  std::cout << "agencies.size: " << example.agencies.size() << std::endl;
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
