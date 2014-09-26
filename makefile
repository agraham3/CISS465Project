all:
	g++ src/*.cpp src/physics/*.cpp src/txt/*.cpp src/objects/*.cpp `sdl-config --cflags --libs` -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_ttf -Isrc/includes -Isrc/physics -Isrc/txt -Isrc/objects-o BomberMan

r:	run
run:
	./BomberMan
fresh:
	find -name '*~' -exec rm '{}' \;
pull:
	git pull
add:
	git add . -A
commit:
	git commit
push: pull fresh add commit
	git push
