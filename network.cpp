#include "network.h"
#include "csv.h"
#include <iostream>
#include <algorithm> // [Added] For std::count
Network::Network(std::string mydirectory)
{
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
void Network::AgencyLoader(std::string path)
{
    CSVReader reader{path};
    while(reader.next()) // Edited- Use only next()
    {
        if (reader.getField("agency_id").empty() || reader.getField("agency_name").empty()) { // Added-Validate important fields
            std::cerr << "Warning: Invalid row in agency.txt. Skipping.\n";
            continue;
        }
        Agency fieldinfos = {
            .id = reader.getField("agency_id"),
            .name = reader.getField("agency_name"),
            .url = reader.getField("agency_url"),
            .timezone = reader.getField("agency_timezone"),
            .language = reader.getField("agency_lang"),
            .phone = reader.getField("agency_phone")
        };
        agencies[fieldinfos.id] = fieldinfos;
    }
}
void Network::CalendarDatesLoader(std::string path)
{
    CSVReader reader{path};
    while(reader.next())
    {
        if (reader.getField("service_id").empty() || reader.getField("date").empty()) {
            std::cerr << "Warning: Invalid row in calendar_dates.txt. Skipping.\n";
            continue;
        }
        CalendarDate fieldinfos = {
            .serviceId = reader.getField("service_id"),
            .date = DateCreator(reader.getField("date")),
            .exception = static_cast<CalendarDateException>(std::stoi(reader.getField("exception_type", "1")))
        };
        calendarDates.push_back(fieldinfos);
    }
}
void Network::CalendarsLoader(std::string path)
{
    CSVReader reader{path};
    while (reader.next())
    {
        if (reader.getField("service_id").empty()) {
            std::cerr << "Warning: Invalid row in calendar.txt. Skipping.\n";
            continue;
        }
        Calendar fieldinfos = {
            .serviceId = reader.getField("service_id"),
            .monday = static_cast<CalendarAvailability>(std::stoi(reader.getField("monday", "1"))),
            .tuesday = static_cast<CalendarAvailability>(std::stoi(reader.getField("tuesday", "1"))),
            .wednesday = static_cast<CalendarAvailability>(std::stoi(reader.getField("wednesday", "1"))),
            .thursday = static_cast<CalendarAvailability>(std::stoi(reader.getField("thursday", "1"))),
            .friday = static_cast<CalendarAvailability>(std::stoi(reader.getField("friday", "1"))),
            .saturday = static_cast<CalendarAvailability>(std::stoi(reader.getField("saturday", "1"))),
            .sunday = static_cast<CalendarAvailability>(std::stoi(reader.getField("sunday", "1"))),
            .startDate = DateCreator(reader.getField("start_date")),
            .endDate = DateCreator(reader.getField("end_date"))
        };
        calendars[fieldinfos.serviceId] = fieldinfos;
    }
}
void Network::FrequencyLoader(std::string path)
{
    // Not implemented yet
}
void Network::LevelsLoader(std::string path)
{
    CSVReader reader{path};
    while(reader.next())
    {
        Level fieldinfos = {
            .id = reader.getField("level_id"),
            .index = std::stoi(reader.getField("level_index")),
            .name = reader.getField("level_name")
        };
        levels[fieldinfos.id] = fieldinfos;
    }
}
void Network::PathwaysLoader(std::string path)
{
    CSVReader reader{path};
    while(reader.next())
    {
        Pathway fieldinfos = {
            reader.getField("pathway_id"),
            reader.getField("from_stop_id"),
            reader.getField("to_stop_id"),
            (EPathwayMode)std::stoi(reader.getField("pathway_mode", "1")),
            reader.getField("is_bidirectional") == "1",
            std::stof(reader.getField("length", "0.0")),
            (unsigned int)std::stoi(reader.getField("traversal_time", "0")),
            std::stoi(reader.getField("stair_count", "0")),
            std::stof(reader.getField("max_slope", "0.0")),
            std::stof(reader.getField("min_width", "0.0")),
            reader.getField("signposted_as")
        };
        pathways[fieldinfos.id] = fieldinfos;
    }
}
void Network::RoutesLoader(std::string path)
{
    CSVReader reader{path};
    while(reader.next())
    {
        Route fieldinfos = {
            reader.getField("route_id"),
            reader.getField("agency_id"),
            reader.getField("route_short_name"),
            reader.getField("route_long_name"),
            reader.getField("route_desc"),
            (RouteType)std::stoi(reader.getField("route_type", "0")),
            reader.getField("route_color"),
            reader.getField("route_text_color")
        };
        routes[fieldinfos.id] = fieldinfos;
    }
}
void Network::ShapesLoader(std::string path)
{
    CSVReader reader{path};
    while(reader.next())
    {
        Shape fieldinfos = {
            reader.getField("shape_id"),
            std::stod(reader.getField("shape_pt_lat")),
            std::stod(reader.getField("shape_pt_lon")),
            (unsigned int)std::stoi(reader.getField("shape_pt_sequence"))
        };
        shapes.push_back(fieldinfos);
    }
}


void Network::StopTimesLoader(std::string path) {
CSVReader reader{path};
 while (reader.hasNext()) 
 {
    if (reader.getField("trip_id").empty() || reader.getField("stop_id").empty()) { // [Added] Check if necessary fields exist before accessing
        std::cerr << "Warning: Invalid row in stop_times.txt. Skipping.\n";
        continue;
    }

    if (reader.next()) 
    {
    StopTime fieldinfos = 
    {
    reader.getField("trip_id"),
    TimeCreator(reader.getField("arrival_time")),
    TimeCreator(reader.getField("departure_time")),
    reader.getField("stop_id"),
    (unsigned int)std::stoi(reader.getField("stop_sequence")),
    (EPickupType)std::stoi(reader.getField("pickup_type")),
    (EDropOffType)std::stoi(reader.getField("drop_off_type")),
    reader.getField("stop_headsign")
    };
    stopTimes.push_back(fieldinfos);
    }
    }
  }

void Network::StopsLoader(std::string path) 
{
CSVReader reader{path};
while (reader.hasNext()) 
{
    if (reader.next()) 
    {

        if (reader.getField("stop_id").empty() || reader.getField("stop_name").empty()) { // [Added] Check if necessary fields exist before accessing
            std::cerr << "Warning: Invalid row in stops.txt. Skipping.\n";
            continue;
        }

    Stop fieldinfos = 
    {
    reader.getField("stop_id"),
    reader.getField("stop_code"),
    reader.getField("stop_name"),
    reader.getField("stop_desc"),
    std::stod(reader.getField("stop_lat")),
    std::stod(reader.getField("stop_lon")),
    (LocationType)std::stoi(reader.getField("location_type")),
    reader.getField("parent_station"),
    (WheelchairAccessibility)std::stoi(reader.getField("wheelchair_boarding")),
    reader.getField("platform_code"),
    reader.getField("level_id"),
    reader.getField("zone_id")
    };
    stops[fieldinfos.id] = fieldinfos;
    }
    }
  }

void Network::TransfersLoader(std::string path) 
{
CSVReader reader{path};
while (reader.hasNext()) 
{
    if (reader.next()) 
    {
    Transfer fieldinfos = 
    {
    reader.getField("from_stop_id"),
    reader.getField("to_stop_id"),
    reader.getField("from_route_id"),
    reader.getField("to_route_id"),
    reader.getField("from_trip_id"),
    reader.getField("to_trip_id"),
    (TransferType)std::stoi(reader.getField("transfer_type")),
    (unsigned int)std::stoi(reader.getField("min_transfer_time", "0"))
    };
    transfers.push_back(fieldinfos);
    }
    }
}

void Network::TripsLoader(std::string path) 
{
CSVReader reader{path};
while (reader.hasNext()) 
{
    if (reader.getField("trip_id").empty() || reader.getField("route_id").empty()) { // [Added] Check if necessary fields exist before accessing
        std::cerr << "Warning: Invalid row in trips.txt. Skipping.\n";
        continue;
    }

    if (reader.next()) 
    {
    Trip fieldinfos = 
    {
    reader.getField("trip_id"),
    reader.getField("route_id"),
    reader.getField("service_id"),
    reader.getField("trip_headsign"),
    reader.getField("trip_short_name"),
    (TripDirection)std::stoi(reader.getField("direction_id")),
    reader.getField("block_id"),
    reader.getField("shape_id"),
    (WheelchairAccessibility)std::stoi(reader.getField("wheelchair_accessible")),
    (BikesAllowed)std::stoi(reader.getField("bikes_allowed", "0")),
    };
    trips.push_back(fieldinfos);
    }
    }
}

GTFSDate Network::DateCreator(std::string timestamp) 
{
    return GTFSDate
    {
    .day = (unsigned char)std::stoi(timestamp.substr(6, 2)), 
    .month = (unsigned char)std::stoi(timestamp.substr(4, 2)),
    .year = (unsigned short)std::stoi(timestamp.substr(0, 4))
    };
  }
  
GTFSTime Network::TimeCreator(std::string timestamp) 
{
    size_t first = timestamp.find(":");
    size_t next = timestamp.find(":", first + 1);
    GTFSTime timeformat = 
    { 
    .hour = (unsigned char)std::stoi(timestamp.substr(0, first)),
    .minute = (unsigned char)std::stoi(timestamp.substr(first + 1, next - first)),
    .second = (unsigned char)std::stoi(timestamp.substr(next + 1))
    };
    return timeformat;
}
