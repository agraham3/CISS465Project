all:
	g++ src/*.cpp src/physics/*.cpp `sdl-config --cflags --libs` -lSDL -lSDL_net -lSDL_image -lSDL_ttf -Isrc/includes -Isrc/physics -o BomberMain

r:	run
run:
	./BomberMain
