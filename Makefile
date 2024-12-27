CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDFLAGS = -L./vcpkg_installed/x64-linux/lib -lyaml-cpp
INCLUDE = -I./vcpkg_installed/x64-linux/include

SRC_DIR = server/src
CSRC_DIR = client/src
BUILD_DIR = build

SERVER_SRC = $(SRC_DIR)/server.cpp $(SRC_DIR)/config.cpp $(SRC_DIR)/handler.cpp
SERVER_OBJ = $(BUILD_DIR)/server.o $(BUILD_DIR)/config.o $(BUILD_DIR)/handler.o

CLIENT_SRC = $(CSRC_DIR)/client.cpp
CLIENT_OBJ = $(BUILD_DIR)/client.o

.PHONY: all clean

all: $(BUILD_DIR)/server $(BUILD_DIR)/client

$(BUILD_DIR)/server: $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/client: $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(BUILD_DIR)/%.o: $(CSRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
