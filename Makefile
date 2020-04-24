CXXFLAGS = -std=c++1z -Wall

SOURCES=$(wildcard *.cpp)
BINARIES=$(patsubst %.cpp,./%,$(SOURCES))

default: $(BINARIES)

./%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(BINARIES)