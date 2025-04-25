reader:
	g++ -std=c++17 -o reader.exe csvtest.cpp csv.cpp network.cpp

autotest:
	g++ -I. -I/usr/local/include -std=c++17 -o /repo/test_runner /tester.cpp /usr/local/lib/libgtest_main.a /usr/local/lib/libgtest.a csv.cpp network.cpp
	
