/* Capitulo 8 - Estructuras y uniones. Problemas PS8.1 a PS8.6.
   Compilar: gcc cap8_ejercicios.c -o cap8
   Cada problema permite capturar los datos o usar datos de ejemplo. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ---------------- utilidades ---------------- */
static void quitar_salto(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++)
        if (s[i] == '\n' || s[i] == '\r') { s[i] = '\0'; break; }
}
static void leer_linea(char *dest, int n)
{
    char tmp[1024];
    if (!fgets(tmp, sizeof tmp, stdin)) { dest[0] = '\0'; return; }
    quitar_salto(tmp);
    strncpy(dest, tmp, n - 1);
    dest[n - 1] = '\0';
}
static int leer_int(void)       { char b[64]; leer_linea(b, 64); return atoi(b); }
static double leer_double(void) { char b[64]; leer_linea(b, 64); return atof(b); }
static void pedir(const char *msg, char *dest, int n) { printf("%s", msg); leer_linea(dest, n); }
static int pedir_int(const char *msg)       { printf("%s", msg); return leer_int(); }
static double pedir_double(const char *msg) { printf("%s", msg); return leer_double(); }
static int usar_ejemplo(void)
{
    printf("  1) Capturar datos   2) Usar datos de ejemplo\n  Opcion: ");
    return leer_int() == 2;
}
/* normaliza para comparar nombres sin importar mayusculas ni acentos (UTF-8) */
static void normalizar(const char *s, char *out)
{
    int i = 0, j = 0;
    while (s[i] != '\0') {
        unsigned char c = (unsigned char)s[i];
        if (c == 0xC3) {
            unsigned char d = (unsigned char)s[i + 1]; char r = '?';
            switch (d) {
                case 0xA1: case 0x81: r = 'a'; break;
                case 0xA9: case 0x89: r = 'e'; break;
                case 0xAD: case 0x8D: r = 'i'; break;
                case 0xB3: case 0x93: r = 'o'; break;
                case 0xBA: case 0x9A: case 0xBC: r = 'u'; break;
                case 0xB1: case 0x91: r = 'n'; break;
            }
            out[j++] = r; i += 2;
        } else { out[j++] = (char)tolower(c); i++; }
    }
    out[j] = '\0';
}
static int mismo_nombre(const char *a, const char *b)
{
    char x[128], y[128];
    normalizar(a, x); normalizar(b, y);
    return strcmp(x, y) == 0;
}

typedef struct { char calle[40]; char cp[10]; char colonia[30]; char ciudad[30]; char telefono[20]; } Domicilio;
static void capturar_domicilio(Domicilio *d)
{
    pedir("    Calle y numero: ", d->calle, 40);
    pedir("    Codigo postal: ", d->cp, 10);
    pedir("    Colonia: ", d->colonia, 30);
    pedir("    Ciudad: ", d->ciudad, 30);
    pedir("    Telefono: ", d->telefono, 20);
}
static Domicilio dom(const char *calle, const char *cp, const char *col, const char *ciu, const char *tel)
{
    Domicilio d;
    strcpy(d.calle, calle); strcpy(d.cp, cp); strcpy(d.colonia, col);
    strcpy(d.ciudad, ciu); strcpy(d.telefono, tel);
    return d;
}

/* =====================================================================
   PS8.1 Banco: depositos y retiros
   ===================================================================== */
typedef struct { long long cuenta; char nombre[40]; Domicilio dom; double saldo; } Cliente;

