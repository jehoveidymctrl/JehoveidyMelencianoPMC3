/* costos.c - implementacion del nucleo. Compilar junto con main.c o con las pruebas. */
#include "costos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

static const char *NOMBRES[CAT_CANTIDAD] = {
    "Combustible", "Mantenimiento", "Reparación", "Seguro", "Impuestos", "Otros"
};

const char *categoria_nombre(Categoria cat)
{
    return (cat >= 0 && cat < CAT_CANTIDAD) ? NOMBRES[cat] : "?";
}

const char *costos_mensaje(int codigo)
{
    switch (codigo) {
        case COSTOS_OK:               return "Operación realizada.";
        case COSTOS_ERR_VEHICULO:     return "Marca, modelo y placa son obligatorios.";
        case COSTOS_ERR_ANIO:         return "El año no es válido.";
        case COSTOS_ERR_ODOMETRO:     return "El odómetro inicial no es válido.";
        case COSTOS_ERR_SIN_VEHICULO: return "Primero debe registrar los datos del vehículo.";
        case COSTOS_ERR_FECHA:        return "La fecha debe tener el formato AAAA-MM-DD y existir.";
        case COSTOS_ERR_CATEGORIA:    return "La categoría no es válida.";
        case COSTOS_ERR_MONTO:        return "El monto debe ser mayor que cero.";
        case COSTOS_ERR_KM:           return "El kilometraje no puede ser menor al odómetro inicial.";
        case COSTOS_ERR_LITROS:       return "Los litros son inválidos (en combustible deben ser mayores que cero).";
        case COSTOS_ERR_NO_ENCONTRADO:return "No existe un gasto con ese número.";
        case COSTOS_ERR_SIN_DATOS:    return "No hay datos suficientes para calcularlo.";
        case COSTOS_ERR_ARCHIVO:      return "No se pudo abrir el archivo.";
        case COSTOS_ERR_FORMATO:      return "El archivo no tiene un formato válido.";
        case COSTOS_ERR_MEMORIA:      return "No hay memoria suficiente.";
    }
    return "Error desconocido.";
}

/* ---------- utilidades internas ---------- */

/* Copia s en dest (tamano n): recorta espacios, cambia '|' y saltos de linea,
   y no corta un caracter UTF-8 por la mitad. Devuelve la longitud copiada. */
static size_t copiar_texto(char *dest, size_t n, const char *s)
{
    size_t len, i;
    while (*s != '\0' && isspace((unsigned char)*s)) s++;
    len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) len--;
    if (len > n - 1) {
        len = n - 1;
        while (len > 0 && ((unsigned char)s[len] & 0xC0) == 0x80) len--;
    }
    for (i = 0; i < len; i++)
        dest[i] = (s[i] == '|') ? '/' : (s[i] == '\n' || s[i] == '\r' || s[i] == '\t') ? ' ' : s[i];
    dest[len] = '\0';
    return len;
}

static int anio_actual(void)
{
    time_t t = time(NULL);
    return localtime(&t)->tm_year + 1900;
}

int fecha_valida(const char *f)
{
    static const int dias[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int y, m, d, i, dim;
    if (f == NULL || strlen(f) != 10) return 0;
    for (i = 0; i < 10; i++) {
        if (i == 4 || i == 7) { if (f[i] != '-') return 0; }
        else if (!isdigit((unsigned char)f[i])) return 0;
    }
    if (sscanf(f, "%4d-%2d-%2d", &y, &m, &d) != 3) return 0;
    if (y < 1900 || y > 2100 || m < 1 || m > 12) return 0;
    dim = dias[m - 1];
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)) dim = 29;
    return d >= 1 && d <= dim;
}

/* ---------- registro ---------- */

void registro_iniciar(Registro *r)
{
    memset(r, 0, sizeof *r);
    r->siguiente_id = 1;
}

void registro_liberar(Registro *r)
{
    free(r->gastos);
    registro_iniciar(r);
}

int vehiculo_definir(Registro *r, const Vehiculo *v)
{
    Vehiculo n;
    int i;
    memset(&n, 0, sizeof n);
    copiar_texto(n.marca, sizeof n.marca, v->marca);
    copiar_texto(n.modelo, sizeof n.modelo, v->modelo);
    copiar_texto(n.placa, sizeof n.placa, v->placa);
    if (n.marca[0] == '\0' || n.modelo[0] == '\0' || n.placa[0] == '\0') return COSTOS_ERR_VEHICULO;
    if (v->anio < 1900 || v->anio > anio_actual() + 1) return COSTOS_ERR_ANIO;
    if (v->odometro_inicial < 0) return COSTOS_ERR_ODOMETRO;
    for (i = 0; i < r->cantidad; i++)               /* no debe contradecir gastos ya registrados */
        if (r->gastos[i].km < v->odometro_inicial) return COSTOS_ERR_ODOMETRO;
    n.anio = v->anio;
    n.odometro_inicial = v->odometro_inicial;
    r->vehiculo = n;
    r->tiene_vehiculo = 1;
    return COSTOS_OK;
}

