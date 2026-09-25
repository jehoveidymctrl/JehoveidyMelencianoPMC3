#include <stdio.h>
int main(void) {
    int y;
    double x;
    printf("Valor entero de Y: ");
    if (scanf("%d", &y) != 1) return 1;
    if (y < 0 || y > 50) x = 0.0;
    else if (y == 0) { puts("La primera expresion no existe para Y=0"); return 1; }
    else if (y <= 10) x = 4.0 / y - y;
    else if (y <= 25) x = (double)y*y*y - 12.0;
    else x = (double)y*y + (double)y*y*y - 18.0;
    printf("Y=%d, X=%.2f\n", y, x);
    return 0;
}