static int buscar_cuenta(Cliente cli[], int n, long long cuenta)   /* busqueda binaria */
{
    int lo = 0, hi = n - 1, m;
    while (lo <= hi) {
        m = (lo + hi) / 2;
        if (cli[m].cuenta == cuenta) return m;
        if (cli[m].cuenta < cuenta) lo = m + 1; else hi = m - 1;
    }
    return -1;
}
static int cmp_cliente(const void *a, const void *b)
{
    long long x = ((const Cliente *)a)->cuenta, y = ((const Cliente *)b)->cuenta;
    return (x > y) - (x < y);
}
void ps81(void)
{
    Cliente cli[100]; int n = 0, i, pos; long long cuenta; char op[16]; double monto;
    if (usar_ejemplo()) {
        Cliente e[] = {
            {1001, "Carlos Andrade", dom("Av. Amazonas 120","170150","La Carolina","Quito","022345678"), 5000.0},
            {1005, "Lucia Mora",     dom("Calle 10 de Agosto 45","170135","Centro","Quito","022111222"), 250.50},
            {1012, "Pedro Salazar",  dom("Av. 6 de Diciembre 800","170517","Mariscal","Quito","022999888"), 12000.0},
            {1020, "Ana Torres",     dom("Calle Bolivar 33","170401","San Roque","Quito","022555444"), 0.0}};
        n = 4; memcpy(cli, e, sizeof e);
    } else {
        n = pedir_int("  Numero de clientes (1 a 100): ");
        if (n < 1 || n > 100) { printf("  N fuera de rango.\n"); return; }
        for (i = 0; i < n; i++) {
            printf("  Cliente %d\n", i + 1);
            cli[i].cuenta = (long long)pedir_double("    Numero de cuenta: ");
            pedir("    Nombre: ", cli[i].nombre, 40);
            capturar_domicilio(&cli[i].dom);
            cli[i].saldo = pedir_double("    Saldo: ");
        }
        qsort(cli, n, sizeof(Cliente), cmp_cliente);
    }
    printf("\n  Cuentas: ");
    for (i = 0; i < n; i++) printf("%lld ", cli[i].cuenta);
    printf("\n");
    cuenta = (long long)pedir_double("\n  Numero de cuenta (0 = fin): ");
    while (cuenta != 0) {
        pos = buscar_cuenta(cli, n, cuenta);
        if (pos < 0) printf("  La cuenta no existe.\n");
        else {
            pedir("  D = deposito, R = retiro/cheque: ", op, 16);
            monto = pedir_double("  Monto: ");
            if (monto <= 0) printf("  Monto no valido.\n");
            else if (toupper((unsigned char)op[0]) == 'D') {
                cli[pos].saldo += monto;
                printf("  Deposito aplicado. Saldo de %s: %.2f\n", cli[pos].nombre, cli[pos].saldo);
            } else if (toupper((unsigned char)op[0]) == 'R') {
                if (monto > cli[pos].saldo) printf("  Saldo insuficiente (%.2f). Operacion rechazada.\n", cli[pos].saldo);
                else { cli[pos].saldo -= monto; printf("  Retiro autorizado. Saldo de %s: %.2f\n", cli[pos].nombre, cli[pos].saldo); }
            } else printf("  Operacion no valida.\n");
        }
        cuenta = (long long)pedir_double("\n  Numero de cuenta (0 = fin): ");
    }
    printf("\n  Saldos finales:\n");
    for (i = 0; i < n; i++) printf("  %lld  %-18s %12.2f\n", cli[i].cuenta, cli[i].nombre, cli[i].saldo);
}

/* =====================================================================
   PS8.2 Tabla de posiciones de futbol
   ===================================================================== */
typedef struct { char nombre[30]; int pj, pg, pe, pp, gf, gc, dg, pts; } Equipo;

