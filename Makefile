build:
	clang -Wall -std=c99 ./src/*.c -o game -lSDL2 -g -O0

run:
	clang -Wall -std=c99 ./src/*.c -o game -lSDL2 -g -O0
	./game

clean:
	rm game
