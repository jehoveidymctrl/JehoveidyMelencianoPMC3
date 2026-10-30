/* Capitulo 7 - Caracteres y cadenas de caracteres. Problemas PS7.1 a PS7.14.
   Compilar: gcc cap7_ejercicios.c -o cap7
   Las cadenas se manejan en UTF-8 (acentos y ñ cuentan como un solo caracter). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* ---------------- utilidades ---------------- */
static void quitar_salto(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++)
        if (s[i] == '\n' || s[i] == '\r') { s[i] = '\0'; break; }
}
/* lee una linea y la recorta a n-1 bytes */
static void leer_linea(char *dest, int n)
{
    char tmp[1024];
    if (!fgets(tmp, sizeof tmp, stdin)) { dest[0] = '\0'; return; }
    quitar_salto(tmp);
    strncpy(dest, tmp, n - 1);
    dest[n - 1] = '\0';
}
static int leer_int(void) { char b[64]; leer_linea(b, 64); return atoi(b); }

/* bytes que ocupa el caracter UTF-8 que comienza en c */
static int bytes_car(unsigned char c)
{
    if (c < 0x80) return 1;
    if ((c >> 5) == 6) return 2;
    if ((c >> 4) == 14) return 3;
    if ((c >> 3) == 30) return 4;
    return 1;
}
static int largo_car(const char *s)          /* longitud en caracteres */
{
    int n = 0, i = 0;
    while (s[i] != '\0') { i += bytes_car((unsigned char)s[i]); n++; }
    return n;
}
static int es_vocal_utf8(const char *s, int *saltar)
{
    unsigned char c = (unsigned char)s[0], d = (unsigned char)s[1];
    *saltar = 1;
    if (c < 0x80) return strchr("aeiouAEIOU", c) != NULL && c != 0;
    if (c == 0xC3) {
        *saltar = 2;
        switch (d) {
            case 0xA1: case 0xA9: case 0xAD: case 0xB3: case 0xBA: case 0xBC:
            case 0x81: case 0x89: case 0x8D: case 0x93: case 0x9A: case 0x9C: return 1;
        }
    }
    return 0;
}
static int es_mayuscula_utf8(const char *s, int *saltar)
{
    unsigned char c = (unsigned char)s[0], d = (unsigned char)s[1];
    *saltar = 1;
    if (c < 0x80) return isupper(c);
    if (c == 0xC3) { *saltar = 2; return d >= 0x80 && d <= 0x9E && d != 0x97; }
    return 0;
}
static int dividir(char *s, char *pal[], int max)   /* destruye s */
{
    int n = 0;
    char *t = strtok(s, " \t");
    while (t && n < max) { pal[n++] = t; t = strtok(NULL, " \t"); }
    return n;
}

