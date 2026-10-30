/* Genera archivos de prueba para cap9_ejercicios.c.  gcc generar_datos.c -o generar_datos */
#include <stdio.h>
#include <string.h>

typedef struct { int matricula; char nombre[40]; int carrera; float promedio; } Alumno;
typedef struct { int clave; char nombre[40]; int existencia; float precio; } Producto;
typedef struct { char calle[40]; int numero; char colonia[40]; char cp[10]; char ciudad[40]; char telefono[20]; } Domicilio;
typedef struct { int clave; char nombre[50]; int edad; char sexo; int condicion; Domicilio dom; } Paciente;

static void texto(const char *nom, const char *contenido)
{
    FILE *f = fopen(nom, "w");
    if (f) { fputs(contenido, f); fclose(f); }
}
int main(void)
{
    FILE *f; int i;
    texto("arc1.txt", "hola\nprogramacion en C\nla cadena mas larga de todas es esta\ncorta\n");
    texto("arc.txt", "México es la novena economía del mundo,\npero tiene más pobres que la mayoría de los países europeos\ncon macroeconomías peores que la de México.\n");
    texto("palabras.txt", "sa sa sa yacusá yacusá\nle mando le mando le mando al maestro\n");
    texto("arc2.txt", "sasaasassassssassas\nssssaaabbbsassbsasbbbasssss\nsssssaaaaaaasssssasssassbbbsbsb\nsssssabsabsbbbbsbabsas\n");
    texto("doc.dat", "El hombre cambia de pais\nTambien siempre enpieza un envase\nEl invierno es imvierno\nUna campana y un tambor\nSe cunple con el inbentario\n");
    texto("arc.dat", "51 8.7 6.7 8.9\n15 7.8 7.6 5.8\n11 7.8 9.0 9.9\n32 8.8 9.9 9.7\n96 8.9 9.1 9.9\n29 9.3 9.6 9.8\n");

    /* ad5.dat: ordenado de mayor a menor, con repetidos */
    f = fopen("ad5.dat", "wb");
    { Alumno a[] = { {900,"Ana Perez",1,9.1f}, {900,"Ana Perez",1,9.1f}, {750,"Luis Gomez",2,8.0f},
                     {620,"Maria Diaz",3,7.5f}, {620,"Maria Diaz",3,7.5f}, {620,"Maria Diaz",3,7.5f},
                     {300,"Jose Ruiz",2,8.8f} };
      fwrite(a, sizeof(Alumno), 7, f); }
    fclose(f);

    /* com.dat: ordenado por clave */
    f = fopen("com.dat", "wb");
    { Producto p[] = { {10,"Pintura vinilica blanca 19L",25,1250.0f}, {20,"Impermeabilizante rojo 19L",12,1890.5f},
                       {30,"Sellador acrilico 4L",40,320.0f}, {50,"Esmalte negro 1L",60,185.0f} };
      fwrite(p, sizeof(Producto), 4, f); }
    fclose(f);

    /* pacientes.dat */
    f = fopen("pacientes.dat", "wb");
    for (i = 1; i <= 12; i++) {
        Paciente p; memset(&p, 0, sizeof p);
        p.clave = i; sprintf(p.nombre, "Paciente %d", i);
        p.edad = (i * 9) % 105; p.sexo = (i % 3 == 0) ? 'F' : (i % 2 ? 'M' : 'F');
        p.condicion = 1 + i % 5;
        strcpy(p.dom.calle, "Av. Amazonas"); p.dom.numero = i; strcpy(p.dom.colonia, "Centro");
        strcpy(p.dom.cp, "170150"); strcpy(p.dom.ciudad, "Quito"); strcpy(p.dom.telefono, "022000000");
        fwrite(&p, sizeof p, 1, f);
    }
    fclose(f);
    printf("Archivos de prueba creados.\n");
    return 0;
}
