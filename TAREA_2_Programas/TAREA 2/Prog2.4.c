#include <stdio.h>
int main(void) {
    double precio;
    printf("Precio inicial: ");
    if (scanf("%lf", &precio) != 1 || precio < 0) return 1;
    double aumento = precio < 1500.0 ? 0.11 : 0.08;
    printf("Nuevo precio: %.2f\n", precio * (1.0 + aumento));
    return 0;
}
