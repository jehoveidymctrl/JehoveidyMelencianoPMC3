## ADDED Requirements

### Requirement: Totales
El sistema SHALL calcular el total general de gastos y el total de cada categoría.

#### Scenario: Totales del ejemplo
- **WHEN** existen gastos de combustible por 40, 50 y 45, mantenimiento por 120 y seguro por 300
- **THEN** el total general es 555.00, el de combustible 135.00 y el de seguro 300.00

#### Scenario: Categoría sin gastos
- **WHEN** no hay gastos de impuestos
- **THEN** el total de impuestos es 0.00

### Requirement: Costo por kilómetro
El sistema SHALL calcular el costo por kilómetro como el total general dividido entre la diferencia del mayor kilometraje registrado y el odómetro inicial.

#### Scenario: Con recorrido
- **WHEN** el total es 555.00, el mayor kilometraje es 10700 y el odómetro inicial 10000
- **THEN** el costo por kilómetro es 555 / 700

#### Scenario: Sin datos
- **WHEN** no hay gastos registrados
- **THEN** el sistema indica que no hay datos suficientes

### Requirement: Consumo promedio de combustible
El sistema SHALL calcular el consumo en km/l como los kilómetros entre la primera y la última carga de combustible, divididos entre los litros de todas las cargas excepto la primera.

#### Scenario: Tres cargas
- **WHEN** hay cargas en km 10100 (10 l), 10400 (12.5 l) y 10700 (11 l)
- **THEN** el consumo es 600 / 23.5 km/l

#### Scenario: Una sola carga
- **WHEN** solo existe una carga de combustible
- **THEN** el sistema indica que no hay datos suficientes