static int cmp_equipo(const void *a, const void *b)
{
    const Equipo *x = a, *y = b;
    if (y->pts != x->pts) return y->pts - x->pts;
    if (y->dg != x->dg)   return y->dg - x->dg;
    return y->gf - x->gf;
}
static int buscar_equipo(Equipo t[], int n, const char *nombre)
{
    int i;
    for (i = 0; i < n; i++) if (mismo_nombre(t[i].nombre, nombre)) return i;
    return -1;
}
/* "America 0 - Puebla 2"  (acepta - o el guion largo) */
static int parsear_partido(char *linea, char *n1, int *g1, char *n2, int *g2)
{
    char *sep = strstr(linea, " \xE2\x80\x93 "); int lon = 5;
    char *sp, *izq, *der;
    if (!sep) { sep = strstr(linea, " - "); lon = 3; }
    if (!sep) return 0;
    *sep = '\0'; izq = linea; der = sep + lon;
    sp = strrchr(izq, ' ');  if (!sp) return 0;
    *g1 = atoi(sp + 1); *sp = '\0'; strcpy(n1, izq);
    sp = strrchr(der, ' ');  if (!sp) return 0;
    *g2 = atoi(sp + 1); *sp = '\0'; strcpy(n2, der);
    return 1;
}
static void aplicar(Equipo *e, int gf, int gc)
{
    e->pj++; e->gf += gf; e->gc += gc; e->dg = e->gf - e->gc;
    if (gf > gc) { e->pg++; e->pts += 3; }
    else if (gf == gc) { e->pe++; e->pts += 1; }
    else e->pp++;
}
static void imprimir_tabla(Equipo t[], int n)
{
    int i;
    printf("\n  %-3s %-14s %3s %3s %3s %3s %3s %3s %4s %4s\n", "#", "Equipo", "PJ", "PG", "PE", "PP", "GF", "GC", "DG", "PTS");
    for (i = 0; i < n; i++)
        printf("  %-3d %-14s %3d %3d %3d %3d %3d %3d %4d %4d\n", i + 1, t[i].nombre,
               t[i].pj, t[i].pg, t[i].pe, t[i].pp, t[i].gf, t[i].gc, t[i].dg, t[i].pts);
}
void ps82(void)
{
    Equipo futbol[20]; int n = 0, i, g1, g2, a, b; char linea[200], n1[60], n2[60];
    const char *ej[] = {"América", "Puebla", "Cruz Azul", "Veracruz", "Necaxa", "Monterrey"};
    if (usar_ejemplo()) { n = 6; for (i = 0; i < n; i++) { memset(&futbol[i], 0, sizeof(Equipo)); strcpy(futbol[i].nombre, ej[i]); } }
    else {
        n = pedir_int("  Numero de equipos (2 a 20): ");
        if (n < 2 || n > 20) { printf("  N fuera de rango.\n"); return; }
        for (i = 0; i < n; i++) { memset(&futbol[i], 0, sizeof(Equipo)); pedir("  Nombre del equipo: ", futbol[i].nombre, 30); }
    }
    printf("\n  Escribe los resultados de la fecha, uno por linea:\n  Ejemplo: America 0 - Puebla 2   (linea vacia = terminar)\n");
    for (;;) {
        printf("  > "); leer_linea(linea, 200);
        if (linea[0] == '\0') break;
        if (!parsear_partido(linea, n1, &g1, n2, &g2)) { printf("  Formato no valido.\n"); continue; }
        a = buscar_equipo(futbol, n, n1); b = buscar_equipo(futbol, n, n2);
        if (a < 0 || b < 0 || a == b) { printf("  Equipo no encontrado.\n"); continue; }
        aplicar(&futbol[a], g1, g2); aplicar(&futbol[b], g2, g1);
    }
    qsort(futbol, n, sizeof(Equipo), cmp_equipo);
    imprimir_tabla(futbol, n);
}

/* =====================================================================
   PS8.3 Profesores (universidad de Barranquilla)
   ===================================================================== */
typedef struct {
    int numero; char nombre[40]; char depto[20]; char puesto[25];
    char grado[20]; char nacionalidad[20]; double salario[12];
} Profesor;

static const char *DEPTOS[4] = {"Economía", "Derecho", "Computación", "Administración"};

