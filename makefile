all:
	g++ src/*.cpp `sdl-config --cflags --libs` -lSDL -lSDL_net -lSDL_image -lSDL_ttf -Isrc/includes -o BomberMain

r:	run
run:
	./BomberMain
