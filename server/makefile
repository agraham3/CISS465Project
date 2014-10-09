all:
	g++ *.cpp ../src/functions.cpp ../src/physics/*.cpp ../src/txt/*.cpp ../src/objects/*.cpp ../src/scClass/*.cpp ../src/SDL/*.cpp `sdl2-config --cflags --libs` -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_ttf -I../src/ -I../src/physics -I../src/txt -I../src/objects -I../src/scClass -I../src/SDL -o Server

r:
	make run
run:
	./Server 8080
