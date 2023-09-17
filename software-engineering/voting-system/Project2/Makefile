SRC_DIR := ./src
OBJ_DIR := ./misc/obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cc)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SRC_FILES))
CXX := clang++
CXXFLAGS := -g -std=c++11 -Wall

votingMain.out: $(OBJ_FILES)
	@$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@rm -f votingMain.out 
	@cd $(OBJ_DIR); rm -f *.o
.PHONY: clean