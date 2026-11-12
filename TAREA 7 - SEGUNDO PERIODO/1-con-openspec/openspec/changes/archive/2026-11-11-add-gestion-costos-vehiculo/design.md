# Design: add-gestion-costos-vehiculo

## Decisiones

1. **Núcleo separado de la interfaz.** `costos.c` no usa `printf` ni `scanf`; solo recibe datos
   y devuelve códigos de error (`CostosError`). Así la misma lógica sirve para la consola y para
   una interfaz gráfica, y se puede probar sin pantalla.
2. **Arreglo dinámico ordenado.** Los gastos viven en un arreglo que crece con `realloc` (se
   duplica la capacidad). Cada inserción se coloca en su posición por fecha, de modo que el
   arreglo siempre está ordenado y listar no requiere ordenar.
3. **Identificadores que no se reutilizan.** `siguiente_id` solo aumenta; se guarda en el archivo
   de forma implícita (máximo id + 1) para que no se repitan al cargar.
4. **Validación en un solo lugar.** `validar_gasto` se usa tanto al registrar como al cargar un
   archivo, así un archivo editado a mano no puede introducir datos inválidos.
5. **Archivo de texto delimitado por `|`.** Es legible y fácil de revisar. El carácter `|` y los
   saltos de línea se eliminan de las descripciones al registrarlas.
6. **Carga segura.** `registro_cargar` lee a un registro temporal y solo reemplaza el actual si
   todo el archivo es válido.
7. **UTF-8.** Los textos se guardan en UTF-8 y el recorte de descripciones respeta los límites de
   los caracteres.

## Fórmulas
- Costo por km = total general / (km máximo registrado − odómetro inicial).
- Consumo = (km última carga − km primera carga) / litros de todas las cargas menos la primera
  (supone tanque lleno en cada carga).

## Riesgos
- El consumo es una aproximación si no se llena el tanque cada vez.
