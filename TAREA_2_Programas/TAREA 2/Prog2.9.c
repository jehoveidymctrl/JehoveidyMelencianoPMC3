#include <stdio.h>
int main(void) {
    long long r, t, q;
    printf("Valores enteros R, T y Q: ");
    if (scanf("%lld %lld %lld", &r, &t, &q) != 3) return 1;
    double expresion = (double)r*r*r*r - (double)t*t*t + 4.0*(double)q*q;
    if (expresion < 820.0)
        printf("Cumplen la condicion: R=%lld, T=%lld, Q=%lld\n", r, t, q);
    else puts("No cumplen la condicion");
    return 0;
}
