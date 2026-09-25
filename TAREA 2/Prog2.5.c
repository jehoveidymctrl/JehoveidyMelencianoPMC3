#include <stdio.h>
#include <math.h>
int main(void) {
    int opcion, valor;
    double resultado;
    printf("Opcion y numero entero: ");
    if (scanf("%d %d", &opcion, &valor) != 2) return 1;
    switch (opcion) {
        case 1: resultado = valor / 5.0; break;
        case 2: resultado = pow(valor, valor); break;
        case 3:
        case 4: resultado = 3.0 * valor; break;
        default: resultado = 1.0; break;
    }
    printf("Resultado: %.2f\n", resultado);
    return 0;
}
