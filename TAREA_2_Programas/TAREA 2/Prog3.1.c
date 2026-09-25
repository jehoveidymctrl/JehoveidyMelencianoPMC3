#include <stdio.h>
int main(void) {
    double salario, nomina = 0;
    for (int profesor = 1; profesor <= 15; profesor++) {
        printf("Salario del profesor %d: ", profesor);
        if (scanf("%lf", &salario) != 1 || salario < 0) return 1;
        nomina += salario;
    }
    printf("Nomina total: %.2f\n", nomina);
    return 0;
}
