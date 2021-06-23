CXX= g++
CXXFLAGS= -Wall -Wextra -O3

sources=$(wildcard *.cpp)
objects=$(sources:%.cpp=%.o)
exec=threadproc

all: $(exec)

$(exec) : $(objects)
	$(CXX) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(objects) $(exec)
