# Tarea 6 – Fundamentos de programación en C

Ejercicios y prácticas basados en el libro *Fundamentos de programación. Piensa en C*.

## Contenido

| Carpeta | Descripción |
|---------|-------------|
| `01-bubblesort` | Algoritmo burbuja en C y su diagrama de flujo (`diagrama_bubblesort.png`) |
| `02-calculadora` | Calculadora con suma, resta, multiplicación, división, **raíz cuadrada** (método de Newton, sin `sqrt()`) y **elevación al cuadrado** |
| `03-capitulo4-funciones` | Capítulo 4: problemas PS4.1 a PS4.9 en C y trazas PS4.10 a PS4.12 explicadas |
| `04-capitulo9-archivos` | Capítulo 9: problemas PS9.1 a PS9.10 (archivos de texto, binarios y de acceso directo) y generador de datos de prueba |
| `05-torres-de-hanoi` | Torres de Hanói resuelto con recursividad |
| `06-memoria-dinamica` | Lectura de un archivo de texto con `malloc()` y `realloc()` |

## Compilar

Requiere `gcc` y `make`.

```bash
make            # compila todo en la carpeta bin/
make clean      # borra bin/
```

## Ejecutar

```bash
./bin/bubblesort
./bin/calculadora
./bin/cap4
./bin/hanoi
./bin/leer archivo.txt

# Capítulo 9: primero crea los archivos de prueba
cd 04-capitulo9-archivos
../bin/generar_datos
../bin/cap9
```

## Notas

- Las respuestas del capítulo 4 de tipo "qué imprime el programa" están en
  `03-capitulo4-funciones/capitulo4_respuestas_trazas.md`.
- En el PS9.2 el libro indica 19 para la vocal *e*; el programa cuenta 20 con las letras acentuadas.

## Autor

Estarlin Cuevas
