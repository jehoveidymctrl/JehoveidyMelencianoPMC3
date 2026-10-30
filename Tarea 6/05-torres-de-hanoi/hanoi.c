/* Torres de Hanoi con recursividad.
   Compilar: gcc hanoi.c -o hanoi
   Idea: para mover n discos de A a C usando B como auxiliar:
     1) mover n-1 discos de A a B (usando C)
     2) mover el disco mas grande de A a C
     3) mover n-1 discos de B a C (usando A)
   Cantidad minima de movimientos: 2^n - 1 */
#include <stdio.h>

static long movimientos = 0;

void hanoi(int n, char origen, char destino, char auxiliar)
{
    if (n == 0) return;                                  /* caso base */
    hanoi(n - 1, origen, auxiliar, destino);
    movimientos++;
    printf("Mov %3ld: mover disco %d de %c a %c\n", movimientos, n, origen, destino);
    hanoi(n - 1, auxiliar, destino, origen);
}

int main(void)
{
    int n;
    do {
        printf("Numero de discos (1 a 10): ");
        if (scanf("%d", &n) != 1) return 1;
    } while (n < 1 || n > 10);

    printf("\nSolucion para %d disco(s):\n\n", n);
    hanoi(n, 'A', 'C', 'B');
    printf("\nTotal de movimientos: %ld (formula 2^n - 1 = %ld)\n", movimientos, (1L << n) - 1);
    printf("Con 64 discos y 1 movimiento por segundo se necesitarian mas de 584 mil millones de anios.\n");
    return 0;
}
