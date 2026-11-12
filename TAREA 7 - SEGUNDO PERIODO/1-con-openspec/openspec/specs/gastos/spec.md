# gastos Specification

## Purpose
Registro, consulta y eliminación de gastos del vehículo.

## Requirements

### Requirement: Registrar un gasto
El sistema SHALL registrar gastos con fecha (AAAA-MM-DD), categoría (combustible, mantenimiento, reparación, seguro, impuestos u otros), monto, kilometraje, litros y descripción. La fecha SHALL ser una fecha real del calendario, el monto SHALL ser mayor que cero, el kilometraje SHALL ser mayor o igual al odómetro inicial, los litros SHALL ser mayores o iguales a cero y mayores que cero cuando la categoría sea combustible.

#### Scenario: Gasto válido
- **WHEN** se registra un combustible del 2026-01-05 por 40.00, km 10100 y 10 litros
- **THEN** el gasto queda guardado

#### Scenario: Sin vehículo
- **WHEN** se intenta registrar un gasto sin haber definido el vehículo
- **THEN** el sistema indica que primero debe registrarse el vehículo

#### Scenario: Monto cero
- **WHEN** el monto es 0
- **THEN** el sistema rechaza el gasto con un error de monto

#### Scenario: Fecha inexistente
- **WHEN** la fecha es 2026-02-30
- **THEN** el sistema rechaza el gasto con un error de fecha

#### Scenario: Formato de fecha incorrecto
- **WHEN** la fecha es 05/01/2026
- **THEN** el sistema rechaza el gasto con un error de fecha

#### Scenario: Año bisiesto
- **WHEN** la fecha es 2024-02-29
- **THEN** el gasto se acepta

#### Scenario: Combustible sin litros
- **WHEN** la categoría es combustible y los litros son 0
- **THEN** el sistema rechaza el gasto con un error de litros

#### Scenario: Kilometraje menor al inicial
- **WHEN** el kilometraje es 9000 y el odómetro inicial es 10000
- **THEN** el sistema rechaza el gasto con un error de kilometraje

### Requirement: Identificador único
El sistema SHALL asignar a cada gasto un número entero creciente que identifique el gasto y que no se reutilice aunque el gasto se elimine.

#### Scenario: Números consecutivos
- **WHEN** se registran dos gastos
- **THEN** reciben los números 1 y 2

#### Scenario: Sin reutilización
- **WHEN** se elimina el gasto 2 y se registra otro
- **THEN** el nuevo gasto no recibe el número 2

### Requirement: Eliminar un gasto
El sistema SHALL permitir eliminar un gasto por su número.

#### Scenario: Gasto existente
- **WHEN** se elimina el gasto número 2 que existe
- **THEN** deja de aparecer en la lista

#### Scenario: Gasto inexistente
- **WHEN** se elimina un número que no existe
- **THEN** el sistema indica que no existe un gasto con ese número

### Requirement: Lista ordenada por fecha
El sistema SHALL mantener los gastos ordenados por fecha ascendente.

#### Scenario: Inserción fuera de orden
- **WHEN** se registra un gasto con fecha anterior a los ya existentes
- **THEN** aparece al inicio de la lista

### Requirement: Limpieza de la descripción
El sistema SHALL reemplazar el carácter | y los saltos de línea de la descripción, y SHALL limitarla a 79 bytes sin cortar caracteres UTF-8 por la mitad.

#### Scenario: Separador en la descripción
- **WHEN** la descripción es "a|b"
- **THEN** la descripción guardada no contiene el carácter |
