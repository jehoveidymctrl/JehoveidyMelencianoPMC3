/* Capitulo 9 - Archivos de datos. Problemas suplementarios PS9.1 a PS9.10.
   Compilar: gcc cap9_ejercicios.c -o cap9
   Archivos de prueba: gcc generar_datos.c -o generar_datos && ./generar_datos */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXL 512

/* ---------------- utilidades de entrada ---------------- */
static void quitar_salto(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++)
        if (s[i] == '\n' || s[i] == '\r') { s[i] = '\0'; break; }
}
static void leer_linea(char *dest, int n)
{
    if (!fgets(dest, n, stdin)) dest[0] = '\0';
    quitar_salto(dest);
}
static int leer_int(void)       { char b[64]; leer_linea(b, 64); return atoi(b); }
static double leer_double(void) { char b[64]; leer_linea(b, 64); return atof(b); }
static void pedir_archivo(const char *def, char *dest)
{
    char b[128];
    printf("Archivo [%s]: ", def);
    leer_linea(b, 128);
    strcpy(dest, b[0] ? b : def);
}
static FILE *abrir(const char *nombre, const char *modo)
{
    FILE *f = fopen(nombre, modo);
    if (!f) printf("  No se pudo abrir '%s'.\n", nombre);
    return f;
}

/* ---------------- PS9.1: cadena mas larga sin strlen ---------------- */
int mi_strlen(const char *s)
{
    int n = 0;
    while (s[n] != '\0') n++;
    return n;
}
void ps91(void)
{
    char nom[128], linea[MAXL], mayor[MAXL] = "";
    int lon, max = 0;
    FILE *f;
    pedir_archivo("arc1.txt", nom);
    if (!(f = abrir(nom, "r"))) return;
    while (fgets(linea, MAXL, f)) {
        quitar_salto(linea);
        lon = mi_strlen(linea);
        if (lon > max) { max = lon; strcpy(mayor, linea); }
    }
    fclose(f);
    printf("  La cadena mas larga mide %d caracteres:\n  \"%s\"\n", max, mayor);
}

/* ---------------- PS9.2: contar vocales (con acentos UTF-8) ---------------- */
void ps92(void)
{
    char nom[128];
    int cont[5] = {0, 0, 0, 0, 0}, c, previo = 0, k;
    const char *vocales = "aeiou";
    FILE *f;
    pedir_archivo("arc.txt", nom);
    if (!(f = abrir(nom, "r"))) return;
    while ((c = fgetc(f)) != EOF) {
        if (previo == 0xC3) {                 /* segunda parte de una letra acentuada */
            switch (c) {
                case 0xA1: case 0x81: cont[0]++; break;   /* a A */
                case 0xA9: case 0x89: cont[1]++; break;   /* e E */
                case 0xAD: case 0x8D: cont[2]++; break;   /* i I */
                case 0xB3: case 0x93: cont[3]++; break;   /* o O */
                case 0xBA: case 0x9A: case 0xBC: cont[4]++; break; /* u U ü */
            }
        } else {
            c = tolower(c);
            for (k = 0; k < 5; k++)
                if (c == vocales[k]) cont[k]++;
        }
        previo = c;
    }
    fclose(f);
    for (k = 0; k < 5; k++) printf("  %c: %d\n", vocales[k], cont[k]);
}

/* ---------------- PS9.3: contar palabras ---------------- */
void ps93(void)
{
    char nom[128];
    int c, enpalabra = 0, palabras = 0;
    FILE *f;
    pedir_archivo("palabras.txt", nom);
    if (!(f = abrir(nom, "r"))) return;
    while ((c = fgetc(f)) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') enpalabra = 0;
        else if (!enpalabra) { enpalabra = 1; palabras++; }
    }
    fclose(f);
    printf("  Hay %d palabras.\n", palabras);
}

/* ---------------- PS9.4: veces que aparece una cadena ---------------- */
int contar_sub(const char *linea, const char *cad)
{
    int n = 0, i, j;
    for (i = 0; linea[i] != '\0'; i++) {
        for (j = 0; cad[j] != '\0' && linea[i + j] == cad[j]; j++) ;
        if (j > 0 && cad[j] == '\0') n++;      /* cuenta tambien las superpuestas */
    }
    return n;
}
void ps94(void)
{
    char nom[128], cad[50], linea[MAXL];
    int total = 0;
    FILE *f;
    printf("  Cadena a buscar (max 49 caracteres): ");
    leer_linea(cad, 50);
    pedir_archivo("arc2.txt", nom);
    if (!(f = abrir(nom, "r"))) return;
    while (fgets(linea, MAXL, f)) { quitar_salto(linea); total += contar_sub(linea, cad); }
    fclose(f);
    printf("  \"%s\" aparece %d veces.\n", cad, total);
}

