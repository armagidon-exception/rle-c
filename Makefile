CC=gcc
CFLAGS=-fsanitize=address -g -Wall -Isrc/
SHELL=/bin/bash

obj/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
.PHONY: test

clean:
	rm -rfv obj/*.o

test:
	./test.sh