static int validar_gasto(const Registro *r, const char *fecha, Categoria cat,
                         double monto, long km, double litros)
{
    if (!r->tiene_vehiculo) return COSTOS_ERR_SIN_VEHICULO;
    if (!fecha_valida(fecha)) return COSTOS_ERR_FECHA;
    if (cat < 0 || cat >= CAT_CANTIDAD) return COSTOS_ERR_CATEGORIA;
    if (!(monto > 0.0)) return COSTOS_ERR_MONTO;
    if (km < r->vehiculo.odometro_inicial) return COSTOS_ERR_KM;
    if (litros < 0.0 || (cat == CAT_COMBUSTIBLE && !(litros > 0.0))) return COSTOS_ERR_LITROS;
    return COSTOS_OK;
}

/* inserta conservando el orden por fecha (a igual fecha, por orden de llegada) */
static int insertar(Registro *r, const Gasto *g)
{
    int pos;
    if (r->cantidad == r->capacidad) {
        int nueva = r->capacidad ? r->capacidad * 2 : 8;
        Gasto *tmp = realloc(r->gastos, (size_t)nueva * sizeof(Gasto));
        if (!tmp) return COSTOS_ERR_MEMORIA;
        r->gastos = tmp; r->capacidad = nueva;
    }
    pos = r->cantidad;
    while (pos > 0 && strcmp(r->gastos[pos - 1].fecha, g->fecha) > 0) {
        r->gastos[pos] = r->gastos[pos - 1];
        pos--;
    }
    r->gastos[pos] = *g;
    r->cantidad++;
    return COSTOS_OK;
}

int gasto_agregar(Registro *r, const char *fecha, Categoria cat, double monto,
                  long km, double litros, const char *descripcion, int *id_nuevo)
{
    Gasto g; int rc;
    rc = validar_gasto(r, fecha, cat, monto, km, litros);
    if (rc != COSTOS_OK) return rc;
    memset(&g, 0, sizeof g);
    g.id = r->siguiente_id;
    strcpy(g.fecha, fecha);
    g.categoria = cat; g.monto = monto; g.km = km; g.litros = litros;
    copiar_texto(g.descripcion, sizeof g.descripcion, descripcion ? descripcion : "");
    rc = insertar(r, &g);
    if (rc != COSTOS_OK) return rc;
    r->siguiente_id++;
    if (id_nuevo) *id_nuevo = g.id;
    return COSTOS_OK;
}

int gasto_eliminar(Registro *r, int id)
{
    int i;
    for (i = 0; i < r->cantidad; i++)
        if (r->gastos[i].id == id) {
            memmove(&r->gastos[i], &r->gastos[i + 1], (size_t)(r->cantidad - i - 1) * sizeof(Gasto));
            r->cantidad--;
            return COSTOS_OK;
        }
    return COSTOS_ERR_NO_ENCONTRADO;
}

/* ---------- reportes ---------- */

double total_general(const Registro *r)
{
    double s = 0; int i;
    for (i = 0; i < r->cantidad; i++) s += r->gastos[i].monto;
    return s;
}

double total_categoria(const Registro *r, Categoria cat)
{
    double s = 0; int i;
    for (i = 0; i < r->cantidad; i++)
        if (r->gastos[i].categoria == cat) s += r->gastos[i].monto;
    return s;
}

int costo_por_km(const Registro *r, double *resultado)
{
    long kmax; int i;
    if (!r->tiene_vehiculo || r->cantidad == 0) return COSTOS_ERR_SIN_DATOS;
    kmax = r->gastos[0].km;
    for (i = 1; i < r->cantidad; i++) if (r->gastos[i].km > kmax) kmax = r->gastos[i].km;
    if (kmax - r->vehiculo.odometro_inicial <= 0) return COSTOS_ERR_SIN_DATOS;
    *resultado = total_general(r) / (double)(kmax - r->vehiculo.odometro_inicial);
    return COSTOS_OK;
}

/* Consumo = (km de la ultima carga - km de la primera) / litros cargados despues de la primera */
int consumo_promedio(const Registro *r, double *km_por_litro)
{
    int i, imin = -1; long kmin, kmax = 0; double litros = 0;
    for (i = 0; i < r->cantidad; i++) {
        const Gasto *g = &r->gastos[i];
        if (g->categoria != CAT_COMBUSTIBLE || g->litros <= 0) continue;
        if (imin < 0 || g->km < r->gastos[imin].km) imin = i;
        if (g->km > kmax) kmax = g->km;
    }
    if (imin < 0) return COSTOS_ERR_SIN_DATOS;
    kmin = r->gastos[imin].km;
    if (kmax <= kmin) return COSTOS_ERR_SIN_DATOS;
    for (i = 0; i < r->cantidad; i++) {
        const Gasto *g = &r->gastos[i];
        if (i == imin || g->categoria != CAT_COMBUSTIBLE || g->litros <= 0) continue;
        litros += g->litros;
    }
    if (litros <= 0) return COSTOS_ERR_SIN_DATOS;
    *km_por_litro = (double)(kmax - kmin) / litros;
    return COSTOS_OK;
}