/* ---------------- PS9.5: intercambiar primera con ultima, etc. ---------------- */
void ps95(void)
{
    char nom[128], linea[MAXL], **v = NULL, *tmp, **nuevo;
    int n = 0, cap = 0, i;
    FILE *f;
    pedir_archivo("arc.txt", nom);
    if (!(f = abrir(nom, "r"))) return;
    while (fgets(linea, MAXL, f)) {
        quitar_salto(linea);
        if (n == cap) {
            cap = cap ? cap * 2 : 8;
            nuevo = realloc(v, cap * sizeof(char *));
            if (!nuevo) { printf("  Sin memoria.\n"); fclose(f); return; }
            v = nuevo;
        }
        v[n] = malloc(strlen(linea) + 1);
        strcpy(v[n++], linea);
    }
    fclose(f);
    for (i = 0; i < n / 2; i++) { tmp = v[i]; v[i] = v[n - 1 - i]; v[n - 1 - i] = tmp; }
    if ((f = abrir("arc_intercambiado.txt", "w"))) {
        for (i = 0; i < n; i++) fprintf(f, "%s\n", v[i]);
        fclose(f);
        printf("  %d cadenas intercambiadas -> arc_intercambiado.txt\n", n);
    }
    for (i = 0; i < n; i++) free(v[i]);
    free(v);
}

/* ---------------- PS9.6: ortografia (mb, mp, nv) ---------------- */
void ps96(void)
{
    char nom[128], linea[MAXL];
    int ln = 0, errores = 0, i, a, b;
    const char *regla;
    FILE *f;
    pedir_archivo("doc.dat", nom);
    if (!(f = abrir(nom, "r"))) return;
    while (fgets(linea, MAXL, f)) {
        quitar_salto(linea); ln++;
        for (i = 0; linea[i] != '\0'; i++) {
            int c = tolower((unsigned char)linea[i]);
            int s = tolower((unsigned char)linea[i + 1]);
            regla = NULL;
            if (c == 'n' && (s == 'b' || s == 'p')) regla = "antes de b/p va m, no n";
            else if (c == 'm' && s == 'v')          regla = "antes de v va n, no m";
            if (regla) {
                a = b = i;
                while (a > 0 && linea[a - 1] != ' ') a--;
                while (linea[b] != '\0' && linea[b] != ' ') b++;
                printf("  Linea %d: \"%.*s\" -> %s\n", ln, b - a, linea + a, regla);
                errores++;
            }
        }
    }
    fclose(f);
    printf("  Errores encontrados: %d\n", errores);
}

/* ---------------- PS9.7: quitar registros repetidos ---------------- */
typedef struct { int matricula; char nombre[40]; int carrera; float promedio; } Alumno;

void ps97(void)
{
    char nom[128];
    Alumno a, ant;
    int primero = 1, leidos = 0, escritos = 0;
    FILE *f, *g;
    pedir_archivo("ad5.dat", nom);
    if (!(f = abrir(nom, "rb"))) return;
    if (!(g = abrir("ad5_sin_repetidos.dat", "wb"))) { fclose(f); return; }
    while (fread(&a, sizeof(Alumno), 1, f) == 1) {
        leidos++;
        /* el archivo esta ordenado, los repetidos son consecutivos */
        if (primero || a.matricula != ant.matricula) {
            fwrite(&a, sizeof(Alumno), 1, g);
            escritos++;
        }
        ant = a; primero = 0;
    }
    fclose(f); fclose(g);
    printf("  Leidos: %d, escritos: %d -> ad5_sin_repetidos.dat\n", leidos, escritos);
}

/* ---------------- PS9.8: ordenar archivo por matricula ---------------- */
typedef struct { int mat; float c[3]; } Calif;

