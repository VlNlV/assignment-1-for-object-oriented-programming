#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <unordered_map>
#include "types.h"

using namespace bht;

class Network{
public:
 std::unordered_map<std::string, Agency> agencies; // Inhalt von agency.txt
 std::vector<CalendarDate> calendarDates; // Inhalt von calendar_dates.txt
 std::unordered_map<std::string, Calendar> calendars; // Inhalt von calendar.txt
 std::unordered_map<std::string, Level> levels; // Inhalt von levels.txt
 std::unordered_map<std::string, Pathway> pathways; // Inhalt von pathways.txt
 std::unordered_map<std::string, Route> routes; // Inhalt von routes.txt
 std::vector<Shape> shapes; // Inhalt von shapes.txt
 std::vector<StopTime> stopTimes; // Inhalt von stop_times.txt
 std::unordered_map<std::string, Stop> stops; // Inhalt von stops.txt
 std::vector<Transfer> transfers; // Inhalt von transfers.txt
 std::vector<Trip> trips; // Inhalt von trips.txt 

Network(std::string mydirectory);

private:
void AgencyLoader(std::string path);
void CalendarDatesLoader(std::string path);
void CalendarsLoader(std::string path);
void FrequencyLoader(std::string path);
void LevelsLoader(std::string path);
void PathwaysLoader(std::string path);
void RoutesLoader(std::string path);
void ShapesLoader(std::string path);
void StopTimesLoader(std::string path);
void StopsLoader(std::string path);
void TransfersLoader(std::string path);
void TripsLoader(std::string path);

GTFSDate DateCreator(std::string timestamp);

GTFSTime TimeCreator(std::string timestamp);

};

#endif
