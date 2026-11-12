/* Gestion de costos de un vehiculo - version directa (sin OpenSpec).
   Todo en un solo archivo, con arreglos de tamano fijo.
   Compilar: gcc costos_vehiculo.c -o costos_vehiculo */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_GASTOS 500
#define ARCHIVO "vehiculo_simple.txt"

const char *CATEGORIAS[6] = {"Combustible", "Mantenimiento", "Reparación", "Seguro", "Impuestos", "Otros"};

struct Gasto { char fecha[11]; int cat; double monto; long km; double litros; char desc[60]; };

char marca[40] = "", modelo[40] = "", placa[16] = "";
int anio = 0, hay_vehiculo = 0;
long odometro = 0;
struct Gasto gastos[MAX_GASTOS];
int n = 0;

void leer(const char *msg, char *dest, int tam)
{
    char t[300];
    printf("%s", msg);
    if (!fgets(t, sizeof t, stdin)) { dest[0] = '\0'; return; }
    t[strcspn(t, "\r\n")] = '\0';
    strncpy(dest, t, tam - 1); dest[tam - 1] = '\0';
}
double leer_num(const char *msg) { char t[64]; leer(msg, t, 64); return atof(t); }

int fecha_ok(const char *f)
{
    int a, m, d, dm[12] = {31,28,31,30,31,30,31,31,30,31,30,31}, i;
    if (strlen(f) != 10) return 0;
    for (i = 0; i < 10; i++)
        if (i == 4 || i == 7 ? f[i] != '-' : (f[i] < '0' || f[i] > '9')) return 0;
    sscanf(f, "%d-%d-%d", &a, &m, &d);
    if (m < 1 || m > 12 || a < 1900) return 0;
    if (m == 2 && (a % 4 == 0 && (a % 100 != 0 || a % 400 == 0))) dm[1] = 29;
    return d >= 1 && d <= dm[m - 1];
}

void guardar(void)
{
    FILE *f = fopen(ARCHIVO, "w");
    int i;
    if (!f) { printf("No se pudo guardar.\n"); return; }
    fprintf(f, "%s;%s;%s;%d;%ld\n", marca, modelo, placa, anio, odometro);
    for (i = 0; i < n; i++)
        fprintf(f, "%s;%d;%.2f;%ld;%.2f;%s\n", gastos[i].fecha, gastos[i].cat, gastos[i].monto,
                gastos[i].km, gastos[i].litros, gastos[i].desc);
    fclose(f);
}

void cargar(void)
{
    FILE *f = fopen(ARCHIVO, "r");
    char linea[300];
    if (!f) return;
    if (fgets(linea, sizeof linea, f) &&
        sscanf(linea, "%39[^;];%39[^;];%15[^;];%d;%ld", marca, modelo, placa, &anio, &odometro) == 5)
        hay_vehiculo = 1;
    while (n < MAX_GASTOS && fgets(linea, sizeof linea, f)) {
        struct Gasto g; g.desc[0] = '\0';
        linea[strcspn(linea, "\r\n")] = '\0';
        if (sscanf(linea, "%10[^;];%d;%lf;%ld;%lf;%59[^\n]", g.fecha, &g.cat, &g.monto, &g.km, &g.litros, g.desc) >= 5)
            gastos[n++] = g;
    }
    fclose(f);
}

void datos_vehiculo(void)
{
    char t[64];
    leer("Marca: ", marca, 40); leer("Modelo: ", modelo, 40); leer("Placa: ", placa, 16);
    leer("Año: ", t, 64); anio = atoi(t);
    odometro = (long)leer_num("Odómetro inicial (km): ");
    if (marca[0] == '\0' || modelo[0] == '\0' || placa[0] == '\0' || anio < 1900 || odometro < 0) {
        printf("Datos inválidos.\n"); hay_vehiculo = 0;
    } else hay_vehiculo = 1;
}

