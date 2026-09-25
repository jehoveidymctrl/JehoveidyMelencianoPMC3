#include <stdio.h>
#include <math.h>
int main(void) {
    const double pi_referencia = 3.1418;
    double aproximacion = 0.0;
    int terminos = 0;
    do {
        int denominador = 2 * terminos + 1;
        aproximacion += (terminos % 2 == 0 ? 4.0 : -4.0) / denominador;
        terminos++;
    } while (fabs(aproximacion - pi_referencia) > 0.0005 && terminos < 10000000);
    printf("Aproximacion de pi: %.6f\nTerminos necesarios: %d\n",
           aproximacion, terminos);
    return 0;
}
