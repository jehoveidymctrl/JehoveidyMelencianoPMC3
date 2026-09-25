#include <stdio.h>
int main(void) {
    long long suma = 0;
    int termino = 2;
    int sumar_cinco = 1;
    while (termino <= 2500) {
        printf("%d ", termino);
        suma += termino;
        termino += sumar_cinco ? 5 : 3;
        sumar_cinco = !sumar_cinco;
    }
    printf("\nSuma de la serie: %lld\n", suma);
    return 0;
}
