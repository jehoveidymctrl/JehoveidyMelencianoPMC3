#include <stdio.h>
int main(void) {
    double precio;
    printf("Precio del producto importado: ");
    if (scanf("%lf", &precio) != 1 || precio < 0) return 1;
    if (precio < 1500.0) {
        precio += precio * 0.11;
        printf("Nuevo precio: %.2f\n", precio);
    }
    return 0;
}
