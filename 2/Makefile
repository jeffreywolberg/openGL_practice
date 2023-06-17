CXX=g++
INCLUDES=
CFLAGS=-g -Wall $(INCLUDES)
LDFLAGS=
LDLIBS=-lglfw -lglut -lGL -lGLEW -lGLU

main: main.o
	$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@

main.o: main.cpp
	$(CXX) -c $(CFLAGS) $^

../Utils.o: ../Utils.cpp
	$(CXX) -c $(CFLAGS) $^ -o $@

../Utils.cpp: ../Utils.hpp






