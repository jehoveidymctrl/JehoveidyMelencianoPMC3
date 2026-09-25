#include <stdio.h>
int main(void) {
    int matricula, carrera, semestre, minimo;
    double promedio, nota_minima;
    printf("Matricula, carrera (1-4), semestre y promedio: ");
    if (scanf("%d %d %d %lf", &matricula, &carrera, &semestre, &promedio) != 4) return 1;
    switch (carrera) {
        case 1: minimo = 6; nota_minima = 8.5; break;
        case 2: minimo = 5; nota_minima = 9.0; break;
        case 3: minimo = 6; nota_minima = 8.8; break;
        case 4: minimo = 7; nota_minima = 9.0; break;
        default: puts("Carrera no valida"); return 1;
    }
    if (semestre >= minimo && promedio >= nota_minima)
        printf("Puede ser asistente: %d, carrera %d, promedio %.2f\n",
               matricula, carrera, promedio);
    else puts("No cumple los requisitos para ser asistente");
    return 0;
}
