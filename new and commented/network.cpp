#include "network.h"
#include "csv.h"
#include <iostream>

// hab das implementiert, weil ich ansonsten ständig "stoi" / "stod" errors bekam (also löst sich das problem mithilfe der hilf-funktionen)

int safeStoi(const std::string& value, int defaultValue = 0)    // eine hilf-funktion fürs sicheres konvertieren eines strings zu einem int (value ist ein zeiger auf den "target"-string und falls konvertierung nicht geklappt hat, dann returne "0")
{  
    if (value.empty()) return defaultValue;     // kleiner check um zu überprüfen, ob "target" string leer ist (returne also den default-Value "0")
    try {                                         // try-catch block (selbst-recherche!!!)
        return std::stoi(value);                    // returned den vorheriger-string-aber-jetzt-int-wert "value" (aber wenn exceptions dazu auftauchen, dann werden sie im "catch"-teil gecatched)
    } catch (...) {                             // (mit selbst-recherche --> (...) heißt, dass es bei alle exceptions reagiert bzw. alle exceptions catched)
        return defaultValue;                // returne den defaultvalue
    }
}

double safeStod(const std::string& value, double defaultValue = 0.0) // eine hilf-funktion fürs sicheres konvertieren eines strings zu einem double wert (value ist ein zeiger auf den "target"-string und falls konvertierung nicht geklappt hat, dann returne "0.0")
{
    if (value.empty()) return defaultValue;         // gleiche wie oben, nur das hier in ein double wert konvertiert werden sollte 
    try {
        return std::stod(value);
    } catch (...) {
        return defaultValue;
    }
}

Network::Network(std::string mydirectory) {     // definition des network konstruktors (es wird dafür gesorgt, dass mit jeder datei gearbeitet wird und zum zugriff auf die datei wird dem pfad "/dateiname.txt" hinzugefügt)
    AgencyLoader(mydirectory + "/agency.txt");                  // die einzelnen funktionen für die dateien)
    CalendarDatesLoader(mydirectory + "/calendar_dates.txt");
    CalendarsLoader(mydirectory + "/calendar.txt");
    FrequencyLoader(mydirectory + "/frequencies.txt");
    LevelsLoader(mydirectory + "/levels.txt");
    PathwaysLoader(mydirectory + "/pathways.txt");
    RoutesLoader(mydirectory + "/routes.txt");
    ShapesLoader(mydirectory + "/shapes.txt");
    StopTimesLoader(mydirectory + "/stop_times.txt");
    StopsLoader(mydirectory + "/stops.txt");
    TransfersLoader(mydirectory + "/transfers.txt");
    TripsLoader(mydirectory + "/trips.txt");
}

void Network::AgencyLoader(std::string path)        // funktion zum verarbeiten der datei "agency.txt" (path entspricht der genau pfad zu agency.txt)
{  
    CSVReader reader(path);                     // erstellt ein objekt "reader" der klasse "CSVReader" und der parameter ist der agency.txt-pfad
    while (reader.next())           // geht ständig zur nächsten zeile für jede iteration
    {
        Agency agency;              // wegen modifizierter network.h (also weglassen von types.h) wird dier das struct aus network.h deklariert und einzelnt die werte zu den headers zugewiesen
        agency.id = reader.getField("agency_id");
        agency.name = reader.getField("agency_name");
        agency.url = reader.getField("agency_url");
        agency.timezone = reader.getField("agency_timezone");
        agency.language = reader.getField("agency_lang");
        agency.phone = reader.getField("agency_phone");
        agencies[agency.id] = agency;           // fügt die agency in eine unordered map
    }
}

void Network::CalendarDatesLoader(std::string path) // ähnliche hier, nur das mit calender_dates.txt gearbeitet wird
{
    CSVReader reader(path);
    while (reader.next()) 
    {
        if (reader.getField("date").empty())        // kleiner check ob "date"-column leer ist
        {
            continue;                           // wenn ja, dann mache weiter
        }
        CalendarDate calDate;
        calDate.serviceId = reader.getField("service_id");
        calDate.date = DateCreator(reader.getField("date"));
        calDate.exception = static_cast<CalendarDateException>(safeStoi(reader.getField("exception_type"), 1));
        calendarDates.push_back(calDate);   // füge caldate in einen vector
    }
}