static void prof(Profesor *p, int num, const char *nom, const char *dep, const char *pue,
                 const char *gra, const char *nac, double base)
{
    int m;
    p->numero = num; strcpy(p->nombre, nom); strcpy(p->depto, dep); strcpy(p->puesto, pue);
    strcpy(p->grado, gra); strcpy(p->nacionalidad, nac);
    for (m = 0; m < 12; m++) p->salario[m] = base;
}
void ps83(void)
{
    Profesor emple[200]; int n = 0, i, m, d, mejor = 0, dmax = 0;
    double total[200], tuni = 0, textr = 0, tdep[4] = {0, 0, 0, 0};
    if (usar_ejemplo()) {
        prof(&emple[0], 1, "Maria Gomez",    "Economía",       "Titular",   "Doctorado", "Colombia",  4200);
        prof(&emple[1], 2, "John Smith",     "Computación",    "Asociado",  "Maestría",  "Estados Unidos", 5200);
        prof(&emple[2], 3, "Luis Perez",     "Derecho",        "Titular",   "Doctorado", "Colombia",  3900);
        prof(&emple[3], 4, "Ana Rojas",      "Administración", "Asistente", "Maestría",  "Colombia",  3100);
        prof(&emple[4], 5, "Hans Muller",    "Computación",    "Titular",   "Doctorado", "Alemania",  6100);
        prof(&emple[5], 6, "Carlos Vega",    "Economía",       "Asociado",  "Maestría",  "Colombia",  3600);
        n = 6; emple[3].salario[11] += 2000;            /* un bono de diciembre */
    } else {
        n = pedir_int("  Numero de profesores (1 a 200): ");
        if (n < 1 || n > 200) { printf("  N fuera de rango.\n"); return; }
        for (i = 0; i < n; i++) {
            printf("  Profesor %d\n", i + 1);
            emple[i].numero = pedir_int("    Numero de empleado: ");
            pedir("    Nombre y apellido: ", emple[i].nombre, 40);
            pedir("    Departamento (Economía/Derecho/Computación/Administración): ", emple[i].depto, 20);
            pedir("    Puesto: ", emple[i].puesto, 25);
            pedir("    Grado academico: ", emple[i].grado, 20);
            pedir("    Nacionalidad: ", emple[i].nacionalidad, 20);
            for (m = 0; m < 12; m++) { printf("    Ingreso mes %d: ", m + 1); emple[i].salario[m] = leer_double(); }
        }
    }
    for (i = 0; i < n; i++) {
        total[i] = 0;
        for (m = 0; m < 12; m++) total[i] += emple[i].salario[m];
        tuni += total[i];
        if (total[i] > total[mejor]) mejor = i;
        if (!mismo_nombre(emple[i].nacionalidad, "Colombia")) textr += total[i];
        for (d = 0; d < 4; d++) if (mismo_nombre(emple[i].depto, DEPTOS[d])) tdep[d] += total[i];
    }
    for (d = 1; d < 4; d++) if (tdep[d] > tdep[dmax]) dmax = d;
    printf("\n  a) Profesor que mas gano: %s | Departamento: %s | Nacionalidad: %s\n     Ingreso total anual: %.2f\n",
           emple[mejor].nombre, emple[mejor].depto, emple[mejor].nacionalidad, total[mejor]);
    printf("  b) Pagado a profesores extranjeros: %.2f (%.2f%% de %.2f)\n", textr, tuni > 0 ? 100.0 * textr / tuni : 0.0, tuni);
    printf("  c) Departamento con mas egresos: %s (%.2f)\n", DEPTOS[dmax], tdep[dmax]);
    for (d = 0; d < 4; d++) printf("       %-15s %12.2f\n", DEPTOS[d], tdep[d]);
}

/* =====================================================================
   PS8.4 Empleados: altas, bajas y listado (arreglo ordenado por numero)
   ===================================================================== */
typedef struct { int numero; char nombre[40]; char depto[25]; Domicilio dom; double salario; } Empleado;

