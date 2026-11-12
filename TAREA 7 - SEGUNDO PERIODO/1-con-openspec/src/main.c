/* main.c - version de consola del programa de gestion de costos de vehiculo.
   Compilar: gcc main.c costos.c -o costos_vehiculo (ver Makefile) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "costos.h"
#ifdef _WIN32
#include <windows.h>
#endif

#define ARCHIVO "vehiculo.dat"

static void leer_linea(const char *msg, char *dest, int n)
{
    char tmp[512];
    printf("%s", msg);
    if (!fgets(tmp, sizeof tmp, stdin)) { dest[0] = '\0'; return; }
    tmp[strcspn(tmp, "\r\n")] = '\0';
    strncpy(dest, tmp, (size_t)n - 1); dest[n - 1] = '\0';
}
static long leer_long(const char *msg)   { char b[64]; leer_linea(msg, b, 64); return atol(b); }
static double leer_double(const char *msg) { char b[64]; leer_linea(msg, b, 64); return atof(b); }

static void datos_vehiculo(Registro *r)
{
    Vehiculo v; int rc; char b[64];
    memset(&v, 0, sizeof v);
    if (r->tiene_vehiculo) printf("(Actual: %s %s, placa %s)\n", r->vehiculo.marca, r->vehiculo.modelo, r->vehiculo.placa);
    leer_linea("Marca: ", v.marca, COSTOS_TXT);
    leer_linea("Modelo: ", v.modelo, COSTOS_TXT);
    leer_linea("Placa: ", v.placa, 16);
    leer_linea("Año: ", b, 64); v.anio = atoi(b);
    v.odometro_inicial = leer_long("Odómetro inicial (km): ");
    rc = vehiculo_definir(r, &v);
    printf("%s\n", costos_mensaje(rc));
}

static void registrar_gasto(Registro *r)
{
    char fecha[64], desc[COSTOS_DESC]; int i, op, rc, id; double monto, litros = 0; long km;
    time_t t = time(NULL); char hoy[16];
    strftime(hoy, sizeof hoy, "%Y-%m-%d", localtime(&t));
    leer_linea("Fecha AAAA-MM-DD (Enter = hoy): ", fecha, 64);
    if (fecha[0] == '\0') strcpy(fecha, hoy);
    for (i = 0; i < CAT_CANTIDAD; i++) printf("  %d) %s\n", i + 1, categoria_nombre((Categoria)i));
    op = (int)leer_long("Categoría: ") - 1;
    monto = leer_double("Monto: ");
    km = leer_long("Kilometraje actual del odómetro: ");
    if (op == CAT_COMBUSTIBLE) litros = leer_double("Litros cargados: ");
    leer_linea("Descripción (opcional): ", desc, COSTOS_DESC);
    rc = gasto_agregar(r, fecha, (Categoria)op, monto, km, litros, desc, &id);
    if (rc == COSTOS_OK) printf("Gasto registrado con el número %d.\n", id);
    else printf("%s\n", costos_mensaje(rc));
}

static void listar(const Registro *r)
{
    int i;
    if (r->cantidad == 0) { printf("No hay gastos registrados.\n"); return; }
    printf("%-4s %-10s %-14s %10s %8s %7s  %s\n", "No.", "Fecha", "Categoría", "Monto", "Km", "Litros", "Descripción");
    for (i = 0; i < r->cantidad; i++) {
        const Gasto *g = &r->gastos[i];
        printf("%-4d %-10s %-14s %10.2f %8ld %7.2f  %s\n", g->id, g->fecha,
               categoria_nombre(g->categoria), g->monto, g->km, g->litros, g->descripcion);
    }
}

static void eliminar(Registro *r)
{
    int id = (int)leer_long("Número del gasto a eliminar: ");
    printf("%s\n", costos_mensaje(gasto_eliminar(r, id)));
}

static void reportes(const Registro *r)
{
    int i; double x;
    printf("\n--- Resumen de costos ---\n");
    if (r->tiene_vehiculo)
        printf("Vehículo: %s %s (%d), placa %s\n", r->vehiculo.marca, r->vehiculo.modelo, r->vehiculo.anio, r->vehiculo.placa);
    for (i = 0; i < CAT_CANTIDAD; i++)
        printf("  %-14s %12.2f\n", categoria_nombre((Categoria)i), total_categoria(r, (Categoria)i));
    printf("  %-14s %12.2f\n", "TOTAL", total_general(r));
    if (costo_por_km(r, &x) == COSTOS_OK) printf("Costo por kilómetro: %.4f\n", x);
    else printf("Costo por kilómetro: no disponible\n");
    if (consumo_promedio(r, &x) == COSTOS_OK) printf("Consumo promedio: %.2f km/l\n", x);
    else printf("Consumo promedio: no disponible (se necesitan 2 o más cargas de combustible)\n");
}

int main(void)
{
    Registro reg; char b[16]; int op, rc;
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    registro_iniciar(&reg);
    rc = registro_cargar(&reg, ARCHIVO);
    if (rc == COSTOS_OK) printf("Datos cargados de %s.\n", ARCHIVO);
    else if (rc == COSTOS_ERR_FORMATO) printf("Aviso: %s tiene un formato inválido; se inicia vacío.\n", ARCHIVO);
    do {
        printf("\n=== Gestión de costos del vehículo ===\n");
        printf("1) Datos del vehículo   2) Registrar gasto   3) Listar gastos\n");
        printf("4) Eliminar gasto       5) Reportes          6) Guardar   0) Salir\n");
        leer_linea("Opción: ", b, 16); op = atoi(b);
        switch (op) {
            case 1: datos_vehiculo(&reg); break;
            case 2: registrar_gasto(&reg); break;
            case 3: listar(&reg); break;
            case 4: eliminar(&reg); break;
            case 5: reportes(&reg); break;
            case 6: printf("%s\n", registro_guardar(&reg, ARCHIVO) == COSTOS_OK ? "Guardado." : "No se pudo guardar."); break;
        }
    } while (op != 0 && !feof(stdin));
    if (registro_guardar(&reg, ARCHIVO) == COSTOS_OK) printf("Datos guardados en %s.\n", ARCHIVO);
    registro_liberar(&reg);
    return 0;
}
