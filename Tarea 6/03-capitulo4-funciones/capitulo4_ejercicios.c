/* Capitulo 4 - Funciones. Problemas suplementarios PS4.1 a PS4.9
   (PS4.10 a PS4.12 son de traza y se responden en el documento).
   Sin variables globales, todo con funciones. Compilar: gcc capitulo4_ejercicios.c -o cap4 -lm */
#include <stdio.h>
#include <math.h>

/* ---------- PS4.1: promedio de pares e impares ---------- */
void promedios(int n)
{
    int i, num, sp = 0, cp = 0, si = 0, ci = 0;
    for (i = 1; i <= n; i++) {
        printf("  Numero %d: ", i);
        scanf("%d", &num);
        if (num % 2 == 0) { sp += num; cp++; }
        else              { si += num; ci++; }
    }
    if (cp > 0) printf("  Promedio de pares:   %.2f\n", (float)sp / cp);
    else        printf("  No hay numeros pares.\n");
    if (ci > 0) printf("  Promedio de impares: %.2f\n", (float)si / ci);
    else        printf("  No hay numeros impares.\n");
}
void ps41(void)
{
    int n;
    do { printf("N (1 a 500): "); scanf("%d", &n); } while (n < 1 || n > 500);
    promedios(n);
}

/* ---------- PS4.2: serie 1 * 1/2 / 1/3 * 1/4 / ... (alterna * y /) ---------- */
double serie(int n)
{
    int i;
    double r = 1.0;                     /* primer termino: 1/1 */
    for (i = 2; i <= n; i++) {
        if (i % 2 == 0) r = r * (1.0 / i);
        else            r = r / (1.0 / i);
    }
    return r;
}
void ps42(void)
{
    int n;
    printf("Numero de terminos: "); scanf("%d", &n);
    printf("Resultado de la serie: %.6f\n", serie(n));
}

/* ---------- PS4.3: factorial ---------- */
double factorial(int n)
{
    double f = 1.0;
    int i;
    for (i = 2; i <= n; i++) f *= i;
    return f;
}
void ps43(void)
{
    int n;
    printf("N: "); scanf("%d", &n);
    if (n < 0) printf("No existe factorial de un negativo.\n");
    else       printf("%d! = %.0f\n", n, factorial(n));
}

/* ---------- PS4.4: inversion a interes compuesto mensual ---------- */
double inversion(double cap, double tasa, int meses)
{
    int i;
    for (i = 1; i <= meses; i++) cap += cap * tasa / 100.0;
    return cap;
}
void ps44(void)
{
    int mes; double cap, tas;
    printf("Meses: ");               scanf("%d", &mes);
    printf("Capital inicial: ");     scanf("%lf", &cap);
    printf("Tasa mensual (%%): ");   scanf("%lf", &tas);
    printf("Monto final: %.2f\n", inversion(cap, tas, mes));
}

/* ---------- PS4.5: 1^1 - 2^2 + 3^3 - ... +/- N^N ---------- */
double potencia(int base, int exp)
{
    double p = 1.0; int i;
    for (i = 1; i <= exp; i++) p *= base;
    return p;
}
double serie_potencias(int n)
{
    double s = 0.0; int i;
    for (i = 1; i <= n; i++) {
        if (i % 2 == 1) s += potencia(i, i);
        else            s -= potencia(i, i);
    }
    return s;
}
void ps45(void)
{
    int n;
    printf("N: "); scanf("%d", &n);
    printf("Resultado: %.0f\n", serie_potencias(n));
}

/* ---------- PS4.6: cos(x) por serie, error <= 0.001 ---------- */
double coseno(double x, int *terminos)
{
    double suma = 1.0, termino = 1.0, anterior;
    int k = 0;
    *terminos = 1;
    do {
        anterior = suma;
        k += 2;
        termino = -termino * x * x / ((k - 1) * k);   /* x^k/k! con signo alterno */
        suma += termino;
        (*terminos)++;
    } while (fabs(suma - anterior) > 0.001);
    return suma;
}
void ps46(void)
{
    double x; int t;
    printf("X (radianes): "); scanf("%lf", &x);
    printf("cos(%.4f) = %.6f  (libreria: %.6f)\n", x, coseno(x, &t), cos(x));
    printf("Terminos requeridos: %d\n", t);
}

/* ---------- PS4.7: numero primo ---------- */
int es_primo(int n)
{
    int d;
    if (n < 2) return 0;
    for (d = 2; d * d <= n; d++)
        if (n % d == 0) return 0;
    return 1;
}
void ps47(void)
{
    int n;
    printf("Numero entero positivo: "); scanf("%d", &n);
    printf("%d %s primo.\n", n, es_primo(n) ? "es" : "no es");
}

/* ---------- PS4.8: numeros perfectos entre 1 y N ---------- */
int es_perfecto(int n)
{
    int d, s = 0;
    for (d = 1; d <= n / 2; d++)
        if (n % d == 0) s += d;
    return s == n;
}
void ps48(void)
{
    int n, i;
    printf("N: "); scanf("%d", &n);
    printf("Perfectos entre 1 y %d:", n);
    for (i = 1; i <= n; i++)
        if (es_perfecto(i)) printf(" %d", i);
    printf("\n");
}

/* ---------- PS4.9: imprimir numero de 4 digitos al reves ---------- */
void invertir(int n)
{
    int i;
    for (i = 0; i < 4; i++) {
        printf("%d   ", n % 10);
        n /= 10;
    }
    printf("\n");
}
void ps49(void)
{
    int n;
    printf("Numero de cuatro digitos: "); scanf("%d", &n);
    invertir(n);
}

/* ---------- Menu ---------- */
int main(void)
{
    int op;
    do {
        printf("\n=== Capitulo 4: Funciones ===\n");
        printf("1) PS4.1 promedios pares/impares   2) PS4.2 serie * y /\n");
        printf("3) PS4.3 factorial                 4) PS4.4 inversion\n");
        printf("5) PS4.5 serie N^N                 6) PS4.6 coseno\n");
        printf("7) PS4.7 primo                     8) PS4.8 perfectos\n");
        printf("9) PS4.9 invertir 4 digitos        0) Salir\nOpcion: ");
        if (scanf("%d", &op) != 1) break;
        switch (op) {
            case 1: ps41(); break; case 2: ps42(); break;
            case 3: ps43(); break; case 4: ps44(); break;
            case 5: ps45(); break; case 6: ps46(); break;
            case 7: ps47(); break; case 8: ps48(); break;
            case 9: ps49(); break;
        }
    } while (op != 0);
    return 0;
}