/* ---------------- PS7.1 ---------------- */
void ps71(void)
{
    char cad[51]; int i = 0, pos = 1, b;
    printf("Cadena (max 50): "); leer_linea(cad, 51);
    printf("Caracteres en posiciones impares (1, 3, 5...): ");
    while (cad[i] != '\0') {
        b = bytes_car((unsigned char)cad[i]);
        if (pos % 2 == 1) fwrite(cad + i, 1, b, stdout);
        i += b; pos++;
    }
    printf("\n");
}
/* ---------------- PS7.2 ---------------- */
void ps72(void)
{
    char cad[51]; int i = 0, pos = 1, b, hay = 0;
    printf("Cadena (max 50): "); leer_linea(cad, 51);
    printf("Digitos en posiciones pares (2, 4, 6...): ");
    while (cad[i] != '\0') {
        b = bytes_car((unsigned char)cad[i]);
        if (pos % 2 == 0 && isdigit((unsigned char)cad[i])) { putchar(cad[i]); hay = 1; }
        i += b; pos++;
    }
    if (!hay) printf("(ninguno)");
    printf("\n");
}
/* ---------------- PS7.3 ---------------- */
void ps73(void)
{
    char cad[128]; int i = 0, n = 0;
    printf("Cadena (max 30 caracteres): "); leer_linea(cad, 128);
    while (cad[i] != '\0' && n < 30) { i += bytes_car((unsigned char)cad[i]); n++; }
    cad[i] = '\0';                                 /* recorta a 30 caracteres */
    for (; n < 30; n++) cad[i++] = '-';
    cad[i] = '\0';
    printf("Cadena completada: %s\n", cad);
}
/* ---------------- PS7.4 ---------------- */
void ps74(void)
{
    const char *mes[] = {"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio",
                         "Agosto","Septiembre","Octubre","Noviembre","Diciembre"};
    char cad[30], nueva[60]; int d, m, a;
    printf("Fecha (dd/mm/aa): "); leer_linea(cad, 30);
    if (sscanf(cad, "%d/%d/%d", &d, &m, &a) != 3 || d < 1 || d > 31 || m < 1 || m > 12 || a < 0) {
        printf("Fecha no valida.\n"); return;
    }
    a += (a < 50) ? 2000 : 1900;                 /* 00-49 -> 20xx, 50-99 -> 19xx */
    sprintf(nueva, "%02d de %s de %d", d, mes[m - 1], a);
    printf("Nueva cadena: %s\n", nueva);
}
/* ---------------- PS7.5 ---------------- */
void ps75(void)
{
    char cad[128]; int i, inicio = 1;
    printf("Cadena (max 50): "); leer_linea(cad, 128);
    for (i = 0; cad[i] != '\0'; i++) {
        unsigned char c = (unsigned char)cad[i];
        if (c == ' ') { inicio = 1; continue; }
        if (inicio) {
            if (c < 0x80) cad[i] = (char)toupper(c);
            else if (c == 0xC3 && (unsigned char)cad[i + 1] >= 0xA0 &&
                     (unsigned char)cad[i + 1] <= 0xBE && (unsigned char)cad[i + 1] != 0xB7)
                cad[i + 1] = (char)((unsigned char)cad[i + 1] - 0x20);   /* á->Á, ñ->Ñ... */
            inicio = 0;
        }
    }
    printf("Resultado: %s\n", cad);
}
/* ---------------- PS7.6 ---------------- */
void ps76(void)
{
    char cad[21], mayor[21] = "", resp[16]; int max = -1, n;
    printf("¿Desea ingresar una cadena? (S/N): "); leer_linea(resp, 16);
    while (toupper((unsigned char)resp[0]) == 'S') {
        printf("Cadena (max 20 caracteres): "); leer_linea(cad, 21);
        n = largo_car(cad);
        if (n > max) { max = n; strcpy(mayor, cad); }
        printf("¿Desea ingresar otra cadena? (S/N): "); leer_linea(resp, 16);
    }
    if (max < 0) printf("No se ingreso ninguna cadena.\n");
    else printf("La cadena mas larga es \"%s\" (%d caracteres).\n", mayor, max);
}
/* ---------------- PS7.7 ---------------- */
void ps77(void)
{
    char cad[30]; int i, ok;
    printf("Numero telefonico (10 digitos): "); leer_linea(cad, 30);
    ok = (strlen(cad) == 10);
    for (i = 0; ok && i < 10; i++) if (!isdigit((unsigned char)cad[i])) ok = 0;
    if (!ok) { printf("Debe tener exactamente 10 digitos.\n"); return; }
    printf("Numero telefonico: %s\n", cad);
    printf("Nueva cadena: (%.2s)-%.1s-%s\n", cad, cad + 2, cad + 3);
}
/* ---------------- PS7.8 ---------------- */
void ps78(void)
{
    char c1[51], c2[51], res[200] = "", *p1[30], *p2[30];
    int n1, n2, i;
    printf("Cadena 1 (max 50): "); leer_linea(c1, 51);
    printf("Cadena 2 (max 50): "); leer_linea(c2, 51);
    n1 = dividir(c1, p1, 30); n2 = dividir(c2, p2, 30);
    for (i = 0; i < n1 || i < n2; i++) {
        if (i < n1) { strcat(res, p1[i]); strcat(res, " "); }
        if (i < n2) { strcat(res, p2[i]); strcat(res, " "); }
    }
    if (res[0]) res[strlen(res) - 1] = '\0';
    printf("Cadena intercalada: %s\n", res);
}
/* ---------------- PS7.9 ---------------- */
void ps79(void)
{
    char cad[51], *p[30]; int n, i;
    printf("Cadena (max 50): "); leer_linea(cad, 51);
    n = dividir(cad, p, 30);
    printf("Cadena invertida: ");
    for (i = n - 1; i >= 0; i--) printf("%s%s", p[i], i > 0 ? " " : "");
    printf("\n");
}
/* ---------------- PS7.10 ---------------- */
void ps710(void)
{
    char hoy[16], cad[51], resp[16]; time_t t = time(NULL); int encontrada = 0;
    strftime(hoy, sizeof hoy, "%d/%m/%Y", localtime(&t));
    printf("Fecha de hoy: %s\n", hoy);
    printf("¿Desea ingresar una cadena? (S/N): "); leer_linea(resp, 16);
    while (toupper((unsigned char)resp[0]) == 'S') {
        printf("Cadena (formato dd/mm/aaaa Texto): "); leer_linea(cad, 51);
        if (strncmp(cad, hoy, 10) == 0) { printf("  >> %s\n", cad); encontrada = 1; }
        printf("¿Desea ingresar otra cadena? (S/N): "); leer_linea(resp, 16);
    }
    if (!encontrada) printf("Ninguna cadena empieza con la fecha de hoy.\n");
}

