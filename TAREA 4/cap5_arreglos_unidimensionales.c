/* ======================================================================
   Fundamentos de programación. Piensa en C  (Osvaldo Cairó)
   Capítulo 5: Arreglos unidimensionales
   Solución de los Problemas suplementarios PS5.1 a PS5.12

   Compilar:  gcc -std=c99 -Wall -Wextra -o cap5 cap5_arreglos_unidimensionales.c
   Ejecutar:  ./cap5

   Nota del libro: todos los problemas se resuelven con funciones.
   Cada problema tiene su función de demostración (psX_Y) con datos de
   ejemplo; las funciones auxiliares reciben los datos como parámetros,
   así que se pueden reutilizar con datos leídos desde el teclado.
   ====================================================================== */

#include <stdio.h>
#include <string.h>

#define MAX 100

/* ---------------------------------------------------------------------
   Utilidades
   --------------------------------------------------------------------- */
void imprimir(const char *titulo, const int v[], int n)
{
    printf("%s: [", titulo);
    for (int i = 0; i < n; i++)
        printf("%s%d", i ? ", " : "", v[i]);
    printf("]\n");
}

void encabezado(const char *texto)
{
    printf("\n==== %s ====\n", texto);
}

/* =====================================================================
   PS5.1  Contar positivos, negativos y nulos
   ===================================================================== */
void contar_signos(const int v[], int n, int *pos, int *neg, int *nul)
{
    *pos = *neg = *nul = 0;
    for (int i = 0; i < n; i++) {
        if (v[i] > 0)      (*pos)++;
        else if (v[i] < 0) (*neg)++;
        else               (*nul)++;
    }
}

void ps5_1(void)
{
    encabezado("PS5.1 Positivos, negativos y nulos");
    int vec[] = {4, -2, 0, 7, -9, 0, 0, 15, -1, 3};
    int n = sizeof vec / sizeof vec[0];
    int pos, neg, nul;

    imprimir("VEC", vec, n);
    contar_signos(vec, n, &pos, &neg, &nul);
    printf("Positivos: %d   Negativos: %d   Nulos: %d\n", pos, neg, nul);
}

/* =====================================================================
   PS5.2  Eliminar repetidos de un arreglo ordenado
   Devuelve el nuevo número de elementos.
   ===================================================================== */
int quitar_repetidos(int v[], int n)
{
    if (n == 0)
        return 0;
    int k = 1;                       /* siguiente posición libre */
    for (int i = 1; i < n; i++)
        if (v[i] != v[k - 1])
            v[k++] = v[i];
    return k;
}

void ps5_2(void)
{
    encabezado("PS5.2 Quitar elementos repetidos (arreglo ordenado)");
    int vec[] = {1, 1, 2, 3, 3, 3, 5, 8, 8, 9};
    int n = sizeof vec / sizeof vec[0];

    imprimir("Antes  ", vec, n);
    n = quitar_repetidos(vec, n);
    imprimir("Después", vec, n);
}

/* =====================================================================
   PS5.3  Los primeros 100 números de Fibonacci
   F(99) = 218 922 995 834 555 169 026 no cabe en ningún entero estándar
   de C (unsigned long long llega a ~1.8e19, es decir, hasta F(93)).
   Por eso cada número se guarda como un arreglo de dígitos decimales
   (aritmética de precisión arbitraria) y los resultados son exactos.
   ===================================================================== */
#define NF  100                      /* cantidad de números de Fibonacci */
#define DIG 24                       /* dígitos por número (sobran)       */

void sumar_grandes(const int a[], const int b[], int r[])
{
    int acarreo = 0;
    for (int i = 0; i < DIG; i++) {  /* dígito menos significativo primero */
        int s = a[i] + b[i] + acarreo;
        r[i] = s % 10;
        acarreo = s / 10;
    }
}

void generar_fibonacci(int fib[NF][DIG])
{
    memset(fib, 0, sizeof(int) * NF * DIG);
    fib[0][0] = 0;                   /* F0 = 0 */
    fib[1][0] = 1;                   /* F1 = 1 */
    for (int i = 2; i < NF; i++)
        sumar_grandes(fib[i - 1], fib[i - 2], fib[i]);
}

void imprimir_grande(const int d[])
{
    int i = DIG - 1;
    while (i > 0 && d[i] == 0)       /* saltar ceros a la izquierda */
        i--;
    for (; i >= 0; i--)
        printf("%d", d[i]);
}

