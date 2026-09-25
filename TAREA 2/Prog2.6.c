#include <stdio.h>
int main(void) {
    int nivel;
    double salario, porcentaje;
    printf("Nivel (1-4) y salario: ");
    if (scanf("%d %lf", &nivel, &salario) != 2 || salario < 0) return 1;
    switch (nivel) {
        case 1: porcentaje = 3.5; break;
        case 2: porcentaje = 4.1; break;
        case 3: porcentaje = 4.8; break;
        case 4: porcentaje = 5.3; break;
        default: puts("Nivel no valido"); return 1;
    }
    printf("Nivel %d, salario nuevo: %.2f\n", nivel,
           salario * (1.0 + porcentaje / 100.0));
    return 0;
}
