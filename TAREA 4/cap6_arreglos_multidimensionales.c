/* ======================================================================
   Fundamentos de programación. Piensa en C  (Osvaldo Cairó)
   Capítulo 6: Arreglos multidimensionales
   Solución de los Problemas suplementarios PS6.1 a PS6.12

   Compilar:  gcc -std=c99 -Wall -Wextra -o cap6 cap6_arreglos_multidimensionales.c
   Ejecutar:  ./cap6

   Todos los problemas se resuelven con funciones (nota del libro).
   Cada problema tiene una función de demostración (psX_Y) con datos de
   ejemplo. Las funciones auxiliares reciben las matrices como parámetros
   (arreglos de longitud variable de C99), por lo que sirven con cualquier
   tamaño dentro de los límites del enunciado.
   ====================================================================== */

#include <stdio.h>

void encabezado(const char *texto)
{
    printf("\n==== %s ====\n", texto);
}

void imprimir_matriz(int m, int n, int mat[m][n])
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%4d", mat[i][j]);
        printf("\n");
    }
}

void imprimir_matriz_real(int m, int n, float mat[m][n])
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%8.1f", mat[i][j]);
        printf("\n");
    }
}

/* =====================================================================
   PS6.1  Unos en las diagonales principales, ceros en el resto
   ===================================================================== */
void llenar_diagonales(int n, int mat[n][n])
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mat[i][j] = (i == j || i + j == n - 1) ? 1 : 0;
}

void ps6_1(void)
{
    encabezado("PS6.1 Diagonales principales");
    int mat4[4][4], mat5[5][5];

    llenar_diagonales(4, mat4);
    printf("N = 4:\n");
    imprimir_matriz(4, 4, mat4);
    llenar_diagonales(5, mat5);
    printf("N = 5:\n");
    imprimir_matriz(5, 5, mat5);
}

/* =====================================================================
   PS6.2  Ventas de 5 departamentos durante 12 meses
   ===================================================================== */
static const char *MESES[12] = {
    "enero", "febrero", "marzo", "abril", "mayo", "junio", "julio",
    "agosto", "septiembre", "octubre", "noviembre", "diciembre"
};

/* a) ventas de la fábrica por mes y total anual */
float ventas_mensuales(float ven[5][12], float total_mes[12])
{
    float anual = 0;
    for (int m = 0; m < 12; m++) {
        total_mes[m] = 0;
        for (int d = 0; d < 5; d++)
            total_mes[m] += ven[d][m];
        anual += total_mes[m];
    }
    return anual;
}

/* b) departamento con mayor venta en un mes dado (0 = enero) */
int depto_mayor_venta(float ven[5][12], int mes, float *monto)
{
    int mejor = 0;
    for (int d = 1; d < 5; d++)
        if (ven[d][mes] > ven[mejor][mes])
            mejor = d;
    *monto = ven[mejor][mes];
    return mejor;
}

/* c) mes de mayor y menor venta de un departamento */
void meses_extremos(float ven[5][12], int depto, int *mmax, int *mmin)
{
    *mmax = *mmin = 0;
    for (int m = 1; m < 12; m++) {
        if (ven[depto][m] > ven[depto][*mmax]) *mmax = m;
        if (ven[depto][m] < ven[depto][*mmin]) *mmin = m;
    }
}

void ps6_2(void)
{
    encabezado("PS6.2 Ventas de la fábrica");
    float ven[5][12], total_mes[12], monto;
    for (int d = 0; d < 5; d++)               /* datos de ejemplo */
        for (int m = 0; m < 12; m++)
            ven[d][m] = 10000.0f + ((d * 37 + m * 53 + d * m * 11) % 40) * 500.0f;

    float anual = ventas_mensuales(ven, total_mes);
    printf("a) Ventas mensuales de la fábrica:\n");
    for (int m = 0; m < 12; m++)
        printf("   %-10s %10.2f\n", MESES[m], total_mes[m]);
    printf("   %-10s %10.2f\n", "ANUAL", anual);

    int d = depto_mayor_venta(ven, 6, &monto);   /* julio = índice 6 */
    printf("b) Mayor venta en julio: departamento %d con %.2f\n", d + 1, monto);

    int mmax, mmin;
    meses_extremos(ven, 2, &mmax, &mmin);        /* departamento 3 = índice 2 */
    printf("c) Departamento 3 -> mayor venta en %s (%.2f), menor en %s (%.2f)\n",
           MESES[mmax], ven[2][mmax], MESES[mmin], ven[2][mmin]);
}

