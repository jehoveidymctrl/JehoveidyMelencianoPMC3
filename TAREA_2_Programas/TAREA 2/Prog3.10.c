#include <stdio.h>
int main(void) {
    int cantidad, numero, impares = 0;
    long long suma_pares = 0, suma_impares = 0;
    printf("Cantidad de numeros: ");
    if (scanf("%d", &cantidad) != 1 || cantidad < 1) return 1;
    for (int i = 1; i <= cantidad; i++) {
        printf("Numero %d: ", i);
        if (scanf("%d", &numero) != 1) return 1;
        if (numero % 2 == 0) suma_pares += numero;
        else { suma_impares += numero; impares++; }
    }
    printf("Suma de pares: %lld\n", suma_pares);
    if (impares) printf("Promedio de impares: %.2f\n", (double)suma_impares / impares);
    else puts("No hubo numeros impares");
    return 0;
}
