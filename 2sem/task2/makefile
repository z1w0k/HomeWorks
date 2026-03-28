CXX = g++
CXXFLAGS = -Wall -std=c++11 -g
TARGET = graph
SOURCES = main.cpp
HEADERS = Graph.h
OBJECTS = main.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

main.o: main.cpp Graph.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

test: $(TARGET)
	./$(TARGET) test

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all test clean
