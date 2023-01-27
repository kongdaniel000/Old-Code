CXX=clang++
CXXFLAGS=-g -std=c++11 -Wall

all: raytracer
clean:
	rm -f *.o *.h.gch raytracer raytracer.ppm
test: raytracer
	./raytracer
.PHONY: all clean test

raytracer: raytracer.o
	$(CXX) $(LDFLAGS) -o $(@) $(^)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $(@) $(<)