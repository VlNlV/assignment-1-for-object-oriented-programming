#include "network.h"
#include "csv.h"
#include <iostream>
#include <cctype>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <vector>
#include <climits>
#include <map>
#include <set>
#include <limits>
#include <cmath>

namespace bht {

std::string kleinschreiber(const std::string& wort)     // selbst-erstellte funktion fürs kleinschreiben von strings (damit suchwort + gesuchte daten an sich "gleich aussehen")
{
    std::string ergebnis_klein;                     // string variable fürs speichern des "verkleinerten" werts (zuerst mal ein leerer string)
    for (char einzelnes_zeichen : wort)             // range-based for schleife fürs durchgehen der einzelnen zeichen des zu-verkleinernden wortes
    {
        ergebnis_klein += std::tolower(einzelnes_zeichen);      // verkleinert das zeichen via "tolower" und hängt an den string (in dem das verkleinerte)
    }
    return ergebnis_klein;      // returnt den string, der jetzt aus dem verkleinerten "wort" besteht
}

int safeStoi(const std::string& value, int defaultValue = 0) // funktion fürs besseres konvertieren von string zu int
{
    if (value.empty()) return defaultValue;     // überprüft ob der wert überhaupt existiert, wenn nicht, dann printe den default value 
    try {                                   // try-and-catch-block (durch selbst-recherche!!!) --> der code block innerhalb des "try"-parts wird ausgeführt, und innerhalb des catch-blocks warten wir, ob entweder bestimmte "exceptions" (bzw. fehler) auftauchen, wenn ja, dann sagen wir, dass ein bestimmter code ausgeführt werden sollte
        return std::stoi(value);        // führe diesen befehl aus (also konvertiere den wert in einen int)
    } catch (...) {                 // mit "..." sagen wir, dass wir nach allen exceptions suchen
        return defaultValue;        // taucht also irgendein exception auf, dann printe den default value
    }
}

double safeStod(const std::string& value, double defaultValue = 0.0) {  // das gleiche wie bei safestoi aber jetzt mit string zu double
    if (value.empty()) return defaultValue;
    try {
        return std::stod(value);
    } catch (...) {
        return defaultValue;
    }
}

Network::Network(std::string mydirectory) {     // der konstruktur beim erstellen eines objekts führt weitere methoden aus, die einzeln die jeweiligen dateien verarbeiten
    AgencyLoader(mydirectory + "/agency.txt");      // z.B. wird zu "mydirectory" (der ein ordner zu allen txt files ist) den entsprechenden dateipfad hinzugefügt, um genau nur mit der datei zu arbeiten
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

void Network::AgencyLoader(std::string path) {          // methode zum verarbeiten der agency
    CSVReader reader(path);                         // CSVReader konstruktor bekommt den agency.txt pfad und die headers werden zunächst in einen vector gesteckt
    while (reader.next()) {
        Agency agency;                              // arbeiten mit dem struct aus types.h
        agency.id = reader.getField("agency_id");       // die einzelnen kategorien werden den entsprechenden werten via getfield zugeordnet
        agency.name = reader.getField("agency_name");
        agency.url = reader.getField("agency_url");
        agency.timezone = reader.getField("agency_timezone");
        agency.language = reader.getField("agency_lang");
        agency.phone = reader.getField("agency_phone");
        agencies[agency.id] = agency;                   // die unordered map "agencies" (bearbeitbar via einem key "agency.id") wird nun dem struct (mit den werten) zugewiesen
    }
}

void Network::CalendarDatesLoader(std::string path) {       // ähnlich wie oben, aber nur für calendardates
    CSVReader reader(path);
    while (reader.next()) {
        if (reader.getField("date").empty()) {              // zusätzlicher check, falls die "date" spalte leer ist
            continue;
        }
        CalendarDate calDate;
        calDate.serviceId = reader.getField("service_id");
        calDate.date = DateCreator(reader.getField("date"));
        calDate.exception = static_cast<CalendarDateException>(safeStoi(reader.getField("exception_type"), 1));     // nutzung von safestoi: also es wird dafür gesorgt, dass der wert unter "exception" RICHTIG in einen int konvertiert wird
        calendarDates.push_back(calDate);           // werte aus dem caldate struct werden nun in den calendarDates vector gepusht
    }
}

void Network::CalendarsLoader(std::string path) {       // ähnliches wie oben! siehe "agencyloader"
    CSVReader reader(path);
    while (reader.next()) {
        if (reader.getField("start_date").empty() || reader.getField("end_date").empty()) {
            continue;
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
        calendar.startDate = DateCreator(reader.getField("start_date"));
        calendar.endDate = DateCreator(reader.getField("end_date"));
        calendars[calendar.serviceId] = calendar;
    }
}

void Network::FrequencyLoader(std::string path) {}      // frequencies.txt ist sowieso leer, also braucht keinen funktionsrumpf

void Network::LevelsLoader(std::string path) {      // ähnliches wie oben! siehe "agencyloader"
    CSVReader reader(path);
    while (reader.next()) {
        Level level;
        level.id = reader.getField("level_id");
        level.index = safeStoi(reader.getField("level_index"));
        level.name = reader.getField("level_name");
        levels[level.id] = level;
    }
}

void Network::PathwaysLoader(std::string path) {        // ähnliches wie oben! siehe "agencyloader"
    CSVReader reader(path);
    while (reader.next()) {
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
        pathways[pathway.id] = pathway;
    }
}

void Network::RoutesLoader(std::string path) {      // ähnliches wie oben! siehe "agencyloader"
    CSVReader reader(path);
    while (reader.next()) {
        Route route;
        route.id = reader.getField("route_id");
        route.agencyId = reader.getField("agency_id");
        route.shortName = reader.getField("route_short_name");
        route.longName = reader.getField("route_long_name");
        route.description = reader.getField("route_desc");
        route.type = static_cast<RouteType>(safeStoi(reader.getField("route_type")));
        route.color = reader.getField("route_color");
        route.textColor = reader.getField("route_text_color");
        routes[route.id] = route;
    }
}

void Network::ShapesLoader(std::string path) {      // ähnliches wie oben! siehe "agencyloader"
    CSVReader reader(path);
    while (reader.next()) {
        Shape shape;
        shape.id = reader.getField("shape_id");
        shape.latitude = safeStod(reader.getField("shape_pt_lat"));
        shape.longitude = safeStod(reader.getField("shape_pt_lon"));
        shape.sequence = static_cast<unsigned int>(safeStoi(reader.getField("shape_pt_sequence")));
        shapes.push_back(shape);
    }
}

void Network::StopTimesLoader(std::string path) {       // ähnliches wie oben! siehe "agencyloader"
    CSVReader reader(path);
    while (reader.next()) {
        if (reader.getField("arrival_time").empty() || reader.getField("departure_time").empty()) {
            continue;
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
        stopTimes.push_back(stopTime);
        stopzutripsmap.insert(std::make_pair(stopTime.stopId, stopTime.tripId));
        tripzustopsmap.insert(std::make_pair(stopTime.tripId, stopTime.stopId));
        tripzustoptimes.insert(std::make_pair(stopTime.tripId, stopTime));
    }
}

void Network::StopsLoader(std::string path) {       // ähnliches wie oben! siehe "agencyloader"
    CSVReader reader(path);
    while (reader.next()) {
        Stop stop;
        stop.id = reader.getField("stop_id");
        stop.code = reader.getField("stop_code");
        stop.name = reader.getField("stop_name");
        stop.description = reader.getField("stop_desc");
        stop.latitide = safeStod(reader.getField("stop_lat"));
        stop.longitude = safeStod(reader.getField("stop_lon"));
        stop.locationType = static_cast<LocationType>(safeStoi(reader.getField("location_type")));
        stop.parentStation = reader.getField("parent_station");
        stop.wheelchairBoarding = static_cast<WheelchairAccessibility>(safeStoi(reader.getField("wheelchair_boarding")));
        stop.platformCode = reader.getField("platform_code");
        stop.levelId = reader.getField("level_id");
        stop.zoneId = reader.getField("zone_id");
        stops[stop.id] = stop;
        if (stop.locationType == LocationType_Stop && !stop.parentStation.empty()) {
        stopszustationsmap.insert(std::make_pair(stop.parentStation, stop.id));
      } 
      else {
        stopszustationsmap.insert(std::make_pair(stop.id, stop.id));
      }
    }
}

void Network::TransfersLoader(std::string path) {       // ähnliches wie oben! siehe "agencyloader"
    CSVReader reader(path);
    while (reader.next()) {
        Transfer transfer;
        transfer.fromStopId = reader.getField("from_stop_id");
        transfer.toStopId = reader.getField("to_stop_id");
        transfer.fromRouteId = reader.getField("from_route_id");
        transfer.toRouteId = reader.getField("to_route_id");
        transfer.fromTripId = reader.getField("from_trip_id");
        transfer.toTripId = reader.getField("to_trip_id");
        transfer.type = static_cast<TransferType>(safeStoi(reader.getField("transfer_type")));
        transfer.minTransferTime = static_cast<unsigned int>(safeStoi(reader.getField("min_transfer_time")));
        transfers.push_back(transfer);
    }
}

void Network::TripsLoader(std::string path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Unable to open trips.txt" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);  // skip header

    while (std::getline(file, line)) {
        std::stringstream linestream(line);
        std::string cell;
        Trip trip;

        std::getline(linestream, cell, ','); trip.routeId = cell;
        std::getline(linestream, cell, ','); trip.serviceId = cell;
        std::getline(linestream, cell, ','); trip.id = cell;
        std::getline(linestream, cell, ','); trip.headsign = cell;
        std::getline(linestream, cell, ','); trip.shortName = cell;
        std::getline(linestream, cell, ',');
        trip.direction = cell.empty() ? TripDirection_Inbound : static_cast<TripDirection>(std::stoi(cell));
        std::getline(linestream, cell, ','); trip.blockId = cell;
        std::getline(linestream, cell, ','); trip.shapeId = cell;
        std::getline(linestream, cell, ',');
        trip.wheelchairAccessible = cell.empty() ? WheelchairAccessibility_NoInformation : static_cast<WheelchairAccessibility>(std::stoi(cell));
        std::getline(linestream, cell, ',');
        trip.bikesAllowed = cell.empty() ? BikesAllowed_NoInformation : static_cast<BikesAllowed>(std::stoi(cell));

        trips.push_back(trip);
    }

    file.close();
}


GTFSDate Network::DateCreator(std::string timestamp) {      // funktion fürs richtige parsen des datums
    GTFSDate date;
    if (timestamp.length() < 8) {           // check, ob das datum weniger als 8 zeichen sind (also nicht YYYYMMDD)
        date.day = 0;                   // falls das der fall ist, dann wird das ganze datum zu 0
        date.month = 0;
        date.year = 0;
        return date;
    }
    date.day = (unsigned char)(safeStoi(timestamp.substr(6, 2)));       // extrahiere die einzelnen datumselemente aus dem timestamp und konvertiere sie mit safestoi (z.B. 19690420 --> "day" ist index 6 und nimmt 2 zeichen also "20", "month" ist index 4 und nimmt 2 zeichen also "04", "year" ist index 0 und nimmt 4 zeichen also "1969")
    date.month = (unsigned char)(safeStoi(timestamp.substr(4, 2)));
    date.year = (unsigned short)(safeStoi(timestamp.substr(0, 4)));
    return date;
}

GTFSTime Network::TimeCreator(std::string timestamp) {      // funktion fürs richtige parsen der zeitangabe
    GTFSTime time;
    if (timestamp.length() < 5 || timestamp.find(":") == std::string::npos) {       // check, falls die zeit weniger als 5 zeichen ist oder kein ":" gefunden werden kann (bei keinem ":" gibt "find" std::string::npos)
        time.hour = 0;          // dann setze alle auf 0
        time.minute = 0;
        time.second = 0;
        return time;
    }
    size_t first = timestamp.find(":");             // "first" wird der index vom ersten ":" also "2"
    size_t next = timestamp.find(":", first + 1);           // next wird der index vom zweiten ":" also "5"

    time.hour = (unsigned char)(safeStoi(timestamp.substr(0, first)));      // zeit elemente werden extrahiert, z.B. 17:38:42, wobei "hour" beim index 0 dann 2 zeichen nimmt also "17", "minute" geht zum index 3 und nimmt 2 zeichen, "second" geht zum index 6 und nimmt einfach den rest des strings, was einfach die letzte ziffer ist
    time.minute = (unsigned char)(safeStoi(timestamp.substr(first + 1, next - first - 1)));
    time.second = (unsigned char)(safeStoi(timestamp.substr(next + 1)));
    return time;
}

std::vector<Stop> Network::search(const std::string& suchwort)      // methoden-definition von der suchfunktion (mit suchwort als argument, wobei "const" und "&" dazu dienen, dass das suchwort nicht geändert und nicht kopiert wird, also nur referenziert wird)
{
    std::vector<Stop> ergebnisvector;                               // vector "ergebnisvector" der aus den stop-objekten besteht
    std::string suchwort_klein = kleinschreiber(suchwort);          // IDEE: mach alle suchwörter + daten (daten, welche dann suchergebnisse werden) klein, damit sie identisch zu einander sind
                                                                    // also das suchwort wird via einer selbsterstellten funktion (kleinschreiber) kleingeschrieben und es resultiert das GLEICHE wort aber halt kleingeschrieben
    for (const auto& [id, stop] : stops)        // range based for schleife fürs durchgehen der inhalte des unordered maps "stops" (einteilung in die "id" und der rest des stop-objekts)
    {
        std::string haltestelle_klein = kleinschreiber(stop.name);      // verkleinert die haltestellen-namen
        if(haltestelle_klein.find(suchwort_klein) != std::string::npos)     // if-check für wenn das VERKLEINERTE suchwort innerhalb der haltestellen-namen zu finden sind (wenn nicht gefunden, gibt "find" dann "std::string::npos" zurück)
        {                                                                      // "find" sucht gerade einen string INNERHALB eines strings, also ist der "ich-hab-nichts-gefunden"-wert ein "std::string:npos"
            ergebnisvector.push_back(stop);     // füge den entsprechenden haltestellen-namen dann in den ergebnisvector
        }
    }
    return ergebnisvector;  // wenn for schleife fertig ist, dann returne den vector
}

std::vector<bht::Route> Network::getRoutes()    // definition der funktion "getRoutes"
{
  std::vector<bht::Route> routevector;          // erstellung eines vektors "routevector" (speicherung von route-objekten)
  for (const auto& [id, route] : routes)        // range-based for schleife fürs durchgehen der route-objekte und einteilen in ihre key und value (bzw. rest des objekts) paare
  {
    routevector.push_back(route);               // der key mit dem rest des objekts (einteilung --> key und value) wird dann in routevector (siehe oben) reingespusht
  } 
  return routevector;                           // nachdem die for schleife vorüber ist (also alle route-objekte mit ihren keys bzw. IDs jetzt im routevector sind), printe den gesamten vector
}

std::string Network::getRouteDisplayName(bht::Route route)  // definition der funktion "getDisplayName" (wichtig hier: printet den short name einer route, falls aber auch noch long name vorhanden ist, dann printet er beide)
{
    if (route.longName.empty())     // if bedingung check, ob long name "feld" leer ist
    {
        return route.shortName;     // wenn ja, dann printe (nur) den short name
    } 
    else                               // wenn obige if bedingung false war
    {
        return route.shortName + " - " + route.longName;       // printe beide mit format (aus der aufgabe) "short name - long name"
    }
}

std::vector<Trip> Network::getTripsForRoute(std::string routeId)    // funktion definition von "getTripsForRoute" (welche eine routeId als argument bekommt)
{
    std::vector<Trip> passendeTrips;        // vector aus trip-objekten namens "passendeTrips" (wie name schon sagt, sollen hier die passenden trips zu einer ausgewähnten linie gepackt werden)
    for (const auto& trip : trips)          // range based for schleife fürs durchgehen der trips (ein einzelner trip wird temporär in "trip" gespeichert)
    {
        if(trip.routeId == routeId)         // zu einem einzelnen trip wird überprüft, ob seine routeId dem argument "routeId" passt
        {
            passendeTrips.push_back(trip);  // wenn ja, dann füge den trip zum vector (von vorhin)
        }
    }
    return passendeTrips;           // am ende (wenn alle vorhandenen und passende trips hinzugefügt wurden) printe den gesamten vector 
}

std::string Network::getTripDisplayName(bht::Trip trip)     // funktion definition von "getTripDisplayName" (der einen trip als argument bekommt)
{
    std::string shortName = trip.shortName;     // speicherung der shortname/headsign in variablen (zur weiternutzung)
    std::string headsign = trip.headsign;

    if (shortName.empty())      // anmerkung über die daten: sofern ich weiß, gibt es immer einen shortname aber nur manchmal keinen headsign, aber sicherheitshalber wurde ein sicherheits-checks gemacht, falls ein shortname fehlen WÜRDE
    {                           // also wenn der shortName fehlt (nicht der fall, sofern ich weiß)
        return headsign;        // printe den headsign
    }

    if (headsign.empty())       // wenn headsign fehlt (kann passieren!)
    {
        return shortName;       // printe den shortname
    }

    return shortName + " - " + headsign;       // wenn die beiden if-bedingungen nicht stimmen (also beide, shortname UND headsign sind vorhanden, was öfters der fall ist) sollen beide geprintet werden mit "-" getrennt
}

bool StopSequenceComparisonHelper(const bht::StopTime& erstesElement, const bht::StopTime& zweitesElement)  // definition der helper function "stopsequencecomparisonhelper", welches zwei stoptime objekte bekommt
{
    return erstesElement.stopSequence < zweitesElement.stopSequence;    // gibt beide zurück mit der reihenfolge, kleinere stopsequence dann größere stopsequence
}

std::vector<StopTime> Network::getStopTimesForTrip(std::string& tripId) // funktion definition von "getStopTimesForTrip", die von einer tripId alle zugehörigen StopTimes-objekte holt
{
    std::vector<StopTime> stoptimesfortrips;    // vector "stoptimesfortrips" von stoptimes-objekten, in dem später die stoptimes des bereits gewählten trips speichert werden

    for(const auto& stopTime : stopTimes)       // range-based for schleife: geht durch alle stoptimes
    {
        if(stopTime.tripId == tripId)           // if bedingung: checkt, ob die tripId einer stoptime mit dem argument "tripId" übereinstimmt
        {
            stoptimesfortrips.push_back(stopTime);  // falls ja, dann füg das stoptime-objekt in den vector von vorhin
        }
    }
    std::sort(stoptimesfortrips.begin(), stoptimesfortrips.end(), StopSequenceComparisonHelper);    // via sort --> werden die haltestellen nach ihrer stopsequence sortiert (mithilfe einer helper function "stopsequencecomparisonhelper")
    return stoptimesfortrips;   // returne den sortierten vector mit den stoptimes
}

Stop Network::getStopById(const std::string& stopId)    // definition der funktion "getStopById", die von einer stopId den namen der haltestelle bekommt
{
    auto positionfinder = stops.find(stopId);       // via find --> sucht nach der position des elements in stops mit der bestimmten "stopId"
    if (positionfinder != stops.end())          // wenn "find" etwas findet, dann wird nicht "stops.end()" zurückgegeben, also hier wird gecheckt, ob etwas passendes gab
    {                                           // "find" wurde oben benutzt, um einen string in einem unordered map zu finden, also ist der "ich-habe-keinen-wert-gefunden" ein "stops.end()"
        return positionfinder->second;          // da stops eine unordered map ist, ist "second" einfach der wert des zugehörigen schlüssels ("first"), also soll die positionfinder nun auf die haltestelle der stopId zeigen
    }

    return Stop{};          // wenn keine haltestelle zu einer stopId gefunden wurde, dann returne einfach ein leeres stop objekt
}

std::vector<StopTime> Network::searchStopTimesForTrip(std::string query, std::string tripId)    // funktion definition von "searchStopTimesForTrip", welches via der query (suchwort) und der tripId die bestimmten stopTimes findet
{
    std::vector<StopTime> passendeResultsZuTripUndQuery;    // vector, in dem die ergebnisse am ende gespeichert werden

    for (const auto& stopTime : stopTimes)      // range based for schleife: fürs durchgehen der stoptime-objekte
    {
        if (stopTime.tripId == tripId)          // erster check für übereinstimmung (wenn tripId eines stopTime-objekts mit der tripId (funktionsargument) übereinstimmt
        {
            Stop stop = getStopById(stopTime.stopId);   // via der stopId des bestimmten stopTime-objekts wird der entsprechende rest des stop-objekts in "stop" gespeichert
            if (stop.name.find(query) != std::string::npos) // zweiter check für übereinstimmung: wenn die query (suchwort) innerhalb des stop namen ist ("find" sucht einen string innerhalb eines strings, also ist der "ich-habe-keinen-wert-gefunden"-wert ein "std::string::npos)
            {
                passendeResultsZuTripUndQuery.push_back(stopTime);  // füge das bestimmte stopTime objekt in den vector von vorhin
            }
        }
    }

    return passendeResultsZuTripUndQuery;   // returne den endgültigen vector
}

std::vector<bht::Stop> Network::getStopsForTransfer(const std::string& stopId)  // methode fürs printen der zugehörigen gleise/bahnhöfe zu einer gegebenen ID (plan: wenn bahnhof ID gegeben wird, dann printe diese erneut und ihre zugehörigen gleise und wenn eine einzelne gleis-ID gegeben wird, dann printe ihren zugehörigen bahnhof mit anderen zugehörigen gleisen zu diesem bahnhof also die "geschwister"-gleisen)
{
    std::vector<bht::Stop> vektorfuerID;    // vektor der am ende alles passende in sich speichert

    auto iteratorderaufstopzeigt = stops.find(stopId);  // da stops eine unordered map ist, arbeiten wir hier mit iteratoren (mittels "find" suchen wir die gegebene id im gesamten stops-struct, der iterator zeigt dann auf das stop-objekt mit der genau erscheinenden stopId aus funktions-argument)
    if (iteratorderaufstopzeigt == stops.end()) // wenn der iterator nichts findet, zeigt er aufs ende, also hier die abfrage, wenn nichts gefunden wurde
    {
        return vektorfuerID;        // printe dann den leeren vektor von oben
    }
    const bht::Stop& ID_identifikator = iteratorderaufstopzeigt->second;    // wenn tatsächlich was gefunden wurde, dann ist der ID_identifikator der rest des stop-objekt vom iterator (bei unordered maps ist das in "second" gespeichert!!)

    std::string IDmitderwirfiltern;     // ne variable für die ID filterung
    if (ID_identifikator.locationType == LocationType_Station)     // hier ne abfrage ob die gefiltere ID ein bahnhof ist (via locationType == 1)-check
    {
        IDmitderwirfiltern = ID_identifikator.id;   // dann wird die ID des identifikators des stops zur ID zur filterung
    }
    else                                            // wenn nicht der fall, also dann ist stopId wohl ein gleis
    {
        IDmitderwirfiltern = ID_identifikator.parentStation;    // dann wird die parent station des gleises zur ID zur filterung
    }

    for(const auto& [id, stop] : stops)     // wir gehen durch alle stops
    {
        if ( (stop.locationType == LocationType_Station && stop.id == IDmitderwirfiltern) || (stop.locationType == LocationType_Stop && stop.parentStation == IDmitderwirfiltern)) // hier eine fallunterscheidung gemacht, um die bahnhöfe mit locationType == 1 und der ID der filterungs-ID oder die gleise mit locationType == 0 und einer parent station der filterungs-ID zu finden
        {
            vektorfuerID.push_back(stop);   // wenn was gefunden, dann packe es in den vektor
        }

    }
    return vektorfuerID;    // returne den vektor
}

std::unordered_set<std::string> Network::getNeighbors(const std::string& stopId) {
    std::unordered_set<std::string> neighbors;

    for (const auto& [tripId, stopTime] : tripzustoptimes) {
        if (stopTime.stopId == stopId) {
            auto range = tripzustoptimes.equal_range(tripId);
            for (auto it = range.first; it != range.second; ++it) {
                if (it->second.stopSequence == stopTime.stopSequence + 1) {
                    neighbors.insert(it->second.stopId);
                }
            }
        }
    }

    return neighbors;
}

std::vector<bht::Stop> Network::getTravelPath(const std::string& fromStopId, const std::string& toStopId) {
    std::unordered_map<std::string, std::string> predecessor;
    std::unordered_map<std::string, int> distance;
    std::unordered_set<std::string> visited;

    distance[fromStopId] = 0;

    // Priority queue for Dijkstra: (distance, stopId)
    std::priority_queue<std::pair<int, std::string>,
                        std::vector<std::pair<int, std::string>>,
                        std::greater<>> queue;
    queue.push({0, fromStopId});

    while (!queue.empty()) {
        auto [dist, current] = queue.top();
        queue.pop();

        if (visited.count(current)) continue;
        visited.insert(current);

        if (current == toStopId) break;

        for (const auto& neighbor : getNeighbors(current)) {
            int newDist = dist + 1;
            if (!distance.count(neighbor) || newDist < distance[neighbor]) {
                distance[neighbor] = newDist;
                predecessor[neighbor] = current;
                queue.push({newDist, neighbor});
            }
        }
    }

    // Reconstruct path
    std::vector<std::string> pathIds;
    std::string current = toStopId;
    while (predecessor.count(current)) {
        pathIds.push_back(current);
        current = predecessor[current];
    }

    if (current != fromStopId) {
        std::cout << "bruh es gibt keinen weg von " << fromStopId << " bis " << toStopId << std::endl;
        return {};
    }

    pathIds.push_back(fromStopId);
    std::reverse(pathIds.begin(), pathIds.end());

    std::vector<bht::Stop> path;
    for (const auto& id : pathIds) {
        path.push_back(getStopById(id));
    }
    return path;
}

NetworkScheduledTrip Network::getScheduledTrip(const std::string& tripId) const // methode für übergabe einer tripId und rückgabe eines NetworkScheduledTrips
{
    const Trip* gefundenertrip = nullptr;       // zeiger auf die tripsuche (aber zunächstmal auf "nullptr" damit er auf nichts zeigt)

    for(const Trip& einzelnefahrt : trips)      // durchgehen der trips
    {
        if (einzelnefahrt.id == tripId)         // herausfiltern des gesuchten trips (übergabe-parameter)
        {
            gefundenertrip = &einzelnefahrt;    // passende fahrt zeigt nun drauf
            break;                              // wir habens also gefunden, schleife wird gebroken
        }
    }
    if(!gefundenertrip)         // wenn kein passender trip gefunden werden konnte, dann printe customized fehler
    {
        throw std::runtime_error("naja hab keine trip id gefunden bruh: " + tripId);    // mein customized fehler
    }    

    std::vector<StopTime> passendestoptimes;        // vektor für die passenden stoptimes
    
    for(const StopTime& einzelnestoptime : stopTimes)       // geht durch die stoptimes
    {
        if(einzelnestoptime.tripId == tripId)       // falls passende stoptimes mit der übergebenen tripId übereinstimmt
        {
            passendestoptimes.push_back(einzelnestoptime);      // füge diese bestimmte stoptime in den vektor
        }
    }
    
    std::sort(passendestoptimes.begin(), passendestoptimes.end(), [](const StopTime& a, const StopTime& b){ return a.stopSequence < b.stopSequence;});      // sortieren mittels "sort" der stoptimes in fahrtrichtung

    return NetworkScheduledTrip(*gefundenertrip, passendestoptimes);     // erzeugung und rückgabe des NetworkScheduledTrip objekts mit übergabe der tripId und den passenden stoptimes in reihenfolge (in fahrtrichtung)

}

}

