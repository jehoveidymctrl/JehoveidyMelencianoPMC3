# Proposal: add-gestion-costos-vehiculo

## Why
Una persona que usa un vehículo necesita saber cuánto le cuesta realmente: combustible,
mantenimiento, reparaciones, seguro e impuestos. Hoy esos datos están dispersos en recibos
y no se puede calcular el costo por kilómetro ni el consumo de combustible.

## What Changes
- Registrar los datos de un vehículo (marca, modelo, placa, año, odómetro inicial).
- Registrar, listar y eliminar gastos clasificados por categoría.
- Calcular totales por categoría, costo por kilómetro y consumo promedio (km/l).
- Guardar y recuperar toda la información desde un archivo de texto.
- Ofrecer una interfaz de consola con menú sobre un núcleo reutilizable.

## Capabilities
- `vehiculo`
- `gastos`
- `reportes`
- `persistencia`

## Out of Scope
- Varios vehículos en el mismo archivo.
- Conversión de monedas o de unidades (millas, galones).
- La interfaz gráfica (se agrega en otra tarea sobre el mismo núcleo).

## Impact
- Código nuevo: `src/costos.h`, `src/costos.c`, `src/main.c`, `tests/test_costos.c`.