static int buscar_emp(Empleado e[], int n, int num)           /* devuelve posicion o -1 */
{
    int lo = 0, hi = n - 1, m;
    while (lo <= hi) {
        m = (lo + hi) / 2;
        if (e[m].numero == num) return m;
        if (e[m].numero < num) lo = m + 1; else hi = m - 1;
    }
    return -1;
}
static void alta_emp(Empleado e[], int *n, int num)
{
    int i, pos = 0; Empleado nuevo;
    if (*n >= 100) { printf("  Arreglo lleno (100 empleados).\n"); return; }
    if (buscar_emp(e, *n, num) >= 0) { printf("  Ya existe un empleado con ese numero.\n"); return; }
    nuevo.numero = num;
    pedir("  Nombre y apellido: ", nuevo.nombre, 40);
    pedir("  Departamento: ", nuevo.depto, 25);
    capturar_domicilio(&nuevo.dom);
    nuevo.salario = pedir_double("  Salario mensual: ");
    while (pos < *n && e[pos].numero < num) pos++;            /* conserva el orden */
    for (i = *n; i > pos; i--) e[i] = e[i - 1];
    e[pos] = nuevo; (*n)++;
    printf("  Alta realizada.\n");
}
static void baja_emp(Empleado e[], int *n, int num)
{
    int i, pos = buscar_emp(e, *n, num);
    if (pos < 0) { printf("  El empleado no existe.\n"); return; }
    printf("  Baja de %s.\n", e[pos].nombre);
    for (i = pos; i < *n - 1; i++) e[i] = e[i + 1];
    (*n)--;
}
static void listado_emp(Empleado e[], int n, const char *depto)
{
    int i, hay = 0;
    printf("  %-8s %-24s %12s\n", "Numero", "Nombre", "Salario");
    for (i = 0; i < n; i++)
        if (mismo_nombre(e[i].depto, depto)) {
            printf("  %-8d %-24s %12.2f\n", e[i].numero, e[i].nombre, e[i].salario);
            hay = 1;
        }
    if (!hay) printf("  No hay empleados en ese departamento.\n");
}
void ps84(void)
{
    Empleado emp[100]; int n = 0, op, num; char depto[25];
    Empleado e[] = {
        {10, "Sofia Munoz",  "Ventas",     dom("Av. Providencia 100","7500000","Providencia","Santiago","224445566"), 850000},
        {20, "Diego Fuentes","Sistemas",   dom("Calle Huerfanos 55","8320000","Centro","Santiago","223332211"), 1200000},
        {30, "Camila Rojas", "Ventas",     dom("Av. Apoquindo 3000","7550000","Las Condes","Santiago","227778899"), 920000}};
    if (usar_ejemplo()) { n = 3; memcpy(emp, e, sizeof e); }
    do {
        printf("\n  1) Altas  2) Bajas  3) Listado por departamento  0) Volver\n  Opcion: ");
        op = leer_int();
        if (op == 1) { num = pedir_int("  Numero de empleado: "); alta_emp(emp, &n, num); }
        else if (op == 2) { num = pedir_int("  Numero de empleado: "); baja_emp(emp, &n, num); }
        else if (op == 3) { pedir("  Departamento: ", depto, 25); listado_emp(emp, n, depto); }
    } while (op != 0);
}

/* =====================================================================
   PS8.5 Tienda de articulos electronicos
   ===================================================================== */
typedef struct { int clave; char nombre[40]; int existencia; } Producto;

