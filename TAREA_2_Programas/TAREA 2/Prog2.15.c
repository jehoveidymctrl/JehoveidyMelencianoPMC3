#include <stdio.h>
int main(void) {
    int tratamiento, edad, dias;
    double precio_dia, costo;
    printf("Tratamiento (1-4), edad y dias: ");
    if (scanf("%d %d %d", &tratamiento, &edad, &dias) != 3 || edad < 0 || dias < 0) return 1;
    switch (tratamiento) {
        case 1: precio_dia = 2800; break;
        case 2: precio_dia = 1950; break;
        case 3: precio_dia = 2500; break;
        case 4: precio_dia = 1150; break;
        default: puts("Tratamiento no valido"); return 1;
    }
    costo = precio_dia * dias;
    if (edad > 60) costo *= 0.75;
    else if (edad < 25) costo *= 0.85;
    printf("Tratamiento %d, %d dias, costo %.2f\n", tratamiento, dias, costo);
    return 0;
}