void Network::CalendarsLoader(std::string path) 
{
    CSVReader reader(path);
    while (reader.next())   
    {
        if (reader.getField("start_date").empty() || reader.getField("end_date").empty())   // kleiner check ob columns "start_date" und "end_date" leer sind
        {
            continue;       // wenn ja, dann mach weiter
        }
        Calendar calendar;
        calendar.serviceId = reader.getField("service_id");
        calendar.monday = static_cast<CalendarAvailability>(safeStoi(reader.getField("monday"), 1));
        calendar.tuesday = static_cast<CalendarAvailability>(safeStoi(reader.getField("tuesday"), 1));
        calendar.wednesday = static_cast<CalendarAvailability>(safeStoi(reader.getField("wednesday"), 1));
        calendar.thursday = static_cast<CalendarAvailability>(safeStoi(reader.getField("thursday"), 1));
        calendar.friday = static_cast<CalendarAvailability>(safeStoi(reader.getField("friday"), 1));
        calendar.saturday = static_cast<CalendarAvailability>(safeStoi(reader.getField("saturday"), 1));
        calendar.sunday = static_cast<CalendarAvailability>(safeStoi(reader.getField("sunday"), 1));
        calendar.startDate = DateCreator(reader.getField("start_date"));        // nutzen der tag-funktionen, um das datum besser zu formatten
        calendar.endDate = DateCreator(reader.getField("end_date"));
        calendars[calendar.serviceId] = calendar;       // füge calender in ein unordered map
    }
}

void Network::FrequencyLoader(std::string path) {}  // frequencies hat nur headers und keine werte also kann weggelassen werden

void Network::LevelsLoader(std::string path) 
{  
    CSVReader reader(path);
    while (reader.next()) 
    {
        Level level;
        level.id = reader.getField("level_id");
        level.index = safeStoi(reader.getField("level_index"));
        level.name = reader.getField("level_name");
        levels[level.id] = level;       // füge levels in ein unordered map
    }
}

void Network::PathwaysLoader(std::string path) 
{
    CSVReader reader(path);
    while (reader.next()) 
    {
        Pathway pathway;
        pathway.id = reader.getField("pathway_id");
        pathway.fromStopId = reader.getField("from_stop_id");
        pathway.toStopId = reader.getField("to_stop_id");
        pathway.mode = static_cast<EPathwayMode>(safeStoi(reader.getField("pathway_mode"), 1));
        pathway.isBidirectional = (reader.getField("is_bidirectional") == "1");
        pathway.length = safeStod(reader.getField("length"));
        pathway.traversalTime = static_cast<unsigned int>(safeStoi(reader.getField("traversal_time")));
        pathway.stairCount = safeStoi(reader.getField("stair_count"));
        pathway.maxSlope = safeStod(reader.getField("max_slope"));
        pathway.minWidth = safeStod(reader.getField("min_width"));
        pathway.signpostedAs = reader.getField("signposted_as");
        pathways[pathway.id] = pathway;     // füge pathway in ein unordered map
    }
}

void Network::RoutesLoader(std::string path)
 {
    CSVReader reader(path);
    while (reader.next()) 
    {
        Route route;
        route.id = reader.getField("route_id");
        route.agencyId = reader.getField("agency_id");
        route.shortName = reader.getField("route_short_name");
        route.longName = reader.getField("route_long_name");
        route.desc = reader.getField("route_desc");
        route.type = static_cast<RouteType>(safeStoi(reader.getField("route_type")));
        route.color = reader.getField("route_color");
        route.textColor = reader.getField("route_text_color");
        routes[route.id] = route;       // füge route in ein unordered map
    }
}

void Network::ShapesLoader(std::string path) 
{
    CSVReader reader(path);
    while (reader.next()) 
    {
        Shape shape;
        shape.shapeId = reader.getField("shape_id");
        shape.latitude = safeStod(reader.getField("shape_pt_lat"));
        shape.longitude = safeStod(reader.getField("shape_pt_lon"));
        shape.sequence = static_cast<unsigned int>(safeStoi(reader.getField("shape_pt_sequence")));
        shapes.push_back(shape);        // füge shape in ein vector
    }
}

void Network::StopTimesLoader(std::string path) 
{
    CSVReader reader(path);
    while (reader.next()) 
    {
        if (reader.getField("arrival_time").empty() || reader.getField("departure_time").empty())  // kleiner check ob headers "arrival_time" und "departure_time" leer sind
        {
            continue;       // wenn ja, dann mache weiter
        }
        StopTime stopTime;
        stopTime.tripId = reader.getField("trip_id");
        stopTime.arrivalTime = TimeCreator(reader.getField("arrival_time"));
        stopTime.departureTime = TimeCreator(reader.getField("departure_time"));
        stopTime.stopId = reader.getField("stop_id");
        stopTime.stopSequence = static_cast<unsigned int>(safeStoi(reader.getField("stop_sequence")));
        stopTime.pickupType = static_cast<EPickupType>(safeStoi(reader.getField("pickup_type")));
        stopTime.dropOffType = static_cast<EDropOffType>(safeStoi(reader.getField("drop_off_type")));
        stopTime.stopHeadsign = reader.getField("stop_headsign");
        stopTimes.push_back(stopTime);      // füge stoptime in einen vector
    }
}

