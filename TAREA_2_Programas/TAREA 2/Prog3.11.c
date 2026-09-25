#include <stdio.h>
int main(void) {
    int rangos[5] = {0};
    double nota;
    printf("Calificacion (-1 termina): ");
    while (scanf("%lf", &nota) == 1 && nota != -1) {
        if (nota < 0 || nota > 10) { puts("Nota fuera de 0 a 10"); return 1; }
        if (nota < 4) rangos[0]++;
        else if (nota < 6) rangos[1]++;
        else if (nota < 8) rangos[2]++;
        else if (nota < 9) rangos[3]++;
        else rangos[4]++;
        printf("Siguiente calificacion (-1 termina): ");
    }
    const char *nombres[] = {"0 a 3.99", "4 a 5.99", "6 a 7.99", "8 a 8.99", "9 a 10"};
    for (int i = 0; i < 5; i++) printf("%s: %d\n", nombres[i], rangos[i]);
    return 0;
}
