CXX = g++
CXXFLAGS = -g -Wall

LD = g++
LDFLAGS = -lpthread

TARGET = BBBKit

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.c=.o)

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)

