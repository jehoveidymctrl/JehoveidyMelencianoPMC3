#include <stdio.h>
int main(void) {
    unsigned long long anterior = 0, actual = 1;
    for (int i = 0; i < 50; i++) {
        printf("%llu%s", anterior, i == 49 ? "\n" : " ");
        unsigned long long siguiente = anterior + actual;
        anterior = actual;
        actual = siguiente;
    }
    return 0;
}
