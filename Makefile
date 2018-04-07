CXX = g++
CXXFLAGS = -g -fPIC -Wall -Wextra
LDFLAGS = -shared

TARGET = BBBKit.so
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)

