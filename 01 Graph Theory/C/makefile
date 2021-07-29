CXX= g++
CXXFLAGS= -Wall -Wextra -O3 -fopenmp

sources=$(wildcard *.cpp)
objects=$(sources:%.cpp=%.o)
exec=threadproc

all: $(exec)

$(exec) : $(objects) 
	$(CXX) $^ -o $@ $(CXXFLAGS)

clean:
	rm -rf $(objects) $(exec)
