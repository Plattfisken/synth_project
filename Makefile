build:
	clang -Wall -std=c99 ./src/sdl_platform.c -o synth_project -lSDL2 -g -O0

run:
	clang -Wall -std=c99 ./src/sdl_platform.c -o synth_project -g -lSDL2 -O0
	./synth_project

clean:
	rm synth_project