void nuevo_gasto(void)
{
    struct Gasto g; char t[64]; int i, pos;
    if (!hay_vehiculo) { printf("Primero registre el vehículo.\n"); return; }
    if (n >= MAX_GASTOS) { printf("Ya no caben más gastos.\n"); return; }
    leer("Fecha AAAA-MM-DD: ", g.fecha, 11);
    if (!fecha_ok(g.fecha)) { printf("Fecha inválida.\n"); return; }
    for (i = 0; i < 6; i++) printf("  %d) %s\n", i + 1, CATEGORIAS[i]);
    leer("Categoría: ", t, 64); g.cat = atoi(t) - 1;
    if (g.cat < 0 || g.cat > 5) { printf("Categoría inválida.\n"); return; }
    g.monto = leer_num("Monto: ");
    if (g.monto <= 0) { printf("El monto debe ser positivo.\n"); return; }
    g.km = (long)leer_num("Kilometraje del odómetro: ");
    if (g.km < odometro) { printf("El kilometraje es menor al inicial.\n"); return; }
    g.litros = 0;
    if (g.cat == 0) {
        g.litros = leer_num("Litros: ");
        if (g.litros <= 0) { printf("Los litros deben ser positivos.\n"); return; }
    }
    leer("Descripción: ", g.desc, 60);
    for (i = 0; g.desc[i]; i++) if (g.desc[i] == ';') g.desc[i] = ',';
    pos = n;                                   /* mantener orden por fecha */
    while (pos > 0 && strcmp(gastos[pos - 1].fecha, g.fecha) > 0) { gastos[pos] = gastos[pos - 1]; pos--; }
    gastos[pos] = g; n++;
    printf("Gasto agregado.\n");
}

void listar(void)
{
    int i;
    if (n == 0) { printf("Sin gastos.\n"); return; }
    for (i = 0; i < n; i++)
        printf("%3d  %s  %-14s %10.2f  %8ld km  %6.2f l  %s\n", i + 1, gastos[i].fecha,
               CATEGORIAS[gastos[i].cat], gastos[i].monto, gastos[i].km, gastos[i].litros, gastos[i].desc);
}

void eliminar(void)
{
    int k = (int)leer_num("Número de línea a eliminar: "), i;
    if (k < 1 || k > n) { printf("No existe esa línea.\n"); return; }
    for (i = k - 1; i < n - 1; i++) gastos[i] = gastos[i + 1];
    n--;
    printf("Eliminado.\n");
}

void reportes(void)
{
    double tot = 0, cat[6] = {0}, lit = 0;
    long kmax = 0, kmin = 0; int i, imin = -1;
    for (i = 0; i < n; i++) {
        tot += gastos[i].monto; cat[gastos[i].cat] += gastos[i].monto;
        if (gastos[i].km > kmax) kmax = gastos[i].km;
        if (gastos[i].cat == 0 && (imin < 0 || gastos[i].km < kmin)) { imin = i; kmin = gastos[i].km; }
    }
    printf("\n--- Resumen ---\n");
    for (i = 0; i < 6; i++) printf("  %-14s %12.2f\n", CATEGORIAS[i], cat[i]);
    printf("  %-14s %12.2f\n", "TOTAL", tot);
    if (hay_vehiculo && n > 0 && kmax > odometro) printf("Costo por km: %.4f\n", tot / (kmax - odometro));
    else printf("Costo por km: no disponible\n");
    for (i = 0; i < n; i++) if (gastos[i].cat == 0 && i != imin) lit += gastos[i].litros;
    if (imin >= 0 && lit > 0) {
        long kfin = kmin;
        for (i = 0; i < n; i++) if (gastos[i].cat == 0 && gastos[i].km > kfin) kfin = gastos[i].km;
        if (kfin > kmin) { printf("Consumo: %.2f km/l\n", (kfin - kmin) / lit); return; }
    }
    printf("Consumo: no disponible\n");
}

int main(void)
{
    char t[16]; int op;
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    cargar();
    do {
        printf("\n== Costos del vehículo ==\n1) Vehículo 2) Nuevo gasto 3) Listar 4) Eliminar 5) Reportes 0) Salir\n");
        leer("Opción: ", t, 16); op = atoi(t);
        if (op == 1) datos_vehiculo(); else if (op == 2) nuevo_gasto();
        else if (op == 3) listar(); else if (op == 4) eliminar(); else if (op == 5) reportes();
    } while (op != 0 && !feof(stdin));
    guardar();
    return 0;
}
