CXX=g++
INCLUDES=
CFLAGS=-g -Wall $(INCLUDES)
LDFLAGS=-L/usr/local/lib/
LDLIBS=-lglfw -lglut -lGL -lGLEW -lGLU -lsoil2

main: main.o ../Utils.o
	$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@

main.o: ../Utils.o main.cpp vertShader.glsl fragShader.glsl
	$(CXX) -c $(CFLAGS) $^

../Utils.o: ../Utils.cpp
	$(CXX) -c $(CFLAGS) $^ -o $@

../Utils.cpp: ../Utils.hpp

.PHONY:
clean:
	rm -rf main *.o ../Utils.o

.PHONY:
run:
	make main; ./main






