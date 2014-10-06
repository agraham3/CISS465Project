all:
	g++ src/*.cpp src/physics/*.cpp src/txt/*.cpp src/objects/*.cpp src/scClass/*.cpp src/SDL/*.cpp `sdl2-config --cflags --libs` -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_ttf -Isrc/ -Isrc/physics -Isrc/txt -Isrc/objects -Isrc/scClass -Isrc/SDL -o BomberMan

r:	run
run:
	./BomberMan 8080
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
