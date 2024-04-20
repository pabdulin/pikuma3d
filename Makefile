build: 
	gcc -Wall -std=c99 ./src/*.c -o renderer -I ./third_party/include/SDL2 -L ./third_party/lib -lmingw32 -lSDL2main -lSDL2

run:
	./renderer

clean:
	rm renderer
