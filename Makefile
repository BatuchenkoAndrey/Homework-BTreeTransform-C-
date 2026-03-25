CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2

SOURCES = main.cpp btree.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = btree_transform
TEST_EXECUTABLE = btree_test

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_EXECUTABLE): btree_test.cpp btree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lgtest -lgtest_main -pthread

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(EXECUTABLE)
	./$(EXECUTABLE)

test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(TEST_EXECUTABLE) *.o

rebuild: clean all

.PHONY: all run test clean rebuild