void ps85(void)
{
    Producto tienda[100]; int n = 0, i, cla, can, k; char linea[100], ope;
    if (usar_ejemplo()) {
        Producto e[] = {{101, "Resistencia 1k (bolsa)", 50}, {102, "Capacitor 100uF", 30},
                        {103, "Arduino UNO", 8}, {104, "Protoboard 830", 15}, {105, "Sensor ultrasonico", 12}};
        n = 5; memcpy(tienda, e, sizeof e);
    } else {
        n = pedir_int("  Numero de productos (1 a 100): ");
        if (n < 1 || n > 100) { printf("  N fuera de rango.\n"); return; }
        for (i = 0; i < n; i++) {
            tienda[i].clave = pedir_int("  Clave: ");
            pedir("  Nombre: ", tienda[i].nombre, 40);
            tienda[i].existencia = pedir_int("  Existencia: ");
        }
    }
    printf("\n  Transacciones: OPE CLAVE CANTIDAD  (C compra, V venta). Termina con: 0 0 0\n");
    for (;;) {
        printf("  > "); leer_linea(linea, 100);
        if (sscanf(linea, " %c %d %d", &ope, &cla, &can) != 3) { printf("  Formato no valido.\n"); continue; }
        if (ope == '0') break;
        ope = (char)toupper((unsigned char)ope);
        for (k = -1, i = 0; i < n; i++) if (tienda[i].clave == cla) { k = i; break; }
        if (k < 0) { printf("  Producto inexistente.\n"); continue; }
        if (can <= 0) { printf("  Cantidad no valida.\n"); continue; }
        if (ope == 'C') { tienda[k].existencia += can; printf("  Compra: %s ahora tiene %d.\n", tienda[k].nombre, tienda[k].existencia); }
        else if (ope == 'V') {
            if (can > tienda[k].existencia) printf("  Existencia insuficiente (%d).\n", tienda[k].existencia);
            else { tienda[k].existencia -= can; printf("  Venta: %s ahora tiene %d.\n", tienda[k].nombre, tienda[k].existencia); }
        } else printf("  Operacion no valida (usa C o V).\n");
    }
    printf("\n  %-6s %-26s %10s\n", "Clave", "Producto", "Existencia");
    for (i = 0; i < n; i++) printf("  %-6d %-26s %10d\n", tienda[i].clave, tienda[i].nombre, tienda[i].existencia);
}

/* =====================================================================
   PS8.6 Escuela privada (estructuras anidadas)
   ===================================================================== */
typedef struct { char calle[40]; int cp; char colonia[30]; char ciudad[30]; char telefono[20]; } DomAlumno;
typedef struct { char nivel[15]; int grado; char salon[5]; } NivelEstudios;
typedef struct { char materia[25]; double promedio; } Calificacion;
typedef struct {
    int matricula; char nombre[40]; DomAlumno dom; NivelEstudios niv; Calificacion cal[7];
} Alumno;

static const char *MATERIAS[7] = {"Matemáticas", "Español", "Historia", "Ciencias", "Inglés", "Arte", "Educación física"};

