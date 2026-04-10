CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
TARGET = university
SOURCES = main.cpp Student.cpp HumanitiesStudent.cpp TechnicalStudent.cpp ScienceStudent.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