/* ---------------- arreglos de cadenas: PS7.11 a PS7.14 ---------------- */
/* Carga hasta max cadenas de ancho 'ancho' en un arreglo plano; devuelve cuantas. */
static int cargar(char *base, int ancho, int max, const char *ejemplo[], int nej)
{
    int n, i, op;
    printf("  1) Escribir las cadenas   2) Usar ejemplo\n  Opcion: "); op = leer_int();
    if (op == 2) {
        for (i = 0; i < nej && i < max; i++) { strncpy(base + i * ancho, ejemplo[i], ancho - 1); base[i * ancho + ancho - 1] = '\0'; }
        printf("  Se cargaron %d cadenas de ejemplo.\n", i);
        return i;
    }
    printf("  ¿Cuantas cadenas (1 a %d)? ", max); n = leer_int();
    if (n < 1 || n > max) n = max;
    for (i = 0; i < n; i++) { printf("  Cadena %d: ", i + 1); leer_linea(base + i * ancho, ancho); }
    return n;
}
static const char *EJ_A[] = {
    "Mexico es la novena economia del mundo",
    "La programacion en C es Muy Util",
    "AEIOU y mas vocales aqui",
    "Estructuras de Datos Edicion Dos",
    "hola mundo" };

void ps711(void)
{
    char arc[10][50]; int n, i, k, v, mejor = -1, imax = 0, s;
    n = cargar(&arc[0][0], 50, 10, EJ_A, 5);
    for (i = 0; i < n; i++) {
        for (v = 0, k = 0; arc[i][k] != '\0'; k += s) if (es_vocal_utf8(arc[i] + k, &s)) v++;
        if (v > mejor) { mejor = v; imax = i; }
    }
    if (n > 0) printf("Cadena con mas vocales (%d): \"%s\"\n", mejor, arc[imax]);
}
void ps712(void)
{
    char arc[10][50]; int n, i, k, v, mejor = -1, imax = 0, s;
    n = cargar(&arc[0][0], 50, 10, EJ_A, 5);
    for (i = 0; i < n; i++) {
        for (v = 0, k = 0; arc[i][k] != '\0'; k += s) if (es_mayuscula_utf8(arc[i] + k, &s)) v++;
        if (v > mejor) { mejor = v; imax = i; }
    }
    if (n > 0) printf("Cadena con mas mayusculas (%d): \"%s\"\n", mejor, arc[imax]);
}
void ps713(void)
{
    char arc[10][50], copia[50], *p[30]; int n, i;
    n = cargar(&arc[0][0], 50, 10, EJ_A, 5);
    for (i = 0; i < n; i++) {
        strcpy(copia, arc[i]);
        printf("  Cadena %d: %d palabras  (\"%s\")\n", i + 1, dividir(copia, p, 30), arc[i]);
    }
}
static const char *EJ_B[] = {
    "Era inevitable, el olor de las almendras amargas le recordaba siempre el",
    "destino de los amores contrariados. El doctor Juvenal Urbino lo percibió",
    "desde que entró en la casa todavía en penumbras, adonde había acudido de",
    "urgencia a ocuparse de un caso que para él había dejado de ser urgente",
    "desde hacía muchos años." };

void ps714(void)
{
    char arc[20][80]; int n, i, k, len, freq[81] = {0}, mx = 0;
    n = cargar(&arc[0][0], 80, 20, EJ_B, 5);
    for (i = 0; i < n; i++) {
        len = 0;
        for (k = 0; ; k++) {
            unsigned char c = (unsigned char)arc[i][k];
            int separador = (c == '\0' || c == ' ' || c == '\t' || (c < 0x80 && ispunct(c)));
            if (separador) {
                if (len > 0) { freq[len]++; if (len > mx) mx = len; len = 0; }
                if (c == '\0') break;
            } else if ((c & 0xC0) != 0x80) len++;      /* cuenta caracteres, no bytes */
        }
    }
    printf("  Longitud de la palabra    Frecuencia\n");
    for (i = 1; i <= mx; i++) printf("  %12d %20d\n", i, freq[i]);
}

/* ---------------- menu ---------------- */
int main(void)
{
    int op;
    do {
        printf("\n=== Capitulo 7: Cadenas de caracteres ===\n");
        printf(" 1) PS7.1 posiciones impares   2) PS7.2 digitos en pares\n");
        printf(" 3) PS7.3 completar con '-'    4) PS7.4 fecha con mes\n");
        printf(" 5) PS7.5 primera en mayuscula 6) PS7.6 cadena mas larga\n");
        printf(" 7) PS7.7 telefono             8) PS7.8 intercalar palabras\n");
        printf(" 9) PS7.9 invertir palabras   10) PS7.10 fecha de hoy\n");
        printf("11) PS7.11 mas vocales        12) PS7.12 mas mayusculas\n");
        printf("13) PS7.13 palabras por cadena 14) PS7.14 frecuencia por longitud\n");
        printf(" 0) Salir\nOpcion: ");
        op = leer_int();
        switch (op) {
            case 1: ps71(); break;  case 2: ps72(); break;  case 3: ps73(); break;
            case 4: ps74(); break;  case 5: ps75(); break;  case 6: ps76(); break;
            case 7: ps77(); break;  case 8: ps78(); break;  case 9: ps79(); break;
            case 10: ps710(); break; case 11: ps711(); break; case 12: ps712(); break;
            case 13: ps713(); break; case 14: ps714(); break;
        }
    } while (op != 0);
    return 0;
}
