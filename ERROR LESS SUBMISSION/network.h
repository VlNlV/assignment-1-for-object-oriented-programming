#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "types.h"

using namespace bht;

class Network {
public:
    Network(std::string directory);

    std::unordered_map<std::string, Agency> agencies;       // war schon vorgegeben:
    std::vector<CalendarDate> calendarDates;
    std::unordered_map<std::string, Calendar> calendars;
    std::unordered_map<std::string, Level> levels;
    std::unordered_map<std::string, Pathway> pathways;
    std::unordered_map<std::string, Route> routes;
    std::vector<Shape> shapes;
    std::vector<StopTime> stopTimes;
    std::unordered_map<std::string, Stop> stops;
    std::vector<Transfer> transfers;
    std::vector<Trip> trips;

private:
    void AgencyLoader(std::string path);        // methoden zum laden der einzelnen dateien (befasst jeweils das öffnen der datei, das durchgehen der datei und zuweisen der entsprechenden werte zu den werten aus dem struct)
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

    GTFSDate DateCreator(std::string timestamp);    // methoden zum besseren formatten für tag und zeit
    GTFSTime TimeCreator(std::string timestamp);
};
