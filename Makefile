CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
LDFLAGS = 

# Исходные файлы и объектные файлы
SOURCES = main.cpp btree.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = btree_transform
TEST_EXECUTABLE = test_edge_cases
TEST_GOOGLE = btree_gtest

# Цели
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_EXECUTABLE): test_edge_cases.cpp btree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_GOOGLE): btree_gtest.cpp btree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -lgtest -lgtest_main -pthread

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(EXECUTABLE)
	./$(EXECUTABLE)

test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

gtest: $(TEST_GOOGLE)
	./$(TEST_GOOGLE)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(TEST_EXECUTABLE) $(TEST_GOOGLE) *.o

rebuild: clean all

.PHONY: all run test gtest clean rebuild
