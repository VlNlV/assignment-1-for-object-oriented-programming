# Quick Overview:
- csvtest.cpp (tests EITHER the functionality of "csv.cpp" or "network.cpp" by testing out their methods. Currently the testing of "csv.cpp" is commented out and only "network.cpp" is being tested inside of the main function)
- csv.cpp (includes the functions for the CSVReader)
- csv.h (the headerfile for "csv.cpp" which includes the declarations of the methods)
- network.cpp (includes the functions for the "Network". I got a bit stuck here, so I got a bit "inspired" by the "sample solution")
- network.h (the headerfile for "network.cpp" which includes the declarations of the methods)
- types.h (already given file which is to be used when creating "network.cpp")

# Current Standpoint: Stuck at Error "terminate called after throwing an instance of 'std::out_of_range' what():  basic_string::substr: __pos (which is 6) > this->size() (which is 0)" (I don't how to overcome this error)
- Compilation of "network.cpp", "csvtest.cpp", and "csv.cpp" (together) works correctly, but when I try to execute it with the command line parameter of a path (that includes the GTFS files) it throws me the error.

# Error pic:
![Alt text](C:/Users/varun/Downloads/errorpic.png)

