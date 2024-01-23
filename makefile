# compiler and compiler flags
CXX=g++
CXXFLAGS=-Wall -std=c++17

# directory definitions
SRCDIR=src

# build target definition
TARGET=main

# existing source files
SRC=$(wildcard $(SRCDIR)/*.cpp)

# build rule for the target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# clean rule to remove compiled files
clean:
	rm -f $(TARGET) *.o