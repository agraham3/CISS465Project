all:
	g++ src/*.cpp src/physics/*.cpp src/txt/*.cpp src/objects/*.cpp src/scClass/*.cpp src/SDL/*.cpp `sdl2-config --cflags --libs` -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_ttf -Isrc/ -Isrc/physics -Isrc/txt -Isrc/objects -Isrc/scClass -Isrc/SDL -o BomberMan

r:	run
run:
	./BomberMan 69.29.94.105 1238
local:
	./BomberMan localhost 1238
fresh:
	find -name '*~' -exec rm '{}' \;
pull:
	git pull
add:
	git add . -A
commit:
	git commit
push: rmexe pull fresh add commit
	git push
bilbo:
	scp -r ./server/ ./src/ amwoods3@bilbo.ccis.edu:~/ciss465/
rmexe:
	rm BomberMan
	rm server/Server