int comparar_mat(const void *x, const void *y)
{
    return ((const Calif *)x)->mat - ((const Calif *)y)->mat;
}
void ps98(void)
{
    char nom[128];
    Calif *v = NULL, *nuevo, r;
    int n = 0, cap = 0, i;
    FILE *f;
    pedir_archivo("arc.dat", nom);
    if (!(f = abrir(nom, "r"))) return;
    while (fscanf(f, "%d %f %f %f", &r.mat, &r.c[0], &r.c[1], &r.c[2]) == 4) {
        if (n == cap) {
            cap = cap ? cap * 2 : 8;
            nuevo = realloc(v, cap * sizeof(Calif));
            if (!nuevo) { printf("  Sin memoria.\n"); free(v); fclose(f); return; }
            v = nuevo;
        }
        v[n++] = r;
    }
    fclose(f);
    qsort(v, n, sizeof(Calif), comparar_mat);
    if ((f = abrir("arc1.dat", "w"))) {
        for (i = 0; i < n; i++)
            fprintf(f, "%d\t%.1f\t%.1f\t%.1f\n", v[i].mat, v[i].c[0], v[i].c[1], v[i].c[2]);
        fclose(f);
        printf("  %d registros ordenados -> arc1.dat\n", n);
    }
    free(v);
}

/* ---------------- PS9.9: comercializadora de pinturas ---------------- */
typedef struct { int clave; char nombre[40]; int existencia; float precio; } Producto;
#define ARCH_COM "com.dat"

static long num_regs(FILE *f)
{
    fseek(f, 0, SEEK_END);
    return ftell(f) / (long)sizeof(Producto);
}
/* busqueda binaria en el archivo (esta ordenado por clave); devuelve posicion o -1 */
static long buscar_prod(FILE *f, int clave, Producto *p)
{
    long lo = 0, hi = num_regs(f) - 1, m;
    while (lo <= hi) {
        m = (lo + hi) / 2;
        fseek(f, m * (long)sizeof(Producto), SEEK_SET);
        fread(p, sizeof(Producto), 1, f);
        if (p->clave == clave) return m;
        if (p->clave < clave) lo = m + 1; else hi = m - 1;
    }
    return -1;
}
static void guardar_prod(FILE *f, long pos, const Producto *p)
{
    fseek(f, pos * (long)sizeof(Producto), SEEK_SET);
    fwrite(p, sizeof(Producto), 1, f);
    fflush(f);
}
void modulo_ventas(void)
{
    FILE *f = abrir(ARCH_COM, "r+b");
    Producto p; long pos; int clave, cant; double total = 0;
    if (!f) return;
    printf("  --- VENTA (clave 0 = fin) ---\n");
    printf("  Clave: "); clave = leer_int();
    while (clave != 0) {
        pos = buscar_prod(f, clave, &p);
        if (pos < 0) printf("  Producto inexistente.\n");
        else {
            printf("  %s | existencia %d | $%.2f\n  Cantidad: ", p.nombre, p.existencia, p.precio);
            cant = leer_int();
            if (cant <= 0 || cant > p.existencia) printf("  Cantidad no valida o insuficiente.\n");
            else {
                p.existencia -= cant;
                guardar_prod(f, pos, &p);
                total += cant * p.precio;
            }
        }
        printf("  Clave: "); clave = leer_int();
    }
    printf("  TOTAL DE LA VENTA: $%.2f\n", total);
    fclose(f);
}
void modulo_reabasto(void)
{
    FILE *f = abrir(ARCH_COM, "r+b");
    Producto p; long pos; int clave, cant;
    if (!f) return;
    printf("  --- REABASTECIMIENTO (clave 0 = fin) ---\n  Clave: "); clave = leer_int();
    while (clave != 0) {
        pos = buscar_prod(f, clave, &p);
        if (pos < 0) printf("  Producto inexistente.\n");
        else {
            printf("  %s | existencia %d\n  Cantidad a ingresar: ", p.nombre, p.existencia);
            cant = leer_int();
            if (cant > 0) { p.existencia += cant; guardar_prod(f, pos, &p); }
        }
        printf("  Clave: "); clave = leer_int();
    }
    fclose(f);
}
/* inserta un producto en su posicion generando un archivo nuevo */
static int insertar_prod(const Producto *nuevo)
{
    FILE *src = fopen(ARCH_COM, "rb"), *dst = fopen("com.tmp", "wb");
    Producto p; int puesto = 0;
    if (!dst) { if (src) fclose(src); return 0; }
    if (src) {
        while (fread(&p, sizeof(Producto), 1, src) == 1) {
            if (!puesto && nuevo->clave < p.clave) { fwrite(nuevo, sizeof(Producto), 1, dst); puesto = 1; }
            fwrite(&p, sizeof(Producto), 1, dst);
        }
        fclose(src);
    }
    if (!puesto) fwrite(nuevo, sizeof(Producto), 1, dst);
    fclose(dst);
    remove(ARCH_COM);
    return rename("com.tmp", ARCH_COM) == 0;
}
void modulo_nuevos(void)
{
    FILE *f; Producto p; int clave;
    printf("  --- NUEVOS PRODUCTOS (clave 0 = fin) ---\n  Clave: "); clave = leer_int();
    while (clave != 0) {
        f = fopen(ARCH_COM, "rb");
        if (f && buscar_prod(f, clave, &p) >= 0) printf("  Esa clave ya existe.\n");
        else {
            p.clave = clave;
            printf("  Nombre: ");      leer_linea(p.nombre, 40);
            printf("  Existencia: ");  p.existencia = leer_int();
            printf("  Precio: ");      p.precio = (float)leer_double();
            if (insertar_prod(&p)) printf("  Producto agregado.\n");
        }
        if (f) fclose(f);
        printf("  Clave: "); clave = leer_int();
    }
}
void modulo_inventario(void)
{
    FILE *f = abrir(ARCH_COM, "rb");
    Producto p;
    if (!f) return;
    printf("  %-6s %-30s %10s %10s\n", "Clave", "Producto", "Existencia", "Precio");
    while (fread(&p, sizeof(Producto), 1, f) == 1)
        printf("  %-6d %-30s %10d %10.2f\n", p.clave, p.nombre, p.existencia, p.precio);
    fclose(f);
}
void ps99(void)
{
    int op;
    do {
        printf("\n  a) 1 Ventas  2 Reabastecimiento  3 Nuevos productos  4 Inventario  0 Volver\n  Opcion: ");
        op = leer_int();
        switch (op) {
            case 1: modulo_ventas(); break;
            case 2: modulo_reabasto(); break;
            case 3: modulo_nuevos(); break;
            case 4: modulo_inventario(); break;
        }
    } while (op != 0);
}

