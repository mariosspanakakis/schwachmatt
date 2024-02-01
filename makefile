# directory definitions
SRCDIR=src
TESTDIR=tests

CXX      = g++
CXXFLAGS = -Wall -std=c++14
LDFLAGS  =

GTEST_DIR = lib/googletest
GTEST_LIBS = -lgtest -lgtest_main -lpthread
GTEST_INCLUDE = -I$(GTEST_DIR)/include -I$(SRCDIR)


# executable name
EXEC = main
TEST_EXEC = test_main

# source files
MAIN_SOURCES  = $(SRCDIR)/main.cpp $(SRCDIR)/bitboard.cpp $(SRCDIR)/attacks.cpp $(SRCDIR)/board.cpp $(SRCDIR)/utils.cpp
TEST_SOURCES = $(TESTDIR)/main_test.cpp

# object files
MAIN_OBJECTS  = $(MAIN_SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

# header dependencies
HEADERS  = $(SRCDIR)/main.hpp $(SRCDIR)/bitboard.hpp $(SRCDIR)/attacks.hpp $(SRCDIR)/board.hpp $(SRCDIR)/utils.hpp

# default target
all: $(EXEC)

# link object files to create executable
$(EXEC): $(MAIN_OBJECTS)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJECTS) -o $@ $(LDFLAGS)

# test target
test: $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $(TEST_EXEC) -L$(GTEST_DIR)/lib $(GTEST_LIBS)

# compile main source files to object files
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# compile test source files to object files
$(TESTDIR)/%.o: $(TESTDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE) -c $< -o $@

# clean target
clean:
	rm -f $(EXEC) $(MAIN_OBJECTS) $(TEST_EXEC) $(TEST_OBJECTS)

# phony targets
.PHONY: all clean test