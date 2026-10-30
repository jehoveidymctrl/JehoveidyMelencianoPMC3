/* Calculadora con raiz cuadrada (algoritmo de Newton) y elevacion al cuadrado.
   Compilar: gcc calculadora.c -o calculadora */
#include <stdio.h>

double absoluto(double x) { return x < 0 ? -x : x; }

double suma(double a, double b)   { return a + b; }
double resta(double a, double b)  { return a - b; }
double mult(double a, double b)   { return a * b; }
double divide(double a, double b) { return a / b; }

/* Raiz cuadrada por el metodo babilonico (Newton):
   x_nuevo = (x + n / x) / 2, hasta que la diferencia sea muy pequena. */
double raiz_cuadrada(double n)
{
    double x, anterior;
    if (n == 0) return 0;
    x = n > 1 ? n : 1;                 /* valor inicial */
    do {
        anterior = x;
        x = (x + n / x) / 2.0;
    } while (absoluto(x - anterior) > 0.0000001);
    return x;
}

double cuadrado(double n) { return n * n; }

int main(void)
{
    int op;
    double a, b;
    do {
        printf("\n===== CALCULADORA =====\n");
        printf("1) Sumar\n2) Restar\n3) Multiplicar\n4) Dividir\n");
        printf("5) Raiz cuadrada\n6) Elevar al cuadrado\n0) Salir\nOpcion: ");
        if (scanf("%d", &op) != 1) break;
        if (op >= 1 && op <= 4) {
            printf("Primer numero: ");  scanf("%lf", &a);
            printf("Segundo numero: "); scanf("%lf", &b);
        }
        switch (op) {
            case 1: printf("Resultado: %.4f\n", suma(a, b)); break;
            case 2: printf("Resultado: %.4f\n", resta(a, b)); break;
            case 3: printf("Resultado: %.4f\n", mult(a, b)); break;
            case 4:
                if (b == 0) printf("Error: division entre cero.\n");
                else printf("Resultado: %.4f\n", divide(a, b));
                break;
            case 5:
                printf("Numero: "); scanf("%lf", &a);
                if (a < 0) printf("Error: no hay raiz real de un negativo.\n");
                else printf("Raiz cuadrada: %.6f\n", raiz_cuadrada(a));
                break;
            case 6:
                printf("Numero: "); scanf("%lf", &a);
                printf("Cuadrado: %.4f\n", cuadrado(a));
                break;
            case 0: printf("Hasta luego.\n"); break;
            default: printf("Opcion invalida.\n");
        }
    } while (op != 0);
    return 0;
}
