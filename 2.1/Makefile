CXX=g++
INCLUDES=-I/opt/homebrew/Cellar/glfw/3.3.8/include/ -I/opt/homebrew/Cellar/glew/2.2.0_1/include/ -I/opt/homebrew/Cellar/glm/0.9.9.8/include/ 
# -I/Library/Developer/CommandLineTools/SDKs/MacOSX13.1.sdk/usr/include/c++/v1/
CFLAGS=-g -Wall $(INCLUDES)
LDFLAGS=-L/opt/homebrew/Cellar/glfw/3.3.8/lib/ -L/opt/homebrew/Cellar/glew/2.2.0_1/lib/ -L/opt/homebrew/Cellar/glm/0.9.9.8/lib/ -L/System/Library/Frameworks/
LDLIBS=-lglfw -lglew $(FRAMEWORKS)
FRAMEWORKS= -framework OpenGL

main: main.o
	$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@

main.o: main.cpp
	$(CXX) -c $(CFLAGS) $^

../Utils.o: ../Utils.cpp
	$(CXX) -c $(CFLAGS) $^ -o $@

../Utils.cpp: ../Utils.hpp