/* ---------- archivo de texto ----------
   COSTOS_VEHICULO 1
   VEHICULO|marca|modelo|placa|anio|odometro
   GASTO|id|fecha|categoria|monto|km|litros|descripcion                         */

int registro_guardar(const Registro *r, const char *ruta)
{
    FILE *f; int i;
    f = fopen(ruta, "w");
    if (!f) return COSTOS_ERR_ARCHIVO;
    fprintf(f, "COSTOS_VEHICULO 1\n");
    if (r->tiene_vehiculo)
        fprintf(f, "VEHICULO|%s|%s|%s|%d|%ld\n", r->vehiculo.marca, r->vehiculo.modelo,
                r->vehiculo.placa, r->vehiculo.anio, r->vehiculo.odometro_inicial);
    for (i = 0; i < r->cantidad; i++) {
        const Gasto *g = &r->gastos[i];
        fprintf(f, "GASTO|%d|%s|%d|%.2f|%ld|%.2f|%s\n", g->id, g->fecha, (int)g->categoria,
                g->monto, g->km, g->litros, g->descripcion);
    }
    return fclose(f) == 0 ? COSTOS_OK : COSTOS_ERR_ARCHIVO;
}

/* divide linea en campos separados por '|' (modifica linea); devuelve cuantos */
static int dividir(char *linea, char *campo[], int max)
{
    int n = 0; char *p = linea;
    campo[n++] = p;
    while (n < max && (p = strchr(p, '|')) != NULL) { *p++ = '\0'; campo[n++] = p; }
    return n;
}

int registro_cargar(Registro *r, const char *ruta)
{
    Registro nuevo; FILE *f; char linea[512], *c[8]; int n, rc = COSTOS_OK, maxid = 0, cabecera = 0;
    f = fopen(ruta, "r");
    if (!f) return COSTOS_ERR_ARCHIVO;
    registro_iniciar(&nuevo);
    while (rc == COSTOS_OK && fgets(linea, sizeof linea, f)) {
        linea[strcspn(linea, "\r\n")] = '\0';
        if (linea[0] == '\0') continue;
        if (!cabecera) { if (strncmp(linea, "COSTOS_VEHICULO", 15) != 0) rc = COSTOS_ERR_FORMATO; cabecera = 1; continue; }
        n = dividir(linea, c, 8);
        if (strcmp(c[0], "VEHICULO") == 0 && n == 6) {
            Vehiculo v; memset(&v, 0, sizeof v);
            copiar_texto(v.marca, sizeof v.marca, c[1]);
            copiar_texto(v.modelo, sizeof v.modelo, c[2]);
            copiar_texto(v.placa, sizeof v.placa, c[3]);
            v.anio = atoi(c[4]); v.odometro_inicial = atol(c[5]);
            if (vehiculo_definir(&nuevo, &v) != COSTOS_OK) rc = COSTOS_ERR_FORMATO;
        } else if (strcmp(c[0], "GASTO") == 0 && n == 8) {
            Gasto g; int cat = atoi(c[3]); memset(&g, 0, sizeof g);
            g.id = atoi(c[1]); g.categoria = (Categoria)cat;
            g.monto = atof(c[4]); g.km = atol(c[5]); g.litros = atof(c[6]);
            if (strlen(c[2]) != 10) { rc = COSTOS_ERR_FORMATO; break; }
            strcpy(g.fecha, c[2]);
            copiar_texto(g.descripcion, sizeof g.descripcion, c[7]);
            if (g.id <= 0 || validar_gasto(&nuevo, g.fecha, g.categoria, g.monto, g.km, g.litros) != COSTOS_OK)
                rc = COSTOS_ERR_FORMATO;
            else if ((rc = insertar(&nuevo, &g)) == COSTOS_OK && g.id > maxid) maxid = g.id;
        } else rc = COSTOS_ERR_FORMATO;
    }
    fclose(f);
    if (rc == COSTOS_OK && !cabecera) rc = COSTOS_ERR_FORMATO;
    if (rc != COSTOS_OK) { registro_liberar(&nuevo); return rc; }
    nuevo.siguiente_id = maxid + 1;
    registro_liberar(r);
    *r = nuevo;
    return COSTOS_OK;
}