void ps5_3(void)
{
    encabezado("PS5.3 Primeros 100 números de Fibonacci");
    static int fib[NF][DIG];
    generar_fibonacci(fib);
    for (int i = 0; i < NF; i++) {
        printf("F(%2d) = ", i);
        imprimir_grande(fib[i]);
        printf("\n");
    }
}

/* =====================================================================
   PS5.4  Insertar y eliminar en un arreglo DESORDENADO sin repetidos
   Devuelven: insertar ->  1 insertado, 0 ya existía, -1 arreglo lleno
              eliminar ->  1 eliminado, 0 no existía
   ===================================================================== */
int buscar(const int v[], int n, int ele)
{
    for (int i = 0; i < n; i++)
        if (v[i] == ele)
            return i;
    return -1;
}

int insertar_desordenado(int v[], int *n, int ele)
{
    if (buscar(v, *n, ele) != -1)
        return 0;
    if (*n >= MAX)
        return -1;
    v[(*n)++] = ele;                 /* al final: no importa el orden */
    return 1;
}

int eliminar_desordenado(int v[], int *n, int ele)
{
    int pos = buscar(v, *n, ele);
    if (pos == -1)
        return 0;
    v[pos] = v[*n - 1];              /* el último ocupa el hueco */
    (*n)--;
    return 1;
}

void ps5_4(void)
{
    encabezado("PS5.4 Insertar / eliminar (arreglo desordenado)");
    int vec[MAX] = {7, 3, 9, 1, 5};
    int n = 5;

    imprimir("Inicial     ", vec, n);
    printf("Insertar 4 -> %d\n", insertar_desordenado(vec, &n, 4));
    imprimir("             ", vec, n);
    printf("Insertar 9 (repetido) -> %d\n", insertar_desordenado(vec, &n, 9));
    printf("Eliminar 3 -> %d\n", eliminar_desordenado(vec, &n, 3));
    imprimir("             ", vec, n);
    printf("Eliminar 20 (no existe) -> %d\n", eliminar_desordenado(vec, &n, 20));
}

/* =====================================================================
   PS5.5  Insertar y eliminar en un arreglo ORDENADO (creciente)
   ===================================================================== */
int insertar_ordenado(int v[], int *n, int ele)
{
    if (*n >= MAX)
        return -1;
    int i = 0;
    while (i < *n && v[i] < ele)     /* posición que le corresponde */
        i++;
    if (i < *n && v[i] == ele)
        return 0;                    /* repetido */
    for (int j = *n; j > i; j--)     /* abrir hueco */
        v[j] = v[j - 1];
    v[i] = ele;
    (*n)++;
    return 1;
}

int eliminar_ordenado(int v[], int *n, int ele)
{
    int i = 0;
    while (i < *n && v[i] < ele)
        i++;
    if (i == *n || v[i] != ele)
        return 0;                    /* no existe */
    for (int j = i; j < *n - 1; j++) /* cerrar hueco conservando el orden */
        v[j] = v[j + 1];
    (*n)--;
    return 1;
}

void ps5_5(void)
{
    encabezado("PS5.5 Insertar / eliminar (arreglo ordenado)");
    int vec[MAX] = {2, 4, 6, 8, 10};
    int n = 5;

    imprimir("Inicial     ", vec, n);
    printf("Insertar 7 -> %d\n", insertar_ordenado(vec, &n, 7));
    imprimir("             ", vec, n);
    printf("Insertar 1 -> %d\n", insertar_ordenado(vec, &n, 1));
    imprimir("             ", vec, n);
    printf("Insertar 6 (repetido) -> %d\n", insertar_ordenado(vec, &n, 6));
    printf("Eliminar 4 -> %d\n", eliminar_ordenado(vec, &n, 4));
    imprimir("             ", vec, n);
    printf("Eliminar 5 (no existe) -> %d\n", eliminar_ordenado(vec, &n, 5));
}

/* =====================================================================
   PS5.6  Calificaciones del examen de admisión
   ===================================================================== */
float promedio(const float v[], int n)
{
    float s = 0;
    for (int i = 0; i < n; i++)
        s += v[i];
    return s / n;
}

int contar_mayores(const float v[], int n, float limite)   /* > limite  */
{
    int c = 0;
    for (int i = 0; i < n; i++)
        if (v[i] > limite)
            c++;
    return c;
}

int contar_mayor_igual(const float v[], int n, float limite) /* >= limite */
{
    int c = 0;
    for (int i = 0; i < n; i++)
        if (v[i] >= limite)
            c++;
    return c;
}

