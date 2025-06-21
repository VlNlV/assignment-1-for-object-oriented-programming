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

# this directory also contains tester.cpp which includes the test file which the automatic feedback systeme uses. Seemingly the example it tests with "getTravelPath" doesnt lead to the expected output (in my terminal), so the main objective would be to find a way for the program to print the correct expected output for the getTravelPath. (for example, according to tester.cpp the specific pair of the start-station and stop-station should lead to a path of 12 stations but actually leads to 0 according my program)