/* =====================================================================
   PS6.3  Intercambiar filas: primera <-> última, segunda <-> penúltima...
   ===================================================================== */
void invertir_filas(int m, int n, float mat[m][n])
{
    for (int i = 0; i < m / 2; i++)
        for (int j = 0; j < n; j++) {
            float aux = mat[i][j];
            mat[i][j] = mat[m - 1 - i][j];
            mat[m - 1 - i][j] = aux;
        }
}

void ps6_3(void)
{
    encabezado("PS6.3 Intercambio de filas");
    float mat[4][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};

    printf("Antes:\n");
    imprimir_matriz_real(4, 3, mat);
    invertir_filas(4, 3, mat);
    printf("Después:\n");
    imprimir_matriz_real(4, 3, mat);
}

/* =====================================================================
   PS6.4  ¿Es un cuadrado mágico?  (criterios del problema PR6.7)
   Matriz cuadrada de orden N impar con los números 1..N*N, donde todas
   las filas, columnas y las dos diagonales suman lo mismo.
   Se incluye el generador de PR6.7 para poder probar la función.
   ===================================================================== */
void generar_cuadrado_magico(int n, int cua[n][n])
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cua[i][j] = 0;

    int f = 0, c = n / 2;                       /* 1 en el centro de la 1.ª fila */
    for (int k = 1; k <= n * n; k++) {
        cua[f][c] = k;
        if (k % n == 0)                         /* regla 4: misma columna, fila siguiente */
            f = (f + 1) % n;
        else {                                  /* reglas 2 y 3: fila anterior, columna posterior */
            f = (f - 1 + n) % n;
            c = (c + 1) % n;
        }
    }
}

int es_cuadrado_magico(int n, int cua[n][n])
{
    if (n % 2 == 0)
        return 0;

    /* contiene exactamente los números 1..N*N, sin repetir */
    int visto[n * n + 1];
    for (int k = 0; k <= n * n; k++)
        visto[k] = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            int v = cua[i][j];
            if (v < 1 || v > n * n || visto[v])
                return 0;
            visto[v] = 1;
        }

    /* sumas de filas, columnas y diagonales */
    int objetivo = 0;
    for (int j = 0; j < n; j++)
        objetivo += cua[0][j];

    int d1 = 0, d2 = 0;
    for (int i = 0; i < n; i++) {
        int sf = 0, sc = 0;
        for (int j = 0; j < n; j++) {
            sf += cua[i][j];
            sc += cua[j][i];
        }
        if (sf != objetivo || sc != objetivo)
            return 0;
        d1 += cua[i][i];
        d2 += cua[i][n - 1 - i];
    }
    return d1 == objetivo && d2 == objetivo;
}

void ps6_4(void)
{
    encabezado("PS6.4 Cuadrado mágico");
    int cua[5][5];

    generar_cuadrado_magico(5, cua);
    imprimir_matriz(5, 5, cua);
    printf("-> %s\n", es_cuadrado_magico(5, cua) ? "ES cuadrado mágico"
                                                  : "NO es cuadrado mágico");

    int aux = cua[0][0];                        /* lo arruinamos a propósito */
    cua[0][0] = cua[4][4];
    cua[4][4] = aux;
    printf("\nTras intercambiar dos elementos:\n");
    imprimir_matriz(5, 5, cua);
    printf("-> %s\n", es_cuadrado_magico(5, cua) ? "ES cuadrado mágico"
                                                  : "NO es cuadrado mágico");
}

/* =====================================================================
   PS6.5  Producto de matrices  C[MxP] = A[MxN] * B[NxP]
   ===================================================================== */
void multiplicar(int m, int n, int p, float a[m][n], float b[n][p], float c[m][p])
{
    for (int i = 0; i < m; i++)
        for (int j = 0; j < p; j++) {
            c[i][j] = 0;
            for (int k = 0; k < n; k++)
                c[i][j] += a[i][k] * b[k][j];
        }
}

void ps6_5(void)
{
    encabezado("PS6.5 Producto de matrices");
    float a[2][3] = {{1, 2, 3}, {4, 5, 6}};
    float b[3][2] = {{7, 8}, {9, 10}, {11, 12}};
    float c[2][2];

    multiplicar(2, 3, 2, a, b, c);
    printf("A (2x3):\n");  imprimir_matriz_real(2, 3, a);
    printf("B (3x2):\n");  imprimir_matriz_real(3, 2, b);
    printf("C = A*B (2x2):\n");
    imprimir_matriz_real(2, 2, c);
}

