# TAREA 2 - Programas en C

Ejemplos 2.1 a 2.15 y 3.1 a 3.15 de los capitulos 2 y 3 de *Fundamentos de programacion: Piensa en C*, reescritos con variables claras, `int main(void)`, mensajes propios y comprobacion basica de entradas. Cada archivo se compila y ejecuta por separado.

| Archivo | Que hace |
|---|---|
| [`Prog2.1.c`](Prog2.1.c) | Promedio: aprobado |
| [`Prog2.2.c`](Prog2.2.c) | Aumento del 11 % |
| [`Prog2.3.c`](Prog2.3.c) | Promedio: aprobado o reprobado |
| [`Prog2.4.c`](Prog2.4.c) | Aumento de precio segun umbral |
| [`Prog2.5.c`](Prog2.5.c) | Funcion por opciones |
| [`Prog2.6.c`](Prog2.6.c) | Salario por nivel |
| [`Prog2.7.c`](Prog2.7.c) | Ordenar tres ventas |
| [`Prog2.8.c`](Prog2.8.c) | Asistente por carrera |
| [`Prog2.9.c`](Prog2.9.c) | Comprobar expresion |
| [`Prog2.10.c`](Prog2.10.c) | Par, impar o nulo |
| [`Prog2.11.c`](Prog2.11.c) | Billete de tren |
| [`Prog2.12.c`](Prog2.12.c) | Igualdad de potencias |
| [`Prog2.13.c`](Prog2.13.c) | Funcion por tramos |
| [`Prog2.14.c`](Prog2.14.c) | Llamada por zona |
| [`Prog2.15.c`](Prog2.15.c) | Costo de spa |
| [`Prog3.1.c`](Prog3.1.c) | Nomina de 15 docentes |
| [`Prog3.2.c`](Prog3.2.c) | Suma de positivos |
| [`Prog3.3.c`](Prog3.3.c) | Suma de pagos con while |
| [`Prog3.4.c`](Prog3.4.c) | Cuadrados y suma |
| [`Prog3.5.c`](Prog3.5.c) | Suma de pagos con do while |
| [`Prog3.6.c`](Prog3.6.c) | Nomina y promedio |
| [`Prog3.7.c`](Prog3.7.c) | Promedio de lanzamientos |
| [`Prog3.8.c`](Prog3.8.c) | Factorial |
| [`Prog3.9.c`](Prog3.9.c) | Serie 2, 7, 10, 15... |
| [`Prog3.10.c`](Prog3.10.c) | Pares e impares |
| [`Prog3.11.c`](Prog3.11.c) | Rangos de notas |
| [`Prog3.12.c`](Prog3.12.c) | Sucesion de Ulam |
| [`Prog3.13.c`](Prog3.13.c) | 50 terminos de Fibonacci |
| [`Prog3.14.c`](Prog3.14.c) | Recuento electoral |
| [`Prog3.15.c`](Prog3.15.c) | Aproximacion de pi |

## Compilar y ejecutar

Desde la carpeta `TAREA 2`, por ejemplo:

```bash
gcc -std=c11 -Wall -Wextra -pedantic Prog2.1.c -o Prog2.1
./Prog2.1
```

Para `Prog2.5.c` y `Prog2.12.c`, agregar `-lm` al final del comando de compilacion. En Windows, ejecutar `Prog2.1.exe` en lugar de `./Prog2.1`.

## Notas sobre el libro

- El programa 2.6 tiene una diferencia entre la tabla de porcentajes (3.5 %, 4.1 %, 4.8 %, 5.3 %) y los multiplicadores del diagrama (1.0035, etc.). Aqui se usan los porcentajes escritos en la tabla.
- En 2.13 se impide dividir entre cero, caso que el codigo impreso no resuelve.
- En 2.15 se aplica el descuento a mayores de 60 y menores de 25, como dice el enunciado; el codigo del libro muestra umbrales distintos.
- En 3.4 se imprime "cuadrado", que coincide con el calculo solicitado; el mensaje del libro dice "cubo" por error.
- En 3.10, 3.14 y 3.6 se evita dividir entre cero cuando faltan datos.

Los programas son versiones propias de los ejercicios, no transcripciones literales.
