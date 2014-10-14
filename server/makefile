all:
	g++ *.cpp ../src/functions.cpp ../src/physics/*.cpp ../src/txt/*.cpp ../src/scClass/*.cpp `sdl-config --cflags --libs` -lSDL -lSDL_net -lSDL_image -lSDL_ttf -I../src/ -I../src/physics -I../src/txt -I../src/scClass -o Server

r:
	make run
run:
	./Server 1238
