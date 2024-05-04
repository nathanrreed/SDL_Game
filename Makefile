.PHONY: all
all: generate run game

debug: CFLAGS = -DDEBUG -g
debug: all

run: tools/generate_enums
	./tools/generate_enums

generate: tools/generate_enums.c
	gcc tools/generate_enums.c -o tools/generate_enums

game.o: *.c menus/*.c
	gcc $(CFLAGS) -c *.c menus/*.c
	mv *.o build/

game: game.o
	gcc $(CFLAGS) build/*.o -lm `pkg-config --cflags --libs sdl3 sdl3-image sdl3-ttf` -o main

.PHONY: clean
clean:
	rm -f build/*.o main