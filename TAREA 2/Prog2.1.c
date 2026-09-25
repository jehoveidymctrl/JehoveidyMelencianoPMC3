#include <stdio.h>
int main(void) {
    float promedio;
    printf("Promedio del alumno: ");
    if (scanf("%f", &promedio) != 1) return 1;
    if (promedio >= 6.0f) puts("Aprobado");
    return 0;
}
