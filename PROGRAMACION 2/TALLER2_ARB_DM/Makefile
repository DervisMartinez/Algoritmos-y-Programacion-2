CXX = g++
CXXFLAGS = -std=c++98 -Wall -Ilib
CXXFLAGS = -std=c++11 -Wall -Ilib
TARGET = arboles
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)