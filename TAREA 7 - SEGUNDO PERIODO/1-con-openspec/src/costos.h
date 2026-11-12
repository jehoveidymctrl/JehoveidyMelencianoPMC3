/* costos.h - Nucleo (logica) del programa de gestion de costos de un vehiculo.
   No tiene entrada/salida por pantalla: lo usan la version de consola y la grafica. */
#ifndef COSTOS_H
#define COSTOS_H

#define COSTOS_TXT  40
#define COSTOS_DESC 80

typedef enum {
    CAT_COMBUSTIBLE = 0, CAT_MANTENIMIENTO, CAT_REPARACION,
    CAT_SEGURO, CAT_IMPUESTOS, CAT_OTROS, CAT_CANTIDAD
} Categoria;

typedef enum {
    COSTOS_OK = 0,
    COSTOS_ERR_VEHICULO,      /* marca, modelo o placa vacios            */
    COSTOS_ERR_ANIO,          /* anio fuera de rango                     */
    COSTOS_ERR_ODOMETRO,      /* odometro inicial negativo o incoherente */
    COSTOS_ERR_SIN_VEHICULO,  /* se pidio un gasto sin vehiculo definido */
    COSTOS_ERR_FECHA,
    COSTOS_ERR_CATEGORIA,
    COSTOS_ERR_MONTO,
    COSTOS_ERR_KM,
    COSTOS_ERR_LITROS,
    COSTOS_ERR_NO_ENCONTRADO,
    COSTOS_ERR_SIN_DATOS,     /* no hay datos suficientes para el calculo */
    COSTOS_ERR_ARCHIVO,
    COSTOS_ERR_FORMATO,
    COSTOS_ERR_MEMORIA
} CostosError;

typedef struct {
    char marca[COSTOS_TXT];
    char modelo[COSTOS_TXT];
    char placa[16];
    int  anio;
    long odometro_inicial;
} Vehiculo;

typedef struct {
    int       id;
    char      fecha[11];              /* AAAA-MM-DD */
    Categoria categoria;
    double    monto;
    long      km;                     /* lectura del odometro */
    double    litros;                 /* solo para combustible */
    char      descripcion[COSTOS_DESC];
} Gasto;

typedef struct {
    Vehiculo vehiculo;
    int      tiene_vehiculo;
    Gasto   *gastos;                  /* siempre ordenado por fecha */
    int      cantidad;
    int      capacidad;
    int      siguiente_id;
} Registro;

void        registro_iniciar(Registro *r);
void        registro_liberar(Registro *r);

int         vehiculo_definir(Registro *r, const Vehiculo *v);
int         gasto_agregar(Registro *r, const char *fecha, Categoria cat, double monto,
                          long km, double litros, const char *descripcion, int *id_nuevo);
int         gasto_eliminar(Registro *r, int id);

double      total_general(const Registro *r);
double      total_categoria(const Registro *r, Categoria cat);
int         costo_por_km(const Registro *r, double *resultado);
int         consumo_promedio(const Registro *r, double *km_por_litro);

int         registro_guardar(const Registro *r, const char *ruta);
int         registro_cargar(Registro *r, const char *ruta);

int         fecha_valida(const char *fecha);
const char *categoria_nombre(Categoria cat);
const char *costos_mensaje(int codigo);

#endif
