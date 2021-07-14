CXX = g++
CPPFLAGS = -lSDL2 -lSDL2main

main: src/*.cpp inc/*.h
	$(CXX) src/*.cpp $(CPPFLAGS) -o main -g
