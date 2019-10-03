# Compiler to use
CXX = g++ -std=c++11

# Flags
CXXFLAGS = -Wall -g

# Executable name
OUT = main

all: $(OUT)

# Executable
$(OUT): main.o 
	$(CXX) main.o -o main $(CXXFLAGS)

# Main
main.o: fileReader.cpp fileReader.h
	$(CXX) -c fileReader.cpp -o $@