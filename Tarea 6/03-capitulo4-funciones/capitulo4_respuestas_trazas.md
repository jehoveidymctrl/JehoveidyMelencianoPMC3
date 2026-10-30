# Capítulo 4 – Funciones: respuestas a los problemas de traza

Los problemas PS4.1 a PS4.9 están resueltos en `capitulo4_ejercicios.c`.

## PS4.10 – Resultados del Programa 4.22
```
15 20 4 30     <- dentro de pal (primera llamada: x=3, y=5)
4 15 3 5       <- main: a=4, b=15, c=3, d=5
16 20 5 20     <- dentro de pal (segunda llamada: x=4, y=4)
4 5 3 5        <- main: a=4, b=5, c=3, d=5
```
La `c` de `pal` es local y no modifica la `c` global (sigue en 3). `b` es global, por eso cambia dentro de la función.

## PS4.11 – Llamadas correctas
1. `trueque(suma(&x), &x);` **Incorrecta**: `suma` recibe un `int`, no una dirección, y su resultado `int` no es un apuntador.
2. `trueque(3, 4);` **Incorrecta**: `trueque` necesita direcciones, no constantes.
3. `suma(10);` **Correcta** (el resultado se descarta).
4. `y = suma(10);` **Correcta**.
5. `z = trueque(&x, &y);` **Incorrecta**: `trueque` es `void`, no devuelve valor.
6. `trueque(&x, &y);` **Correcta**.

## PS4.12 – Resultados del Programa 4.24
El libro trae dos detalles que no compilan: falta el `;` en `K *= K` y `::K` es sintaxis de C++ (la K global). Con esas dos correcciones:

| Iteración | f1 | f2 | f3 | f4 |
|-----------|----|----|----|----|
| 1 | 25 | 4 | 9 | 29 |
| 2 | 625 | 4 | 12 | 629 |
| 3 | 390625 | 4 | 15 | 390629 |
| 4 | desborda `int` | 4 | 18 | desborda `int` |

En la 4.ª iteración 390625² no cabe en un `int` (en la prueba dio -2030932031 y -2030932027; el valor exacto depende del compilador).
- f1 usa la K global y la modifica.
- f2 usa una K local que se reinicia a 3.
- f3 usa una K `static` que conserva su valor entre llamadas.
- f4 suma su K local (4) con la K global.