static double promedio_general(const Alumno *a)
{
    double s = 0; int i;
    for (i = 0; i < 7; i++) s += a->cal[i].promedio;
    return s / 7.0;
}
static void cargar_ejemplo_escuela(Alumno *e, int n)
{
    const char *nom[] = {"Ana Lopez","Luis Ramos","Sofia Cruz","Jorge Diaz","Marta Ruiz","Pablo Nunez",
                         "Elena Soto","Raul Mena","Irene Vela","Hugo Pineda","Lola Reyes","Omar Tapia"};
    const char *niv[] = {"primaria", "secundaria", "preparatoria"};
    int i, j;
    for (i = 0; i < n; i++) {
        memset(&e[i], 0, sizeof(Alumno));
        e[i].matricula = 1001 + i;
        strcpy(e[i].nombre, nom[i % 12]);
        strcpy(e[i].dom.calle, "Calle Reforma 10"); e[i].dom.cp = 6600 + i;
        strcpy(e[i].dom.colonia, "Juarez"); strcpy(e[i].dom.ciudad, "Ciudad de México"); strcpy(e[i].dom.telefono, "5555550000");
        strcpy(e[i].niv.nivel, niv[i / 4]); e[i].niv.grado = 1 + (i % 2); strcpy(e[i].niv.salon, (i % 4 < 2) ? "A" : "B");
        for (j = 0; j < 7; j++) {
            strcpy(e[i].cal[j].materia, MATERIAS[j]);
            e[i].cal[j].promedio = 6.0 + ((i * 7 + j * 3) % 40) / 10.0;
        }
    }
}
void ps86(void)
{
    Alumno *esc; int n, i, j, op, mat, grado, hay, mejor; char nivel[15], salon[5];
    if (usar_ejemplo()) { n = 12; esc = malloc(n * sizeof(Alumno)); if (!esc) return; cargar_ejemplo_escuela(esc, n); }
    else {
        n = pedir_int("  Numero de alumnos (1 a 1000): ");
        if (n < 1 || n > 1000) { printf("  N fuera de rango.\n"); return; }
        esc = malloc(n * sizeof(Alumno)); if (!esc) { printf("  Sin memoria.\n"); return; }
        for (i = 0; i < n; i++) {
            printf("  Alumno %d\n", i + 1);
            esc[i].matricula = pedir_int("    Matricula: ");
            pedir("    Nombre y apellido: ", esc[i].nombre, 40);
            pedir("    Calle y numero: ", esc[i].dom.calle, 40);
            esc[i].dom.cp = pedir_int("    Codigo postal: ");
            pedir("    Colonia: ", esc[i].dom.colonia, 30);
            pedir("    Ciudad: ", esc[i].dom.ciudad, 30);
            pedir("    Telefono: ", esc[i].dom.telefono, 20);
            pedir("    Nivel (primaria/secundaria/preparatoria): ", esc[i].niv.nivel, 15);
            esc[i].niv.grado = pedir_int("    Grado: ");
            pedir("    Salon: ", esc[i].niv.salon, 5);
            for (j = 0; j < 7; j++) {
                strcpy(esc[i].cal[j].materia, MATERIAS[j]);
                printf("    Promedio de %s: ", MATERIAS[j]); esc[i].cal[j].promedio = leer_double();
            }
        }
    }
    do {
        printf("\n  a) 1 Promedio por matricula   2 Listado por nivel/grado/salon   3 Mejor promedio del grupo   0 Volver\n  Opcion: ");
        op = leer_int();
        if (op == 1) {
            mat = pedir_int("  Matricula: ");
            for (hay = 0, i = 0; i < n; i++)
                if (esc[i].matricula == mat) { printf("  %s: promedio general %.2f\n", esc[i].nombre, promedio_general(&esc[i])); hay = 1; }
            if (!hay) printf("  Matricula no encontrada.\n");
        } else if (op == 2 || op == 3) {
            pedir("  Nivel de estudios: ", nivel, 15); grado = pedir_int("  Grado: "); pedir("  Salon: ", salon, 5);
            hay = 0; mejor = -1;
            for (i = 0; i < n; i++)
                if (mismo_nombre(esc[i].niv.nivel, nivel) && esc[i].niv.grado == grado && mismo_nombre(esc[i].niv.salon, salon)) {
                    if (op == 2) printf("  %-6d %-16s %5.2f\n", esc[i].matricula, esc[i].nombre, promedio_general(&esc[i]));
                    if (mejor < 0 || promedio_general(&esc[i]) > promedio_general(&esc[mejor])) mejor = i;
                    hay = 1;
                }
            if (!hay) printf("  No hay alumnos en ese grupo.\n");
            else if (op == 3) printf("  Mejor promedio: %d  %s  %.2f\n", esc[mejor].matricula, esc[mejor].nombre, promedio_general(&esc[mejor]));
        }
    } while (op != 0);
    free(esc);
}

/* ---------------- menu ---------------- */
int main(void)
{
    int op;
    do {
        printf("\n=== Capitulo 8: Estructuras y uniones ===\n");
        printf(" 1) PS8.1 Banco            2) PS8.2 Tabla de futbol\n");
        printf(" 3) PS8.3 Profesores       4) PS8.4 Empleados (altas/bajas)\n");
        printf(" 5) PS8.5 Tienda           6) PS8.6 Escuela\n");
        printf(" 0) Salir\nOpcion: ");
        op = leer_int();
        switch (op) {
            case 1: ps81(); break; case 2: ps82(); break; case 3: ps83(); break;
            case 4: ps84(); break; case 5: ps85(); break; case 6: ps86(); break;
        }
    } while (op != 0);
    return 0;
}
