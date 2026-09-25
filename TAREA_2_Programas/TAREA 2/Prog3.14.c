#include <stdio.h>
int main(void) {
    int votos[6] = {0}, voto, total = 0;
    printf("Voto (1-5; 0 termina): ");
    while (scanf("%d", &voto) == 1 && voto != 0) {
        if (voto >= 1 && voto <= 5) votos[voto]++;
        else votos[0]++;
        total++;
        printf("Siguiente voto (0 termina): ");
    }
    if (total == 0) { puts("No hubo votos"); return 0; }
    printf("Total: %d\n", total);
    for (int i = 1; i <= 5; i++)
        printf("Candidato %d: %d votos (%.2f%%)\n", i, votos[i], 100.0*votos[i]/total);
    printf("Nulos: %d votos (%.2f%%)\n", votos[0], 100.0*votos[0]/total);
    return 0;
}
