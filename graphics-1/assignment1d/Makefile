SRC_DIR := ./src
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
CXX := clang++
CXXFLAGS := -g -std=c++11 -Wall

raytracer: $(OBJ_FILES)
	@$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@rm -f *.ppm
	@rm -f raytracer 
	@cd $(OBJ_DIR); rm -f *.o
.PHONY: clean