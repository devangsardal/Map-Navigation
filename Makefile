CXX = g++
CXXFLAGS = -Wall -g

default: navigation

navigation: main.o Navigator.o support.o SegmentMapper.o AttractionMapper.o MapLoader.o
	$(CXX) $(CXXFLAGS) -o navigation main.o Navigator.o support.o SegmentMapper.o AttractionMapper.o MapLoader.o

main.o: 
	$(CXX) $(CXXFLAGS) -c main.cpp

Navigator.o: 
	$(CXX) $(CXXFLAGS) -c Navigator.cpp

support.o: 
	$(CXX) $(CXXFLAGS) -c support.cpp

SegmentMapper.o: 
	$(CXX) $(CXXFLAGS) -c SegmentMapper.cpp

AttractionMapper.o: 
	$(CXX) $(CXXFLAGS) -c AttractionMapper.cpp

MapLoader.o: 
	$(CXX) $(CXXFLAGS) -c MapLoader.cpp

clean: 
	rm *.o navigation

clean-objects:
	rm *.o