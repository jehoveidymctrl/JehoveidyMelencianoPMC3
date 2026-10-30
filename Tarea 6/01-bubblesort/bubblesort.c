/* Ordenamiento burbuja (Bubble Sort). Diagrama de flujo: diagrama_bubblesort.png
   Compilar: gcc bubblesort.c -o bubblesort */
#include <stdio.h>

void burbuja(int A[], int n)
{
    int i, j, aux;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (A[j] > A[j + 1]) {
                aux = A[j];
                A[j] = A[j + 1];
                A[j + 1] = aux;
            }
}

int main(void)
{
    int A[100], n, i;
    printf("Cantidad de elementos (1 a 100): ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 100) return 1;
    for (i = 0; i < n; i++) { printf("A[%d] = ", i); scanf("%d", &A[i]); }
    burbuja(A, n);
    printf("Ordenado:");
    for (i = 0; i < n; i++) printf(" %d", A[i]);
    printf("\n");
    return 0;
}
