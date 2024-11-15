CXX := g++
CXXFLAGS := -Iinclude -Wall -std=c++11
SRC_DIR := src
TEST_DIR := tests
OBJ_DIR := obj
BIN_DIR := bin

# Source files and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Test targets
TEST_TARGET1 := test
TEST_SRC1 := $(TEST_DIR)/test.cpp
TEST_TARGET2 := test_merge
TEST_SRC2 := $(TEST_DIR)/test_merge.cpp

.PHONY: all clean tests

all: $(BIN_DIR)/$(TEST_TARGET1) $(BIN_DIR)/$(TEST_TARGET2)

$(BIN_DIR)/$(TEST_TARGET1): $(TEST_SRC1) $(OBJS) $(OBJ_DIR)/TreeMerge.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/$(TEST_TARGET2): $(TEST_SRC2) $(OBJS) $(OBJ_DIR)/TreeMerge.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(BIN_DIR) $(OBJ_DIR)

tests: $(BIN_DIR)/$(TEST_TARGET1) $(BIN_DIR)/$(TEST_TARGET2)
	@echo Running tests...
	@./$(BIN_DIR)/$(TEST_TARGET1)
	@./$(BIN_DIR)/$(TEST_TARGET2)
