# an overview of the current stand of things:

## Assignment 4 expectations:
- part a) 
    - Method --> std::vector<bht::Stop> getStopsForTransfer(const std::string& stopId)
        - update: should work
- part b)
    - Method --> std::unordered_set<std::string> getNeighbors(const std::string& stopId)
        - update: should work
    - Method --> std::vector<bht::Stop> getTravelPath(const std::string& fromStopId, const std::string& toStopId)
        - update: for whatever reason detects no valid travelpath for any two points (keeps printing my custom error message)  
- part c)
    - Method --> NetworkScheduledTrip getScheduledTrip(const std::string& tripId) const;
        - update: currently does "terminate called after throwing an instance of 'std::logic_error' what():  basic_string::_M_construct null not valid"


## this version of getNeighbors and getStopsTransfer were inspired by the sample solution (because my own versions would crash in the automatic test feedback)
