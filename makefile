# directory definitions
SRCDIR=src
TESTDIR=tests

CXX      = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS  =

GTEST_DIR = lib/googletest
GTEST_LIBS = -lgtest -lgtest_main -lpthread
GTEST_INCLUDE = -I$(GTEST_DIR)/include -I$(SRCDIR)

# executable names
EXEC = main
TEST_EXEC = test

# source files
MAIN_SOURCES  = $(SRCDIR)/main.cpp $(SRCDIR)/bitboard.cpp $(SRCDIR)/attacks.cpp $(SRCDIR)/board.cpp $(SRCDIR)/utils.cpp $(SRCDIR)/move.cpp $(SRCDIR)/movegen.cpp
TEST_SOURCES = $(TESTDIR)/unit_test.cpp

# object files
MAIN_OBJECTS  = $(MAIN_SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o) $(SRCDIR)/bitboard.o $(SRCDIR)/attacks.o $(SRCDIR)/board.o $(SRCDIR)/utils.o $(SRCDIR)/move.o $(SRCDIR)/movegen.o

# header dependencies
HEADERS  = $(SRCDIR)/main.h $(SRCDIR)/bitboard.h $(SRCDIR)/attacks.h $(SRCDIR)/board.h $(SRCDIR)/utils.h $(SRCDIR)/move.h $(SRCDIR)/movegen.h

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
