#include <stdio.h>
int main(void) {
    double salario, nomina = 0;
    int profesores = 0;
    do {
        printf("Salario del profesor (0 termina): ");
        if (scanf("%lf", &salario) != 1 || salario < 0) return 1;
        if (salario != 0) { nomina += salario; profesores++; }
    } while (salario != 0);
    if (profesores == 0) { puts("No se registraron salarios"); return 0; }
    printf("Nomina: %.2f, promedio: %.2f\n", nomina, nomina / profesores);
    return 0;
}
