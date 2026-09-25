#include <stdio.h>
int main(void) {
    unsigned long long numero;
    printf("Entero positivo para la sucesion de Ulam: ");
    if (scanf("%llu", &numero) != 1 || numero == 0) return 1;
    printf("%llu", numero);
    for (int pasos = 0; numero != 1 && pasos < 10000; pasos++) {
        if (numero % 2 == 0) numero /= 2;
        else {
            if (numero > (18446744073709551615ULL - 1) / 3) {
                puts("\nEl valor excede el limite del tipo entero"); return 1;
            }
            numero = 3 * numero + 1;
        }
        printf(" %llu", numero);
    }
    putchar('\n');
    if (numero != 1) puts("Se alcanzo el limite de pasos");
    return 0;
}
