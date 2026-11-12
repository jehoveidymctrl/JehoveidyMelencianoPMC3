# Tasks: add-gestion-costos-vehiculo

## 1. Núcleo
- [x] 1.1 Definir estructuras `Vehiculo`, `Gasto`, `Registro` y códigos de error (`costos.h`)
- [x] 1.2 Implementar `registro_iniciar` / `registro_liberar` con arreglo dinámico
- [x] 1.3 Implementar `vehiculo_definir` con sus validaciones
- [x] 1.4 Implementar `fecha_valida` (incluye años bisiestos)

## 2. Gastos
- [x] 2.1 Implementar `gasto_agregar` con validaciones e inserción ordenada por fecha
- [x] 2.2 Asignar identificadores únicos que no se reutilizan
- [x] 2.3 Implementar `gasto_eliminar`
- [x] 2.4 Limpiar la descripción (carácter `|`, saltos de línea, UTF-8)

## 3. Reportes
- [x] 3.1 Totales general y por categoría
- [x] 3.2 Costo por kilómetro
- [x] 3.3 Consumo promedio en km/l

## 4. Persistencia
- [x] 4.1 Guardar en archivo de texto
- [x] 4.2 Cargar con validación y sin modificar el registro si el archivo es inválido

## 5. Interfaz de consola
- [x] 5.1 Menú: vehículo, registrar gasto, listar, eliminar, reportes, guardar
- [x] 5.2 Cargar al iniciar y guardar al salir

## 6. Pruebas
- [x] 6.1 Una prueba automática por cada escenario de las especificaciones (`make test`)
- [x] 6.2 Ejecutar las pruebas con AddressSanitizer para detectar errores de memoria
