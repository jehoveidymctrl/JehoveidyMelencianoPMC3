#include <stdio.h>
int main(void) {
    double pago, total = 0;
    printf("Pago (0 termina): ");
    if (scanf("%lf", &pago) != 1) return 1;
    while (pago != 0) {
        total += pago;
        printf("Siguiente pago (0 termina): ");
        if (scanf("%lf", &pago) != 1) return 1;
    }
    printf("Total pagado: %.2f\n", total);
    return 0;
}
