#include <stdio.h>
int main(void) {
    double pago, total = 0;
    do {
        printf("Pago (0 termina): ");
        if (scanf("%lf", &pago) != 1) return 1;
        total += pago;
    } while (pago != 0);
    printf("Total de pagos: %.2f\n", total);
    return 0;
}
