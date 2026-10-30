CC     = gcc
CFLAGS = -Wall -Wextra -std=c99

all: bin/cap7 bin/cap8

bin:
	mkdir -p bin

bin/cap7: 07-capitulo7-cadenas/cap7_ejercicios.c | bin
	$(CC) $(CFLAGS) $< -o $@
bin/cap8: 08-capitulo8-estructuras/cap8_ejercicios.c | bin
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf bin
