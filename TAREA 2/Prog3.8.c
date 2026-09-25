#include <stdio.h>
int main(void) {
    int numero;
    unsigned long long factorial = 1;
    printf("Numero entero (0 a 20): ");
    if (scanf("%d", &numero) != 1 || numero < 0 || numero > 20) {
        puts("Debe estar entre 0 y 20"); return 1;
    }
    for (int i = 2; i <= numero; i++) factorial *= (unsigned)i;
    printf("%d! = %llu\n", numero, factorial);
    return 0;
}
