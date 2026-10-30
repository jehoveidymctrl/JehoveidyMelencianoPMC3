/* Lee un archivo de texto con memoria dinamica usando malloc() y realloc().
   Compilar: gcc leer_archivo_dinamico.c -o leer
   Uso:      ./leer archivo.txt     (o sin argumentos para que lo pida)
   - El contenido completo crece caracter a caracter en un buffer (realloc duplica la capacidad).
   - Luego se separa en lineas: un arreglo de apuntadores tambien crece con realloc
     y cada linea se guarda con malloc del tamano justo. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Devuelve el contenido del archivo en memoria dinamica (liberar con free). */
char *leer_contenido(const char *nombre, size_t *longitud)
{
    FILE *f = fopen(nombre, "r");
    char *buf, *tmp;
    size_t cap = 16, n = 0;
    int c;

    if (!f) return NULL;
    buf = malloc(cap);
    if (!buf) { fclose(f); return NULL; }

    while ((c = fgetc(f)) != EOF) {
        if (n + 1 >= cap) {                       /* +1: espacio para '\0' */
            cap *= 2;
            tmp = realloc(buf, cap);
            if (!tmp) { free(buf); fclose(f); return NULL; }
            buf = tmp;
        }
        buf[n++] = (char)c;
    }
    buf[n] = '\0';
    fclose(f);

    tmp = realloc(buf, n + 1);                    /* ajusta al tamano exacto */
    if (tmp) buf = tmp;
    *longitud = n;
    return buf;
}

/* Divide el texto en lineas; cada una con su propio malloc. */
char **separar_lineas(const char *texto, int *num)
{
    char **lineas = NULL, **tmp;
    int cap = 0;
    const char *ini = texto, *fin;
    size_t len;

    *num = 0;
    while (*ini != '\0') {
        fin = strchr(ini, '\n');
        len = fin ? (size_t)(fin - ini) : strlen(ini);

        if (*num == cap) {
            cap = cap ? cap * 2 : 4;
            tmp = realloc(lineas, cap * sizeof(char *));
            if (!tmp) return lineas;
            lineas = tmp;
        }
        lineas[*num] = malloc(len + 1);
        if (!lineas[*num]) return lineas;
        memcpy(lineas[*num], ini, len);
        lineas[*num][len] = '\0';
        (*num)++;

        if (!fin) break;
        ini = fin + 1;
    }
    return lineas;
}

int main(int argc, char *argv[])
{
    char nombre[256], *texto, **lineas;
    size_t longitud;
    int num, i;

    if (argc > 1) strncpy(nombre, argv[1], sizeof nombre - 1), nombre[sizeof nombre - 1] = '\0';
    else {
        printf("Nombre del archivo de texto: ");
        if (scanf("%255s", nombre) != 1) return 1;
    }

    texto = leer_contenido(nombre, &longitud);
    if (!texto) { printf("No se pudo leer '%s'.\n", nombre); return 1; }

    lineas = separar_lineas(texto, &num);

    printf("\n--- %s: %zu caracteres, %d lineas ---\n", nombre, longitud, num);
    for (i = 0; i < num; i++)
        printf("%3d: %s\n", i + 1, lineas[i]);

    for (i = 0; i < num; i++) free(lineas[i]);
    free(lineas);
    free(texto);
    return 0;
}