void Network::StopsLoader(std::string path) 
{
    CSVReader reader(path);
    while (reader.next()) 
    {
        Stop stop;
        stop.id = reader.getField("stop_id");
        stop.code = reader.getField("stop_code");
        stop.name = reader.getField("stop_name");
        stop.desc = reader.getField("stop_desc");
        stop.latitude = safeStod(reader.getField("stop_lat"));
        stop.longitude = safeStod(reader.getField("stop_lon"));
        stop.locationType = static_cast<LocationType>(safeStoi(reader.getField("location_type")));
        stop.parentStation = reader.getField("parent_station");
        stop.wheelchairBoarding = static_cast<WheelchairAccessibility>(safeStoi(reader.getField("wheelchair_boarding")));
        stop.platformCode = reader.getField("platform_code");
        stop.levelId = reader.getField("level_id");
        stop.zoneId = reader.getField("zone_id");
        stops[stop.id] = stop;      // füge stop in ein unordered map
    }
}

void Network::TransfersLoader(std::string path) 
{
    CSVReader reader(path);
    while (reader.next()) 
    {
        Transfer transfer;
        transfer.fromStopId = reader.getField("from_stop_id");
        transfer.toStopId = reader.getField("to_stop_id");
        transfer.fromRouteId = reader.getField("from_route_id");
        transfer.toRouteId = reader.getField("to_route_id");
        transfer.fromTripId = reader.getField("from_trip_id");
        transfer.toTripId = reader.getField("to_trip_id");
        transfer.transferType = static_cast<TransferType>(safeStoi(reader.getField("transfer_type")));
        transfer.minTransferTime = static_cast<unsigned int>(safeStoi(reader.getField("min_transfer_time")));
        transfers.push_back(transfer);      // füge transfer in einen vector
    }
}

void Network::TripsLoader(std::string path) 
{
    CSVReader reader(path);
    while (reader.next()) 
    {
        Trip trip;
        trip.tripId = reader.getField("trip_id");
        trip.routeId = reader.getField("route_id");
        trip.serviceId = reader.getField("service_id");
        trip.tripHeadsign = reader.getField("trip_headsign");
        trip.tripShortName = reader.getField("trip_short_name");
        trip.directionId = static_cast<TripDirection>(safeStoi(reader.getField("direction_id")));
        trip.blockId = reader.getField("block_id");
        trip.shapeId = reader.getField("shape_id");
        trip.wheelchairAccessible = static_cast<WheelchairAccessibility>(safeStoi(reader.getField("wheelchair_accessible")));
        trip.bikesAllowed = static_cast<BikesAllowed>(safeStoi(reader.getField("bikes_allowed")));
        trips.push_back(trip);      // füge trip in einen vector
    }
}

GTFSDate Network::DateCreator(std::string timestamp)    // funktion fürs richtige formatten der tagesangabe
{
    GTFSDate date;
    if (timestamp.length() < 8)     // kleiner check ob die tagesangabe "YYYYMMDD" ist (bezüglich der länge)
    {
        date.day = 0;       // wenn nicht der fall, dann setze sie auf 0
        date.month = 0;
        date.year = 0;
        return date;        // returne das datum
    }
    date.day = (unsigned char)(safeStoi(timestamp.substr(6, 2)));       // aus der tagesangabe werden die einzelnen bestandteile (tag, monat, jahr) extrahiert (via substring)
    date.month = (unsigned char)(safeStoi(timestamp.substr(4, 2)));     // außerdem sorgt "safestoi" dass die felder mit den zeiten richtig in int konvertiert wird
    date.year = (unsigned short)(safeStoi(timestamp.substr(0, 4)));
    return date;        // returne das richtig formatted datum
}

GTFSTime Network::TimeCreator(std::string timestamp) 
{
    GTFSTime time;
    if (timestamp.length() < 5 || timestamp.find(":") == std::string::npos)     // kleiner check, falls die zeitangabe nicht valid ist oder ob es keinen doppelpunkt finden kann ("std::string::npos" --> selbst-recherche!! entspricht einer konstante, die hier zum benutzt wird, um zu überprüfen, ob ":" nicht in der zeitangabe ist)
    {
        time.hour = 0;
        time.minute = 0;        // setze alle zeitangabe zu 0
        time.second = 0;
        return time;        // returne die zeit
    }
    size_t first = timestamp.find(":");     // findet die position des ersten doppelpunkts
    size_t next = timestamp.find(":", first + 1);   // findet die position des zweiten doppelpunkts

    time.hour = (unsigned char)(safeStoi(timestamp.substr(0, first)));      // extrahiert die stunde aus der zeitangabe
    time.minute = (unsigned char)(safeStoi(timestamp.substr(first + 1, next - first - 1))); // extrahiert die minute aus der zeitangabe
    time.second = (unsigned char)(safeStoi(timestamp.substr(next + 1)));    // extrahiert die sekunde aus der zeitangabe
    return time;        // returne die zeit
}
