## ADDED Requirements

### Requirement: Guardar y cargar
El sistema SHALL guardar el vehículo y los gastos en un archivo de texto y SHALL poder cargarlos de nuevo conservando todos los datos y el siguiente número de gasto.

#### Scenario: Ida y vuelta
- **WHEN** se guarda el registro y se carga en uno vacío
- **THEN** se recuperan el vehículo, los 5 gastos y el mismo total general

#### Scenario: Número siguiente
- **WHEN** se carga un archivo con gastos hasta el número 5
- **THEN** el siguiente gasto recibe el número 6

#### Scenario: Archivo inexistente
- **WHEN** se intenta cargar un archivo que no existe
- **THEN** el sistema indica que no se pudo abrir el archivo

#### Scenario: Archivo dañado
- **WHEN** el archivo tiene líneas con formato inválido
- **THEN** el sistema indica formato inválido y el registro actual no se modifica

### Requirement: Formato legible
El archivo SHALL ser de texto con una cabecera COSTOS_VEHICULO 1, una línea VEHICULO y una línea GASTO por cada gasto, con campos separados por el carácter |.

#### Scenario: Contenido del archivo
- **WHEN** se guarda un registro con un vehículo y un gasto
- **THEN** el archivo contiene la cabecera, una línea VEHICULO y una línea GASTO
