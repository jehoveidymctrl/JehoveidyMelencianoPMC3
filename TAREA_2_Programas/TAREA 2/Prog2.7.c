#include <stdio.h>
int main(void) {
    double ventas[3], temporal;
    printf("Ventas de los tres vendedores: ");
    if (scanf("%lf %lf %lf", &ventas[0], &ventas[1], &ventas[2]) != 3) return 1;
    for (int i = 0; i < 2; i++)
        for (int j = i + 1; j < 3; j++)
            if (ventas[j] > ventas[i]) {
                temporal = ventas[i]; ventas[i] = ventas[j]; ventas[j] = temporal;
            }
    printf("Ventas de mayor a menor: %.2f, %.2f, %.2f\n",
           ventas[0], ventas[1], ventas[2]);
    return 0;
}
