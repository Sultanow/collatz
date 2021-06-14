CXX= g++
CXXFLAGS= -Wall -Wextra

sources=$(wildcard *.cpp)
objects=$(sources:%.cpp=%.o)
exec=threadproc

all: $(exec)

$(exec) : $(objects)
	$(CXX) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(objects) $(exec)
