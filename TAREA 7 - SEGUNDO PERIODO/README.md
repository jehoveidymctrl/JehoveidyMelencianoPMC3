# Tarea 7 – Segundo periodo: Gestión de costos de un vehículo

Programa en C para registrar los gastos de un vehículo (combustible, mantenimiento, reparación,
seguro, impuestos, otros) y calcular totales, **costo por kilómetro** y **consumo promedio (km/l)**.

| Carpeta | Punto de la tarea | Contenido |
|---------|-------------------|-----------|
| `1-con-openspec` | 1. Con OpenSpec | Carpeta `openspec/` (propuesta, especificaciones, diseño, tareas) + código en `src/` + pruebas en `tests/` |
| `2-sin-openspec` | 2. Sin OpenSpec | El mismo programa hecho directo, en un solo archivo |
| `3-interfaz-grafica` | 3. Interfaz gráfica | Ventana de Windows (API Win32) sobre el mismo núcleo de la carpeta 1 |

## Qué hace el programa

- Registra marca, modelo, placa, año y odómetro inicial del vehículo.
- Registra, lista (ordenada por fecha) y elimina gastos. Valida fechas reales (incluye años bisiestos),
  montos positivos, kilometraje coherente y litros en combustible.
- Reportes: total por categoría, total general, costo por km y consumo promedio.
- Guarda y carga los datos en un archivo de texto (`vehiculo.dat`, o `vehiculo_simple.txt` en la versión 2).

Ejemplo del que salen los números de las pruebas: Toyota Corolla, odómetro inicial 10000; tres cargas
de combustible (40, 50 y 45 → 10100, 10400 y 10700 km con 10, 12.5 y 11 litros), mantenimiento 120 y
seguro 300. Total 555.00 · costo por km 0.7929 · consumo 25.53 km/l.

## Compilar y ejecutar

Se necesita `gcc`. En Windows sirve MinGW-w64 (o MSYS2); en Linux/Mac, `gcc` normal.

**Versión de consola con OpenSpec (1):**
```bash
cd 1-con-openspec
make            # o: gcc src/main.c src/costos.c -o costos_vehiculo
./costos_vehiculo
make test       # ejecuta las 36 pruebas automáticas
```

**Versión sin OpenSpec (2):**
```bash
cd 2-sin-openspec
gcc costos_vehiculo.c -o costos_vehiculo
./costos_vehiculo
```

**Interfaz gráfica (3), solo Windows:**
```bat
cd 3-interfaz-grafica
build.bat       # genera costos_gui.exe (equivale a: make)
costos_gui.exe
```
La ventana usa solo la API nativa de Windows: no hay que instalar librerías. Comparte el archivo
`vehiculo.dat` con la versión de consola, así que se pueden abrir los mismos datos en las dos.
Con Visual Studio: `cl /utf-8 /I..\1-con-openspec\src gui_win32.c ..\1-con-openspec\src\costos.c user32.lib gdi32.lib /link /SUBSYSTEM:WINDOWS`.

## Cómo se organizó con OpenSpec (carpeta 1)

OpenSpec trabaja en este orden: **propuesta → especificaciones → diseño → tareas → implementar → archivar**.

```
openspec/
  config.yaml                      contexto y reglas del proyecto
  specs/                           descripción actual del sistema (4 capacidades)
    vehiculo/  gastos/  reportes/  persistencia/      cada una con su spec.md
  changes/archive/2026-11-11-add-gestion-costos-vehiculo/
    proposal.md                    por qué y qué cambia
    design.md                      decisiones técnicas
    tasks.md                       lista de tareas (todas marcadas)
    specs/                         las especificaciones del cambio (## ADDED Requirements)
```

Cada requisito usa **SHALL** y tiene escenarios **WHEN/THEN**; cada escenario tiene su prueba en
`tests/test_costos.c` (los nombres de las pruebas siguen los de los escenarios).

> Esta carpeta `openspec/` la escribí a mano con el mismo formato que genera la herramienta, porque
> no pude instalar ni ejecutar el comando `openspec`. Si tu profesor exige que se genere con el
> programa, instala `@fission-ai/openspec`, ejecuta `openspec init` en esta carpeta y usa los
> archivos como base: los comandos `openspec validate` y `openspec list` te dirán si el formato es aceptado.

## Comparación entre las dos versiones

| | Con OpenSpec (1) | Sin OpenSpec (2) |
|---|---|---|
| Estructura | Núcleo `costos.c` sin pantalla + `main.c` | Un solo archivo con variables globales |
| Gastos | Arreglo dinámico (`realloc`) | Arreglo fijo de 500 |
| Validación | Códigos de error y mensajes en un solo lugar | Mensajes dentro de cada función |
| Pruebas | 36 pruebas automáticas, una por escenario | Solo pruebas manuales |
| Documentación | 425 líneas de especificación, diseño y tareas | Ninguna |
| Líneas de código | 493 (núcleo + consola) + 114 de pruebas | 170 |
| Reutilizable por la interfaz gráfica | Sí | No |

Con OpenSpec hay que escribir primero qué debe hacer el programa, y eso obligó a definir casos
límite antes de programar (30 de febrero, combustible sin litros, archivo dañado…). La versión directa
es más corta y rápida de escribir, pero esos casos quedan sin documentar ni probar.

## Verificación

- El núcleo y la consola se compilaron con `-Wall -Wextra` sin avisos y las 36 pruebas pasan, también
  con AddressSanitizer (sin errores de memoria).
- La interfaz gráfica **no se pudo ejecutar** donde se escribió (no había Windows ni MinGW); solo se
  comprobó su sintaxis. Si al compilar o abrir la ventana algo falla, revisar primero `build.bat`.
