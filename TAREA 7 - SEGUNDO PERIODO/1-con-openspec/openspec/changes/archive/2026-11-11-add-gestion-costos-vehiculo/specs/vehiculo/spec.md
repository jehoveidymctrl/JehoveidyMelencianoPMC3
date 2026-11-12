## ADDED Requirements

### Requirement: Registrar datos del vehículo
El sistema SHALL permitir registrar marca, modelo, placa, año y odómetro inicial del vehículo. Marca, modelo y placa SHALL ser obligatorios, el año SHALL estar entre 1900 y el año actual más uno, y el odómetro inicial SHALL ser mayor o igual a cero.

#### Scenario: Datos válidos
- **WHEN** se ingresan Toyota, Corolla, placa ABC123, año 2020 y odómetro inicial 10000
- **THEN** el vehículo queda registrado y el sistema indica que hay un vehículo definido

#### Scenario: Año fuera de rango
- **WHEN** se ingresa el año 1850
- **THEN** el sistema rechaza los datos con un error de año

#### Scenario: Placa vacía
- **WHEN** la placa contiene solo espacios
- **THEN** el sistema rechaza los datos indicando que marca, modelo y placa son obligatorios

#### Scenario: Odómetro negativo
- **WHEN** el odómetro inicial es -5
- **THEN** el sistema rechaza los datos con un error de odómetro

### Requirement: Coherencia con los gastos existentes
El sistema SHALL rechazar un odómetro inicial mayor que el kilometraje de cualquier gasto ya registrado.

#### Scenario: Odómetro contradictorio
- **WHEN** existe un gasto con kilometraje 10100 y se intenta definir un odómetro inicial de 12000
- **THEN** el sistema rechaza el cambio con un error de odómetro
