/* Pruebas automaticas: cada bloque corresponde a un escenario de openspec/specs/.
   Compilar y ejecutar desde 1-con-openspec:  make test */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../src/costos.h"

static int total = 0, fallos = 0;
#define CHECK(cond, nombre) do { total++; if (!(cond)) { fallos++; printf("  FALLA: %s\n", nombre); } } while (0)
static int casi(double a, double b) { return fabs(a - b) < 0.0001; }

static Vehiculo corolla(void)
{
    Vehiculo v; memset(&v, 0, sizeof v);
    strcpy(v.marca, "Toyota"); strcpy(v.modelo, "Corolla"); strcpy(v.placa, "ABC123");
    v.anio = 2020; v.odometro_inicial = 10000;
    return v;
}
static void cargar_ejemplo(Registro *r)
{
    Vehiculo v = corolla();
    vehiculo_definir(r, &v);
    gasto_agregar(r, "2026-01-05", CAT_COMBUSTIBLE, 40, 10100, 10, "Primer tanque", NULL);
    gasto_agregar(r, "2026-01-20", CAT_COMBUSTIBLE, 50, 10400, 12.5, "", NULL);
    gasto_agregar(r, "2026-02-10", CAT_COMBUSTIBLE, 45, 10700, 11, "", NULL);
    gasto_agregar(r, "2026-01-25", CAT_MANTENIMIENTO, 120, 10500, 0, "Cambio de aceite", NULL);
    gasto_agregar(r, "2026-02-01", CAT_SEGURO, 300, 10600, 0, "Poliza anual", NULL);
}

