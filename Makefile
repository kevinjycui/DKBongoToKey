CXX = g++
CPPFLAGS = -lSDL2 -lSDL2main -lSDL2_mixer

main: src/*.cpp inc/*.h
	$(CXX) src/*.cpp $(CPPFLAGS) -o dkbongo_manager
