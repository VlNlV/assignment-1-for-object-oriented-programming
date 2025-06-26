#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "types.h"
#include <unordered_set>
#include "scheduled_trip.h"
#include <memory>
#include <map>

namespace bht {

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

    std::multimap<std::string, std::string> stopzutripsmap;     // multimap container zum zuordnen von einem stop zu ihren trips

    std::multimap<std::string, std::string> stopzustationsmap;  // dasselbe aber von stop zu ihren stations

    std::multimap<std::string, std::string> tripzustopsmap;     // dasselbe aber trips zu ihren stops 

    std::multimap<std::string, StopTime> tripzustoptimesmap;    // dasselbe aber von trip zu ihren stoptimes
    
    std::unordered_map<std::string, std::shared_ptr<NetworkScheduledTrip>> tripsdiescheduledsind;   // shared_ptr mit unordered map für die trips die gescheduled sind

    std::vector<Stop> search(const std::string& suchwort);  // methoden-deklaration für die such funktion (bekommt einen string "const" mit "std:string&" damit wert nicht verändert und kopiert wird, "&" ist nämlich eine referenz --> good practice)
                                                            // ergebnis von methode ist vector aus stop-objekten

    std::vector<bht::Route> getRoutes();                    // deklarationen zu funktionen "getRoutes" und "getRouteDisplayName", wobei "getRoutes" einen vektor von "Route"-objekten zurückgibt

    std::string getRouteDisplayName(bht::Route route);      // "getRouteDisplayName" arbeitet noch mit einem Parameter (also einer route)

    std::vector<Trip> getTripsForRoute(std::string routeId);    // deklaration der funktion "getTripsForRoute", um die einzelnen trips einer einzelnen linie herauszufinden

    std::string getTripDisplayName(bht::Trip trip);         // deklaration der funktion "getTripDisplayName", um die tripshortname bzw. tripheadsign einer einzelnen linie zu bestimmen

    std::vector<StopTime> getStopTimesForTrip(std::string& tripId); // deklaration der funktion "getStopTimesForTrip", die mittels einer tripId die stoptimes-objekte bestimmt

    Stop getStopById(const std::string& stopId);    // deklaration der funktion "getStopById" die mittels einer stopId die haltestelle bestimmt

    std::vector<StopTime> searchStopTimesForTrip(std::string query, std::string tripId); // deklaration der funktion "searchStopTimesForTrip" die mittels der query (also des suchworts) und der tripId die stoptimes bestimmt

    std::vector<bht::Stop> getStopsForTransfer(const std::string& stopId);  // verlangte methode für die teilaufgabe a) aus Übung 4

    std::unordered_set<std::string> getNeighbors(const std::string& stopId); // erste verlangt methode für die teilaufgabe b) aus Übung 4 <-- diese methode braucht dazu den header (siehe oben)

    std::vector<bht::Stop> getTravelPath(const std::string& fromStopId, const std::string& toStopId);   // zweite verlangte methode für die teilaufgabe b) (berechnet den kürzesten weg von start zu ziel)

    NetworkScheduledTrip getScheduledTrip(const std::string& tripId) const;     //verlangt methode aus teilaufgabe c) (übergabe einer tripId und rückgabe eines NetworkScheduledTrip-Objekts)

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
}
