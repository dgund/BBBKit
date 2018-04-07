CXX = g++
CXXFLAGS = -g -fPIC -Wall -Wextra
LDFLAGS = -shared -lpthread

PROJECT = libbbbkit
TARGET = $(PROJECT).so
HEADERS = $(wildcard *.h)
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

INSTALL_INCLUDE_DIR = /usr/local/include
INSTALL_LIB_DIR = /usr/local/lib

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

install:
	mkdir -p $(INSTALL_INCLUDE_DIR)/$(PROJECT)
	cp $(HEADERS) $(INSTALL_INCLUDE_DIR)/$(PROJECT)
	cp $(TARGET) $(INSTALL_LIB_DIR)/

uninstall:
	rm -rf $(INSTALL_INCLUDE_DIR)/$(PROJECT)
	rm -f $(INSTALL_LIB_DIR)/$(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)

