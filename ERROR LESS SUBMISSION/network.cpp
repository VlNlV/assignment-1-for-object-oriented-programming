#include "network.h"
#include "csv.h"
#include <iostream>

// Helper function to safely convert string to int
int safeStoi(const std::string& value, int defaultValue = 0) {
    if (value.empty()) return defaultValue;
    try {
        return std::stoi(value);
    } catch (...) {
        return defaultValue;
    }
}

// Helper function to safely convert string to double
double safeStod(const std::string& value, double defaultValue = 0.0) {
    if (value.empty()) return defaultValue;
    try {
        return std::stod(value);
    } catch (...) {
        return defaultValue;
    }
}

Network::Network(std::string mydirectory) {
    AgencyLoader(mydirectory + "/agency.txt");
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

void Network::AgencyLoader(std::string path) {
    CSVReader reader(path);
    while (reader.next()) {
        Agency agency;
        agency.id = reader.getField("agency_id");
        agency.name = reader.getField("agency_name");
        agency.url = reader.getField("agency_url");
        agency.timezone = reader.getField("agency_timezone");
        agency.language = reader.getField("agency_lang");
        agency.phone = reader.getField("agency_phone");
        agencies[agency.id] = agency;
    }
}

void Network::CalendarDatesLoader(std::string path) {
    CSVReader reader(path);
    while (reader.next()) {
        if (reader.getField("date").empty()) {
            continue;
        }
        CalendarDate calDate;
        calDate.serviceId = reader.getField("service_id");
        calDate.date = DateCreator(reader.getField("date"));
        calDate.exception = static_cast<CalendarDateException>(safeStoi(reader.getField("exception_type"), 1));
        calendarDates.push_back(calDate);
    }
}

void Network::CalendarsLoader(std::string path) {
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

void Network::FrequencyLoader(std::string path) {}

void Network::LevelsLoader(std::string path) {
    CSVReader reader(path);
    while (reader.next()) {
        Level level;
        level.id = reader.getField("level_id");
        level.index = safeStoi(reader.getField("level_index"));
        level.name = reader.getField("level_name");
        levels[level.id] = level;
    }
}

void Network::PathwaysLoader(std::string path) {
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

void Network::RoutesLoader(std::string path) {
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

void Network::ShapesLoader(std::string path) {
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

void Network::StopTimesLoader(std::string path) {
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
    }
}

void Network::StopsLoader(std::string path) {
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
    }
}

void Network::TransfersLoader(std::string path) {
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
    CSVReader reader(path);
    while (reader.next()) {
        Trip trip;
        trip.id = reader.getField("trip_id");
        trip.routeId = reader.getField("route_id");
        trip.serviceId = reader.getField("service_id");
        trip.headsign = reader.getField("trip_headsign");
        trip.shortName = reader.getField("trip_short_name");
        trip.direction = static_cast<TripDirection>(safeStoi(reader.getField("direction_id")));
        trip.blockId = reader.getField("block_id");
        trip.shapeId = reader.getField("shape_id");
        trip.wheelchairAccessible = static_cast<WheelchairAccessibility>(safeStoi(reader.getField("wheelchair_accessible")));
        trip.bikesAllowed = static_cast<BikesAllowed>(safeStoi(reader.getField("bikes_allowed")));
        trips.push_back(trip);
    }
}

GTFSDate Network::DateCreator(std::string timestamp) {
    GTFSDate date;
    if (timestamp.length() < 8) {
        date.day = 0;
        date.month = 0;
        date.year = 0;
        return date;
    }
    date.day = (unsigned char)(safeStoi(timestamp.substr(6, 2)));
    date.month = (unsigned char)(safeStoi(timestamp.substr(4, 2)));
    date.year = (unsigned short)(safeStoi(timestamp.substr(0, 4)));
    return date;
}

GTFSTime Network::TimeCreator(std::string timestamp) {
    GTFSTime time;
    if (timestamp.length() < 5 || timestamp.find(":") == std::string::npos) {
        time.hour = 0;
        time.minute = 0;
        time.second = 0;
        return time;
    }
    size_t first = timestamp.find(":");
    size_t next = timestamp.find(":", first + 1);

    time.hour = (unsigned char)(safeStoi(timestamp.substr(0, first)));
    time.minute = (unsigned char)(safeStoi(timestamp.substr(first + 1, next - first - 1)));
    time.second = (unsigned char)(safeStoi(timestamp.substr(next + 1)));
    return time;
}