void ps5_6(void)
{
    encabezado("PS5.6 Examen de admisión");
    float alu[] = {1250, 1320, 1500, 1610, 980, 1400, 1300, 1550, 1180, 1305};
    int n = sizeof alu / sizeof alu[0];

    printf("a) Promedio general: %.2f\n", promedio(alu, n));
    printf("b) Aprobados (> 1300): %.2f%%\n",
           100.0f * contar_mayores(alu, n, 1300) / n);
    printf("c) Alumnos con 1500 o más: %d\n", contar_mayor_igual(alu, n, 1500));
}

/* =====================================================================
   PS5.7  Cosecha mensual de cereales
   ===================================================================== */
static const char *MESES[12] = {
    "enero", "febrero", "marzo", "abril", "mayo", "junio", "julio",
    "agosto", "septiembre", "octubre", "noviembre", "diciembre"
};

int indice_mayor(const float v[], int n)
{
    int k = 0;
    for (int i = 1; i < n; i++)
        if (v[i] > v[k])
            k = i;
    return k;
}

void ps5_7(void)
{
    encabezado("PS5.7 Cosecha de cereales");
    float cos[12] = {120.5f, 98.0f, 150.2f, 200.0f, 310.7f, 280.3f,
                     90.1f, 60.0f, 130.4f, 220.8f, 340.9f, 275.6f};
    float prom = promedio(cos, 12);
    int mayor = indice_mayor(cos, 12);

    printf("a) Promedio anual: %.2f toneladas\n", prom);
    printf("b) Meses por encima del promedio: %d\n",
           contar_mayores(cos, 12, prom));
    printf("c) Mayor cosecha: %s con %.2f toneladas\n",
           MESES[mayor], cos[mayor]);
}

/* =====================================================================
   PS5.8  Mezclar dos arreglos DESORDENADOS -> uno DESCENDENTE
   ===================================================================== */
void ordenar_descendente(int v[], int n)           /* inserción */
{
    for (int i = 1; i < n; i++) {
        int x = v[i], j = i - 1;
        while (j >= 0 && v[j] < x) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = x;
    }
}

int mezclar_desordenados(const int a[], int n, const int b[], int m, int r[])
{
    for (int i = 0; i < n; i++) r[i] = a[i];
    for (int j = 0; j < m; j++) r[n + j] = b[j];
    ordenar_descendente(r, n + m);
    return n + m;
}

void ps5_8(void)
{
    encabezado("PS5.8 Mezcla de desordenados -> descendente");
    int a[] = {5, 1, 9, 3};
    int b[] = {8, 2, 7, 4, 6};
    int r[2 * MAX];
    int n = sizeof a / sizeof a[0], m = sizeof b / sizeof b[0];

    imprimir("VEC1", a, n);
    imprimir("VEC2", b, m);
    int t = mezclar_desordenados(a, n, b, m, r);
    imprimir("Resultado", r, t);
}

/* =====================================================================
   PS5.9  Mezclar dos arreglos ASCENDENTES -> uno ASCENDENTE
   ===================================================================== */
int mezclar_ascendentes(const int a[], int n, const int b[], int m, int r[])
{
    int i = 0, j = 0, k = 0;
    while (i < n && j < m)
        r[k++] = (a[i] <= b[j]) ? a[i++] : b[j++];
    while (i < n) r[k++] = a[i++];
    while (j < m) r[k++] = b[j++];
    return k;
}

void ps5_9(void)
{
    encabezado("PS5.9 Mezcla de ascendentes -> ascendente");
    int a[] = {1, 4, 6, 10};
    int b[] = {2, 3, 7, 8, 12};
    int r[2 * MAX];
    int n = sizeof a / sizeof a[0], m = sizeof b / sizeof b[0];

    imprimir("VEC1", a, n);
    imprimir("VEC2", b, m);
    int t = mezclar_ascendentes(a, n, b, m, r);
    imprimir("Resultado", r, t);
}

/* =====================================================================
   PS5.10  VEC1 ascendente + VEC2 descendente -> uno ASCENDENTE
   VEC1 se recorre de izquierda a derecha y VEC2 de derecha a izquierda
   (así ambos se leen de menor a mayor).
   ===================================================================== */
int mezclar_mixtos(const int a[], int n, const int b[], int m, int r[])
{
    int i = 0, j = m - 1, k = 0;
    while (i < n && j >= 0)
        r[k++] = (a[i] <= b[j]) ? a[i++] : b[j--];
    while (i < n)  r[k++] = a[i++];
    while (j >= 0) r[k++] = b[j--];
    return k;
}

