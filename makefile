all:
	g++ src/*.cpp src/physics/*.cpp `sdl-config --cflags --libs` -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_ttf -Isrc/includes -Isrc/physics -o BomberMain

r:	run
run:
	./BomberMain
fresh:
	find -name '*~' -exec rm '{}' \;
	
