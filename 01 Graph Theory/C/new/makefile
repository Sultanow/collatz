CXX= g++
CXXFLAGS= -fopenmp -Wall -Wextra -O3
LDFLAGS= -lpthread

sources=$(wildcard *.cpp)
objects=$(sources:%.cpp=%.o)
exec=threadproc

all: $(exec)

$(exec) : $(objects)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -rf $(objects) $(exec)