void ps5_10(void)
{
    encabezado("PS5.10 Mezcla ascendente + descendente -> ascendente");
    int a[] = {1, 4, 6, 10};
    int b[] = {12, 8, 7, 3, 2};
    int r[2 * MAX];
    int n = sizeof a / sizeof a[0], m = sizeof b / sizeof b[0];

    imprimir("VEC1 (asc)", a, n);
    imprimir("VEC2 (desc)", b, m);
    int t = mezclar_mixtos(a, n, b, m, r);
    imprimir("Resultado", r, t);
}

/* =====================================================================
   PS5.11  Números perfectos
   Un número es perfecto si es igual a la suma de sus divisores propios.

   LIMITACIÓN IMPORTANTE: sólo se conocen 51 números perfectos y crecen
   muy rápido. El 8.º ya vale 2 305 843 008 139 952 128 (el mayor que cabe
   en unsigned long long); el 9.º tiene 37 dígitos y el 30.º tiene más de
   79 000 dígitos. Es imposible obtener 30 con los tipos de C del libro,
   y buscarlos "por fuerza bruta" tardaría siglos desde el 6.º en adelante.

   Solución: se usa la fórmula de Euclides-Euler:
        2^(p-1) * (2^p - 1)   es perfecto  <=>  2^p - 1 es primo
   y se comprueba que 2^p - 1 sea primo con la prueba de Lucas-Lehmer.
   Se llena el arreglo con todos los perfectos que caben (8).
   ===================================================================== */
#define NPERF 30

int es_primo_pequeno(int p)
{
    if (p < 2) return 0;
    for (int d = 2; d * d <= p; d++)
        if (p % d == 0) return 0;
    return 1;
}

/* ¿2^p - 1 es primo?  (p <= 31 para que s*s quepa en 64 bits) */
int mersenne_es_primo(int p)
{
    if (p == 2) return 1;                      /* 3 es primo */
    if (!es_primo_pequeno(p)) return 0;
    unsigned long long M = (1ULL << p) - 1, s = 4;
    for (int i = 0; i < p - 2; i++)
        s = (s * s + M - 2) % M;
    return s == 0;
}

/* Verifica por definición (divisores hasta la raíz) */
int es_perfecto_por_definicion(unsigned long long x)
{
    unsigned long long suma = 1;
    for (unsigned long long d = 2; d * d <= x; d++)
        if (x % d == 0) {
            suma += d;
            if (d != x / d) suma += x / d;
        }
    return suma == x;
}

int llenar_perfectos(unsigned long long perf[], int max)
{
    int k = 0;
    for (int p = 2; p <= 31 && k < max; p++)
        if (mersenne_es_primo(p))
            perf[k++] = (1ULL << (p - 1)) * ((1ULL << p) - 1);
    return k;
}

void ps5_11(void)
{
    encabezado("PS5.11 Números perfectos");
    unsigned long long perf[NPERF];
    int k = llenar_perfectos(perf, NPERF);

    printf("Con los tipos de C sólo caben %d de los 30 pedidos:\n", k);
    for (int i = 0; i < k; i++) {
        printf("%2d) %llu", i + 1, perf[i]);
        if (perf[i] < 1000000000000ULL)
            printf("   (verificado por definición: %s)",
                   es_perfecto_por_definicion(perf[i]) ? "sí" : "NO");
        printf("\n");
    }
}

/* =====================================================================
   PS5.12  ¿El arreglo es palíndrome?
   ===================================================================== */
int es_palindrome(const int v[], int n)
{
    for (int i = 0, j = n - 1; i < j; i++, j--)
        if (v[i] != v[j])
            return 0;
    return 1;
}

void ps5_12(void)
{
    encabezado("PS5.12 Arreglo palíndrome");
    int a[] = {2, 4, 5, 4, 2};
    int b[] = {1, 2, 3, 4};
    int c[] = {7, 7};

    imprimir("A", a, 5);
    printf("  -> %s\n", es_palindrome(a, 5) ? "es palíndrome" : "no es palíndrome");
    imprimir("B", b, 4);
    printf("  -> %s\n", es_palindrome(b, 4) ? "es palíndrome" : "no es palíndrome");
    imprimir("C", c, 2);
    printf("  -> %s\n", es_palindrome(c, 2) ? "es palíndrome" : "no es palíndrome");
}

/* ===================================================================== */
int main(void)
{
    ps5_1();
    ps5_2();
    ps5_3();
    ps5_4();
    ps5_5();
    ps5_6();
    ps5_7();
    ps5_8();
    ps5_9();
    ps5_10();
    ps5_11();
    ps5_12();
    return 0;
}
