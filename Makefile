CXX := g++
CXXFLAGS := -Iinclude -Wall -std=c++11
SRC_DIR := src
TEST_DIR := tests
OBJ_DIR := obj
BIN_DIR := bin

# Source files and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Test target
TEST_TARGET := test
TEST_SRC := $(TEST_DIR)/test.cpp

.PHONY: all clean

all: $(BIN_DIR)/$(TEST_TARGET)

$(BIN_DIR)/$(TEST_TARGET): $(TEST_SRC) $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(BIN_DIR) $(OBJ_DIR)
