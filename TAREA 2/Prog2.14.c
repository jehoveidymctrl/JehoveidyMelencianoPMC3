#include <stdio.h>
int main(void) {
    int clave, segundos;
    double tarifa;
    printf("Clave de zona y duracion en segundos: ");
    if (scanf("%d %d", &clave, &segundos) != 2 || segundos < 0) return 1;
    switch (clave) {
        case 1: tarifa = 0.13; break;
        case 2: tarifa = 0.11; break;
        case 5: tarifa = 0.22; break;
        case 6: tarifa = 0.19; break;
        case 7:
        case 9: tarifa = 0.17; break;
        case 10: tarifa = 0.20; break;
        case 15: tarifa = 0.39; break;
        case 20: tarifa = 0.28; break;
        default: puts("Clave no valida"); return 1;
    }
    printf("Zona %d, %d segundos, costo %.2f\n", clave, segundos,
           tarifa * segundos / 60.0);
    return 0;
}