/* =====================================================================
   PS6.6  C = A + B^T   con A[MxN], B[NxM], C[MxN]
   ===================================================================== */
void sumar_traspuesta(int m, int n, int a[m][n], int b[n][m], int c[m][n])
{
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            c[i][j] = a[i][j] + b[j][i];
}

void ps6_6(void)
{
    encabezado("PS6.6 A + traspuesta de B");
    int a[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int b[3][2] = {{10, 20}, {30, 40}, {50, 60}};
    int c[2][3];

    sumar_traspuesta(2, 3, a, b, c);
    printf("A (2x3):\n");   imprimir_matriz(2, 3, a);
    printf("B (3x2):\n");   imprimir_matriz(3, 2, b);
    printf("A + B^T (2x3):\n");
    imprimir_matriz(2, 3, c);
}

/* =====================================================================
   PS6.7  Recorrido en espiral (hacia dentro, en sentido horario)
   ===================================================================== */
void recorrer_espiral(int m, int n, int mat[m][n])
{
    int arriba = 0, abajo = m - 1, izq = 0, der = n - 1;

    while (arriba <= abajo && izq <= der) {
        for (int j = izq; j <= der; j++)        printf("%d ", mat[arriba][j]);
        arriba++;
        for (int i = arriba; i <= abajo; i++)   printf("%d ", mat[i][der]);
        der--;
        if (arriba <= abajo) {
            for (int j = der; j >= izq; j--)    printf("%d ", mat[abajo][j]);
            abajo--;
        }
        if (izq <= der) {
            for (int i = abajo; i >= arriba; i--) printf("%d ", mat[i][izq]);
            izq++;
        }
    }
    printf("\n");
}

void ps6_7(void)
{
    encabezado("PS6.7 Recorrido en espiral");
    int mat[4][5] = {{ 1,  2,  3,  4,  5},
                     {14, 15, 16, 17,  6},
                     {13, 20, 19, 18,  7},
                     {12, 11, 10,  9,  8}};

    imprimir_matriz(4, 5, mat);
    printf("Espiral: ");
    recorrer_espiral(4, 5, mat);
}

/* =====================================================================
   PS6.8  Recorrido columna a columna
   ===================================================================== */
void recorrer_por_columnas(int m, int n, int mat[m][n])
{
    for (int j = 0; j < n; j++)
        for (int i = 0; i < m; i++)
            printf("%d ", mat[i][j]);
    printf("\n");
}

void ps6_8(void)
{
    encabezado("PS6.8 Recorrido por columnas");
    int mat[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};

    imprimir_matriz(3, 4, mat);
    printf("Por columnas: ");
    recorrer_por_columnas(3, 4, mat);
}

/* =====================================================================
   PS6.9  Problema de las ocho reinas (backtracking)
   Se coloca una reina por fila; en cada fila se prueban las 8 columnas
   y se retrocede si no hay lugar seguro. Se muestra la primera solución
   y se cuenta el total de soluciones (son 92).
   ===================================================================== */
#define NR 8

int es_seguro(int tab[NR][NR], int fila, int col)
{
    for (int i = 0; i < fila; i++) {
        if (tab[i][col]) return 0;                          /* misma columna */
        int d = fila - i;
        if (col - d >= 0 && tab[i][col - d]) return 0;      /* diagonal \ */
        if (col + d < NR && tab[i][col + d]) return 0;      /* diagonal / */
    }
    return 1;
}

void resolver_reinas(int tab[NR][NR], int fila, int *soluciones)
{
    if (fila == NR) {
        (*soluciones)++;
        if (*soluciones == 1) {
            printf("Primera solución encontrada:\n");
            for (int i = 0; i < NR; i++) {
                for (int j = 0; j < NR; j++)
                    printf(" %c", tab[i][j] ? 'R' : '.');
                printf("\n");
            }
        }
        return;
    }
    for (int col = 0; col < NR; col++)
        if (es_seguro(tab, fila, col)) {
            tab[fila][col] = 1;
            resolver_reinas(tab, fila + 1, soluciones);
            tab[fila][col] = 0;                             /* retroceso */
        }
}

void ps6_9(void)
{
    encabezado("PS6.9 Ocho reinas");
    int tab[NR][NR] = {{0}};
    int soluciones = 0;

    resolver_reinas(tab, 0, &soluciones);
    printf("Total de soluciones: %d\n", soluciones);
}

/* =====================================================================
   PS6.10  Recorrido del caballo (regla de Warnsdorff)
   En cada paso el caballo va a la casilla libre desde la que tendrá
   MENOS movimientos posibles. Esa heurística resuelve el tablero 8x8
   sin retroceso. Cada casilla guarda el número de movimiento (1..64).
   ===================================================================== */
#define NT 8
static const int DX[8] = { 2,  1, -1, -2, -2, -1,  1,  2};
static const int DY[8] = { 1,  2,  2,  1, -1, -2, -2, -1};

int casilla_libre(int tab[NT][NT], int x, int y)
{
    return x >= 0 && x < NT && y >= 0 && y < NT && tab[x][y] == 0;
}

int grado(int tab[NT][NT], int x, int y)      /* movimientos disponibles desde (x,y) */
{
    int g = 0;
    for (int k = 0; k < 8; k++)
        if (casilla_libre(tab, x + DX[k], y + DY[k]))
            g++;
    return g;
}

int recorrido_caballo(int tab[NT][NT], int x, int y)
{
    for (int i = 0; i < NT; i++)
        for (int j = 0; j < NT; j++)
            tab[i][j] = 0;

    tab[x][y] = 1;
    for (int paso = 2; paso <= NT * NT; paso++) {
        int mejor = -1, mejor_grado = 9;
        for (int k = 0; k < 8; k++) {
            int nx = x + DX[k], ny = y + DY[k];
            if (casilla_libre(tab, nx, ny)) {
                int g = grado(tab, nx, ny);
                if (g < mejor_grado) {
                    mejor_grado = g;
                    mejor = k;
                }
            }
        }
        if (mejor == -1)
            return 0;                              /* quedó atrapado */
        x += DX[mejor];
        y += DY[mejor];
        tab[x][y] = paso;
    }
    return 1;
}

void ps6_10(void)
{
    encabezado("PS6.10 Recorrido del caballo");
    int tab[NT][NT];

    if (recorrido_caballo(tab, 0, 0)) {
        printf("Recorrido completo (64 movimientos) desde la esquina (1,1):\n");
        imprimir_matriz(NT, NT, tab);
    } else
        printf("No se encontró recorrido completo.\n");
}

/* =====================================================================
   PS6.11  Visitantes a 10 centros turísticos
   SEC[centro][mes][año]  -> 10 centros, 12 meses, 5 años (año 4 = último)
   Los datos de ejemplo son inventados.
   ===================================================================== */
#define NC 10

void llenar_visitantes(int sec[NC][12][5])
{
    for (int c = 0; c < NC; c++)
        for (int m = 0; m < 12; m++)
            for (int a = 0; a < 5; a++)
                sec[c][m][a] = 20000 + (c * 7919 + m * 104729 + a * 15485863 + c * m * 31) % 80000;
}

/* a) total de visitantes de cada centro en los cinco años */
void total_por_centro(int sec[NC][12][5], long total[NC])
{
    for (int c = 0; c < NC; c++) {
        total[c] = 0;
        for (int m = 0; m < 12; m++)
            for (int a = 0; a < 5; a++)
                total[c] += sec[c][m][a];
    }
}

/* b) índices del centro más y menos visitado */
void centros_extremos(const long total[], int n, int *cmax, int *cmin)
{
    *cmax = *cmin = 0;
    for (int c = 1; c < n; c++) {
        if (total[c] > total[*cmax]) *cmax = c;
        if (total[c] < total[*cmin]) *cmin = c;
    }
}

/* c) mes del último año con mayor y menor afluencia (sumando los 10 centros) */
void mes_extremo_ultimo_anio(int sec[NC][12][5], int *mmax, int *mmin,
                             long *vmax, long *vmin)
{
    long total_mes[12];
    for (int m = 0; m < 12; m++) {
        total_mes[m] = 0;
        for (int c = 0; c < NC; c++)
            total_mes[m] += sec[c][m][4];
    }
    *mmax = *mmin = 0;
    for (int m = 1; m < 12; m++) {
        if (total_mes[m] > total_mes[*mmax]) *mmax = m;
        if (total_mes[m] < total_mes[*mmin]) *mmin = m;
    }
    *vmax = total_mes[*mmax];
    *vmin = total_mes[*mmin];
}

void ps6_11(void)
{
    encabezado("PS6.11 Centros turísticos");
    static int sec[NC][12][5];
    long total[NC];
    llenar_visitantes(sec);

    total_por_centro(sec, total);
    printf("a) Visitantes por centro (5 años):\n");
    for (int c = 0; c < NC; c++)
        printf("   Centro %2d: %ld\n", c + 1, total[c]);

    int cmax, cmin;
    centros_extremos(total, NC, &cmax, &cmin);
    printf("b) Más visitado: centro %d (%ld). Menos visitado: centro %d (%ld)\n",
           cmax + 1, total[cmax], cmin + 1, total[cmin]);

    int mmax, mmin;
    long vmax, vmin;
    mes_extremo_ultimo_anio(sec, &mmax, &mmin, &vmax, &vmin);
    printf("c) Último año -> mayor afluencia: %s (%ld); menor: %s (%ld)\n",
           MESES[mmax], vmax, MESES[mmin], vmin);
}

/* =====================================================================
   PS6.12  Asistentes a estadios de fútbol
   FUT[estadio][mes][año]  -> año 4 = último.
   OJO: el enunciado dice "diez estadios", pero el dato se declara como
   FUT[7][12][5]. Se sigue la declaración (7); para usar 10 basta con
   cambiar la constante NE.
   ===================================================================== */
#define NE 7

void llenar_asistentes(int fut[NE][12][5])
{
    for (int e = 0; e < NE; e++)
        for (int m = 0; m < 12; m++)
            for (int a = 0; a < 5; a++)
                fut[e][m][a] = 30000 + (e * 6007 + m * 7727 + a * 1299709 + e * m * 53) % 60000;
}

/* a) asistentes del último año por estadio y orden de mayor a menor */
void ultimo_anio_ordenado(int fut[NE][12][5], int orden[NE], long total[NE])
{
    for (int e = 0; e < NE; e++) {
        total[e] = 0;
        for (int m = 0; m < 12; m++)
            total[e] += fut[e][m][4];
        orden[e] = e;
    }
    for (int i = 1; i < NE; i++) {              /* inserción sobre los índices */
        int x = orden[i], j = i - 1;
        while (j >= 0 && total[orden[j]] < total[x]) {
            orden[j + 1] = orden[j];
            j--;
        }
        orden[j + 1] = x;
    }
}

/* b) estadios con mayor y menor asistencia en los cinco años */
void estadios_extremos(int fut[NE][12][5], int *emax, int *emin,
                       long *vmax, long *vmin)
{
    long total[NE];
    for (int e = 0; e < NE; e++) {
        total[e] = 0;
        for (int m = 0; m < 12; m++)
            for (int a = 0; a < 5; a++)
                total[e] += fut[e][m][a];
    }
    *emax = *emin = 0;
    for (int e = 1; e < NE; e++) {
        if (total[e] > total[*emax]) *emax = e;
        if (total[e] < total[*emin]) *emin = e;
    }
    *vmax = total[*emax];
    *vmin = total[*emin];
}

/* c) mes del último año con mayor asistencia de un estadio */
int mes_mayor_ultimo_anio(int fut[NE][12][5], int estadio)
{
    int mejor = 0;
    for (int m = 1; m < 12; m++)
        if (fut[estadio][m][4] > fut[estadio][mejor][4])
            mejor = m;
    return mejor;
}

void ps6_12(void)
{
    encabezado("PS6.12 Estadios de fútbol");
    static int fut[NE][12][5];
    int orden[NE];
    long total[NE];
    llenar_asistentes(fut);

    ultimo_anio_ordenado(fut, orden, total);
    printf("a) Asistentes del último año (mayor a menor):\n");
    for (int i = 0; i < NE; i++)
        printf("   Estadio %d: %ld\n", orden[i] + 1, total[orden[i]]);

    int emax, emin;
    long vmax, vmin;
    estadios_extremos(fut, &emax, &emin, &vmax, &vmin);
    printf("b) 5 años -> mayor afluencia: estadio %d (%ld); menor: estadio %d (%ld)\n",
           emax + 1, vmax, emin + 1, vmin);

    printf("c) Mes de mayor afluencia del último año, por estadio:\n");
    for (int e = 0; e < NE; e++) {
        int m = mes_mayor_ultimo_anio(fut, e);
        printf("   Estadio %d: %s (%d asistentes)\n", e + 1, MESES[m], fut[e][m][4]);
    }
}

/* ===================================================================== */
int main(void)
{
    ps6_1();
    ps6_2();
    ps6_3();
    ps6_4();
    ps6_5();
    ps6_6();
    ps6_7();
    ps6_8();
    ps6_9();
    ps6_10();
    ps6_11();
    ps6_12();
    return 0;
}
