#include <stdio.h>
int main(void) {
    int numero;
    printf("Numero entero: ");
    if (scanf("%d", &numero) != 1) return 1;
    if (numero == 0) puts("Nulo");
    else if (numero % 2 == 0) puts("Par");
    else puts("Impar");
    return 0;
}
