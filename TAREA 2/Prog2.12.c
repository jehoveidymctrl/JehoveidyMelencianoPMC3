#include <stdio.h>
#include <math.h>
int main(void) {
    int t, p, n;
    printf("Valores enteros T, P y N: ");
    if (scanf("%d %d %d", &t, &p, &n) != 3) return 1;
    if (p == 0) { puts("P debe ser diferente de cero"); return 1; }
    double izquierda = pow((double)t / p, n);
    double derecha = pow(t, n) / pow(p, n);
    if (isfinite(izquierda) && isfinite(derecha) &&
        fabs(izquierda - derecha) <= 1e-9 * fmax(1.0, fabs(derecha)))
        puts("Se comprueba la igualdad");
    else puts("No se comprueba la igualdad");
    return 0;
}
