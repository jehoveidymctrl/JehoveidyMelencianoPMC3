CC     = gcc
CFLAGS = -Wall -Wextra -std=c99

all: costos_vehiculo

costos_vehiculo: src/main.c src/costos.c src/costos.h
	$(CC) $(CFLAGS) src/main.c src/costos.c -o $@

test: tests/test_costos.c src/costos.c src/costos.h
	$(CC) $(CFLAGS) tests/test_costos.c src/costos.c -o test_costos -lm
	./test_costos

clean:
	rm -f costos_vehiculo test_costos *.exe vehiculo.dat
