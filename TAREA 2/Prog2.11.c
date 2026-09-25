#include <stdio.h>
int main(void) {
    int distancia, dias;
    double costo;
    printf("Distancia de ida (km) y dias de estancia: ");
    if (scanf("%d %d", &distancia, &dias) != 2 || distancia < 0 || dias < 0) return 1;
    costo = 2.0 * distancia * 0.19;
    if (2 * distancia > 500 && dias > 10) costo *= 0.80;
    printf("Costo del billete: %.2f\n", costo);
    return 0;
}
