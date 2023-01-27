CXX=clang++
CXXFLAGS=-g -std=c++11 -Wall

all: phong
clean:
	rm -f *.o *.h.gch phong phong.ppm
test: phong
	./phong
.PHONY: all clean test

phong: phong.o
	$(CXX) $(LDFLAGS) -o $(@) $(^)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $(@) $(<)