int main(void)
{
    Registro r; Vehiculo v; double x; int id, i;

    printf("== vehiculo ==\n");
    registro_iniciar(&r);
    v = corolla();
    CHECK(vehiculo_definir(&r, &v) == COSTOS_OK && r.tiene_vehiculo, "datos validos se aceptan");
    v = corolla(); v.anio = 1850;
    CHECK(vehiculo_definir(&r, &v) == COSTOS_ERR_ANIO, "anio 1850 se rechaza");
    v = corolla(); strcpy(v.placa, "   ");
    CHECK(vehiculo_definir(&r, &v) == COSTOS_ERR_VEHICULO, "placa vacia se rechaza");
    v = corolla(); v.odometro_inicial = -5;
    CHECK(vehiculo_definir(&r, &v) == COSTOS_ERR_ODOMETRO, "odometro negativo se rechaza");
    registro_liberar(&r);

    registro_iniciar(&r);
    v = corolla(); vehiculo_definir(&r, &v);
    gasto_agregar(&r, "2026-01-05", CAT_OTROS, 10, 10100, 0, "", NULL);
    v = corolla(); v.odometro_inicial = 12000;
    CHECK(vehiculo_definir(&r, &v) == COSTOS_ERR_ODOMETRO, "odometro inicial mayor que un gasto existente");
    registro_liberar(&r);

    printf("== gastos ==\n");
    registro_iniciar(&r);
    CHECK(gasto_agregar(&r, "2026-01-05", CAT_OTROS, 10, 100, 0, "", NULL) == COSTOS_ERR_SIN_VEHICULO, "gasto sin vehiculo");
    v = corolla(); vehiculo_definir(&r, &v);
    CHECK(gasto_agregar(&r, "2026-01-05", CAT_COMBUSTIBLE, 40, 10100, 10, "Tanque", &id) == COSTOS_OK && id == 1, "gasto valido con id 1");
    CHECK(gasto_agregar(&r, "2026-01-06", CAT_OTROS, 5, 10110, 0, "Lavado", &id) == COSTOS_OK && id == 2, "segundo gasto con id 2");
    CHECK(gasto_agregar(&r, "2026-01-07", CAT_OTROS, 0, 10120, 0, "", NULL) == COSTOS_ERR_MONTO, "monto cero");
    CHECK(gasto_agregar(&r, "2026-02-30", CAT_OTROS, 5, 10120, 0, "", NULL) == COSTOS_ERR_FECHA, "30 de febrero");
    CHECK(gasto_agregar(&r, "05/01/2026", CAT_OTROS, 5, 10120, 0, "", NULL) == COSTOS_ERR_FECHA, "formato de fecha incorrecto");
    CHECK(gasto_agregar(&r, "2026-01-08", CAT_COMBUSTIBLE, 30, 10130, 0, "", NULL) == COSTOS_ERR_LITROS, "combustible sin litros");
    CHECK(gasto_agregar(&r, "2026-01-08", CAT_OTROS, 5, 9000, 0, "", NULL) == COSTOS_ERR_KM, "km menor al odometro inicial");
    CHECK(gasto_agregar(&r, "2024-02-29", CAT_OTROS, 5, 10100, 0, "", NULL) == COSTOS_OK, "29 de febrero de anio bisiesto");
    CHECK(strcmp(r.gastos[0].fecha, "2024-02-29") == 0 && strcmp(r.gastos[2].fecha, "2026-01-06") == 0, "lista ordenada por fecha");
    CHECK(gasto_agregar(&r, "2026-03-01", CAT_OTROS, 5, 10200, 0, "a|b", NULL) == COSTOS_OK
          && strchr(r.gastos[r.cantidad - 1].descripcion, '|') == NULL, "el separador | no se guarda");
    CHECK(gasto_eliminar(&r, 2) == COSTOS_OK, "eliminar gasto existente");
    CHECK(gasto_eliminar(&r, 2) == COSTOS_ERR_NO_ENCONTRADO, "eliminar gasto inexistente");
    CHECK(gasto_agregar(&r, "2026-04-01", CAT_OTROS, 5, 10300, 0, "", &id) == COSTOS_OK && id == 5, "los ids no se reutilizan");
    registro_liberar(&r);

    printf("== reportes ==\n");
    registro_iniciar(&r);
    CHECK(costo_por_km(&r, &x) == COSTOS_ERR_SIN_DATOS, "costo por km sin datos");
    cargar_ejemplo(&r);
    CHECK(casi(total_general(&r), 555.0), "total general = 555");
    CHECK(casi(total_categoria(&r, CAT_COMBUSTIBLE), 135.0), "total combustible = 135");
    CHECK(casi(total_categoria(&r, CAT_SEGURO), 300.0), "total seguro = 300");
    CHECK(casi(total_categoria(&r, CAT_IMPUESTOS), 0.0), "total sin gastos = 0");
    CHECK(costo_por_km(&r, &x) == COSTOS_OK && casi(x, 555.0 / 700.0), "costo por km = 555/700");
    CHECK(consumo_promedio(&r, &x) == COSTOS_OK && casi(x, 600.0 / 23.5), "consumo = 600/23.5 km/l");
    registro_liberar(&r);
    registro_iniciar(&r);
    v = corolla(); vehiculo_definir(&r, &v);
    gasto_agregar(&r, "2026-01-05", CAT_COMBUSTIBLE, 40, 10100, 10, "", NULL);
    CHECK(consumo_promedio(&r, &x) == COSTOS_ERR_SIN_DATOS, "consumo con una sola carga");
    CHECK(costo_por_km(&r, &x) == COSTOS_OK, "costo por km con un gasto");
    registro_liberar(&r);

    printf("== persistencia ==\n");
    registro_iniciar(&r); cargar_ejemplo(&r);
    CHECK(registro_guardar(&r, "prueba_costos.tmp") == COSTOS_OK, "guardar archivo");
    { Registro b; registro_iniciar(&b);
      CHECK(registro_cargar(&b, "prueba_costos.tmp") == COSTOS_OK, "cargar archivo");
      CHECK(b.tiene_vehiculo && strcmp(b.vehiculo.placa, "ABC123") == 0 && b.cantidad == 5, "se recuperan vehiculo y 5 gastos");
      CHECK(casi(total_general(&b), total_general(&r)), "mismo total despues de cargar");
      CHECK(gasto_agregar(&b, "2026-03-01", CAT_OTROS, 1, 10800, 0, "", &id) == COSTOS_OK && id == 6, "id siguiente se conserva");
      registro_liberar(&b); }
    { Registro b; registro_iniciar(&b);
      CHECK(registro_cargar(&b, "no_existe_xyz.tmp") == COSTOS_ERR_ARCHIVO, "archivo inexistente");
      registro_liberar(&b); }
    { FILE *f = fopen("prueba_costos.tmp", "w"); Registro b; registro_iniciar(&b);
      fputs("COSTOS_VEHICULO 1\nGASTO|x|basura\n", f); fclose(f);
      CHECK(registro_cargar(&b, "prueba_costos.tmp") == COSTOS_ERR_FORMATO && b.cantidad == 0, "archivo danado se rechaza");
      registro_liberar(&b); }
    remove("prueba_costos.tmp");
    for (i = 0; i < 1000; i++) gasto_agregar(&r, "2026-05-01", CAT_OTROS, 1, 20000 + i, 0, "", NULL);
    CHECK(r.cantidad == 1005, "el arreglo crece dinamicamente (1005 gastos)");
    registro_liberar(&r);

    printf("\n%d pruebas, %d fallas\n", total, fallos);
    return fallos ? 1 : 0;
}