/* ---------------- PS9.10: pacientes del hospital ---------------- */
typedef struct { char calle[40]; int numero; char colonia[40]; char cp[10]; char ciudad[40]; char telefono[20]; } Domicilio;
typedef struct { int clave; char nombre[50]; int edad; char sexo; int condicion; Domicilio dom; } Paciente;

void ps910(void)
{
    char nom[128];
    Paciente p;
    int hombres = 0, mujeres = 0, cond[6] = {0}, edades[11] = {0}, total = 0, i, k;
    FILE *f;
    pedir_archivo("pacientes.dat", nom);
    if (!(f = abrir(nom, "rb"))) return;
    while (fread(&p, sizeof(Paciente), 1, f) == 1) {
        total++;
        k = toupper((unsigned char)p.sexo);
        if (k == 'M' || k == 'H') hombres++;       /* M o H = masculino */
        else if (k == 'F')        mujeres++;
        if (p.condicion >= 1 && p.condicion <= 5) cond[p.condicion]++;
        k = p.edad / 10; if (k > 10) k = 10;        /* 100 o mas = ultima categoria */
        if (k >= 0) edades[k]++;
    }
    fclose(f);
    if (total == 0) { printf("  El archivo esta vacio.\n"); return; }
    printf("  Pacientes: %d\n", total);
    printf("  a) Hombres: %.1f%%   Mujeres: %.1f%%\n", 100.0 * hombres / total, 100.0 * mujeres / total);
    printf("  b) Por condicion:\n");
    for (i = 1; i <= 5; i++) printf("     Condicion %d: %d\n", i, cond[i]);
    printf("  c) Por edad:\n");
    for (i = 0; i < 10; i++) printf("     %d-%d: %d\n", i * 10, i * 10 + 9, edades[i]);
    printf("     >=100: %d\n", edades[10]);
}

/* ---------------- Menu ---------------- */
int main(void)
{
    int op;
    do {
        printf("\n=== Capitulo 9: Archivos de datos ===\n");
        printf(" 1) PS9.1 cadena mas larga      2) PS9.2 vocales\n");
        printf(" 3) PS9.3 palabras              4) PS9.4 buscar cadena\n");
        printf(" 5) PS9.5 intercambiar cadenas  6) PS9.6 ortografia\n");
        printf(" 7) PS9.7 quitar repetidos      8) PS9.8 ordenar por matricula\n");
        printf(" 9) PS9.9 comercializadora     10) PS9.10 hospital\n");
        printf(" 0) Salir\nOpcion: ");
        op = leer_int();
        switch (op) {
            case 1: ps91(); break;  case 2: ps92(); break;
            case 3: ps93(); break;  case 4: ps94(); break;
            case 5: ps95(); break;  case 6: ps96(); break;
            case 7: ps97(); break;  case 8: ps98(); break;
            case 9: ps99(); break;  case 10: ps910(); break;
        }
    } while (op != 0);
    return 0;
}
