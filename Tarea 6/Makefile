CC     = gcc
CFLAGS = -Wall -Wextra -std=c99

PROGRAMAS = bin/bubblesort bin/calculadora bin/cap4 bin/cap9 bin/generar_datos bin/hanoi bin/leer

all: $(PROGRAMAS)

bin:
	mkdir -p bin

bin/bubblesort: 01-bubblesort/bubblesort.c | bin
	$(CC) $(CFLAGS) $< -o $@
bin/calculadora: 02-calculadora/calculadora.c | bin
	$(CC) $(CFLAGS) $< -o $@
bin/cap4: 03-capitulo4-funciones/capitulo4_ejercicios.c | bin
	$(CC) $(CFLAGS) $< -o $@ -lm
bin/cap9: 04-capitulo9-archivos/cap9_ejercicios.c | bin
	$(CC) $(CFLAGS) $< -o $@
bin/generar_datos: 04-capitulo9-archivos/generar_datos.c | bin
	$(CC) $(CFLAGS) $< -o $@
bin/hanoi: 05-torres-de-hanoi/hanoi.c | bin
	$(CC) $(CFLAGS) $< -o $@
bin/leer: 06-memoria-dinamica/leer_archivo_dinamico.c | bin
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf bin
