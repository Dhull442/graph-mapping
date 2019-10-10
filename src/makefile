# Compiler to use
CXX = g++ -std=c++0x

# Flags
CXXFLAGS = -Wall -g -O3

# Executable name
OUT = main

all: $(OUT)

# Executable
$(OUT): main.o fileReader.o processGraphs.o
	$(CXX) main.o fileReader.o processGraphs.o -o main $(CXXFLAGS)

# Main
main.o: fileReader.h processGraphs.h

# FileReader
fileReader.o: fileReader.cpp fileReader.h
	$(CXX) -c fileReader.cpp -o $@

# processGraph
processGraphs.o: processGraphs.cpp fileReader.h processGraphs.h
	$(CXX) -c processGraphs.cpp -o $@

# Clean
clean:
	rm *.o
	rm $(OUT)
