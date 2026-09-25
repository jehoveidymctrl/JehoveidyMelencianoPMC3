#include <stdio.h>
int main(void) {
    int cantidad, numero;
    long long suma = 0;
    printf("Cantidad de numeros: ");
    if (scanf("%d", &cantidad) != 1 || cantidad < 0) return 1;
    for (int i = 0; i < cantidad; i++) {
        printf("Numero %d: ", i + 1);
        if (scanf("%d", &numero) != 1) return 1;
        if (numero > 0) suma += numero;
    }
    printf("Suma de los positivos: %lld\n", suma);
    return 0;
}
