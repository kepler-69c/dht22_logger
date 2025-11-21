CXX = g++
CXXFLAGS = -Wall -std=c++23 -O3
LDFLAGS = -l wiringPi

dht22: dht22.cpp dht22lib.cpp logger.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)