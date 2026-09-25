#include <stdio.h>
int main(void) {
    int numero;
    long long suma = 0;
    printf("Numero natural (0 termina): ");
    if (scanf("%d", &numero) != 1) return 1;
    while (numero != 0) {
        if (numero < 0) { puts("Solo numeros naturales"); return 1; }
        long long cuadrado = (long long)numero * numero;
        printf("Cuadrado de %d: %lld\n", numero, cuadrado);
        suma += cuadrado;
        printf("Otro numero (0 termina): ");
        if (scanf("%d", &numero) != 1) return 1;
    }
    printf("Suma de cuadrados: %lld\n", suma);
    return 0;
}
