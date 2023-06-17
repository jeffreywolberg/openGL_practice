CXX=g++
INCLUDES=
CFLAGS=-g -Wall $(INCLUDES)
LDFLAGS=
LDLIBS=-lglfw -lglut -lGL -lGLEW -lGLU

main: main.o ../Utils.o
	$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@

main.o: fragShader.glsl vertShader.glsl ../Utils.o main.cpp
	$(CXX) -c $(CFLAGS) $^

../Utils.o: ../Utils.cpp
	$(CXX) -c $(CFLAGS) $^ -o $@

../Utils.cpp: ../Utils.hpp






