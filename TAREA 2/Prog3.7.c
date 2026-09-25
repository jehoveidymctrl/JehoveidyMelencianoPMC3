#include <stdio.h>
int main(void) {
    int lanzamientos;
    double distancia, suma = 0;
    do {
        printf("Cantidad de lanzamientos (1 a 10): ");
        if (scanf("%d", &lanzamientos) != 1) return 1;
    } while (lanzamientos < 1 || lanzamientos > 10);
    for (int i = 1; i <= lanzamientos; i++) {
        printf("Distancia del lanzamiento %d: ", i);
        if (scanf("%lf", &distancia) != 1 || distancia < 0) return 1;
        suma += distancia;
    }
    printf("Promedio: %.2f\n", suma / lanzamientos);
    return 0;
}
