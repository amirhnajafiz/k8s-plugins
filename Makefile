CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDFLAGS =

SRC_DIR = server/src
BUILD_DIR = build

SERVER_SRC = $(SRC_DIR)/server.cpp
SERVER_OBJ = $(BUILD_DIR)/server.o

.PHONY: all clean

all: $(BUILD_DIR)/server

$(BUILD_DIR)/server: $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
