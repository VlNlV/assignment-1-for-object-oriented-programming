#pragma once
#include <string>
#include <unordered_map>
#include <vector>

// die GTFS structs von types.h (einbisschen modifiziert)
struct GTFSDate {
    unsigned char day;
    unsigned char month;
    unsigned short year;
};

struct GTFSTime {
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
};

struct Agency {
    std::string id;
    std::string name;
    std::string url;
    std::string timezone;
    std::string language;
    std::string phone;
};

enum CalendarDateException { Added = 1, Removed = 2 };
struct CalendarDate {
    std::string serviceId;
    GTFSDate date;
    CalendarDateException exception;
};

enum CalendarAvailability { Available = 1, NotAvailable = 0 };
struct Calendar {
    std::string serviceId;
    CalendarAvailability monday, tuesday, wednesday, thursday, friday, saturday, sunday;
    GTFSDate startDate;
    GTFSDate endDate;
};

struct Level {
    std::string id;
    int index;
    std::string name;
};

enum EPathwayMode { Walkway = 1, Stairs, Escalator, Elevator, FareGate, ExitGate };
struct Pathway {
    std::string id;
    std::string fromStopId;
    std::string toStopId;
    EPathwayMode mode;
    bool isBidirectional;
    float length;
    unsigned int traversalTime;
    int stairCount;
    float maxSlope;
    float minWidth;
    std::string signpostedAs;
};

enum RouteType { Tram, Subway, Rail, Bus, Ferry, CableCar, Gondola, Funicular };
struct Route {
    std::string id;
    std::string agencyId;
    std::string shortName;
    std::string longName;
    std::string desc;
    RouteType type;
    std::string color;
    std::string textColor;
};

struct Shape {
    std::string shapeId;
    double latitude;
    double longitude;
    unsigned int sequence;
};

enum EPickupType { RegularPickup = 0, NoPickup = 1, PhonePickup = 2, DriverCoordPickup = 3 };
enum EDropOffType { RegularDropoff = 0, NoDropoff = 1, PhoneDropoff = 2, DriverCoordDropoff = 3 };

struct StopTime {
    std::string tripId;
    GTFSTime arrivalTime;
    GTFSTime departureTime;
    std::string stopId;
    unsigned int stopSequence;
    EPickupType pickupType;
    EDropOffType dropOffType;
    std::string stopHeadsign;
};

enum LocationType { StopLocation = 0, Station, Entrance, GenericNode, BoardingArea };
enum WheelchairAccessibility { Unknown = 0, Accessible = 1, NotAccessible = 2 };

struct Stop {
    std::string id;
    std::string code;
    std::string name;
    std::string desc;
    double latitude;
    double longitude;
    LocationType locationType;
    std::string parentStation;
    WheelchairAccessibility wheelchairBoarding;
    std::string platformCode;
    std::string levelId;
    std::string zoneId;
};

enum TransferType { Recommended, Timed, MinimumTime, NotPossible };
struct Transfer {
    std::string fromStopId;
    std::string toStopId;
    std::string fromRouteId;
    std::string toRouteId;
    std::string fromTripId;
    std::string toTripId;
    TransferType transferType;
    unsigned int minTransferTime;
};

enum TripDirection { Outbound = 0, Inbound = 1 };
enum BikesAllowed { UnknownBikes = 0, Allowed = 1, NotAllowed = 2 };

struct Trip {
    std::string tripId;
    std::string routeId;
    std::string serviceId;
    std::string tripHeadsign;
    std::string tripShortName;
    TripDirection directionId;
    std::string blockId;
    std::string shapeId;
    WheelchairAccessibility wheelchairAccessible;
    BikesAllowed bikesAllowed;
};

// die eigentliche "network.h"
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
