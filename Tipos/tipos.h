#ifndef TIPOS_H_INCLUDED
#define TIPOS_H_INCLUDED

typedef struct
{
    int anio;
    int trimestre;
    char codigo_pais[3];
    char descripcion_pais[50];
    char operacion;
    float monto;
    
}tPersonales;

typedef struct{
    char codigo_pais[3];
    char descripcion_pais[50];
    char continente[20];
}tContinentes;

typedef struct{
    char continente[20];
    int registros;
    float total_credito;
    float total_debito;
}tClasificacion;

typedef struct{
    int anio;
    int trimestre;
    char codigo_pais[3];
    char descripcion_pais[50];
    char continente[20];
    char operacion;
    float monto;
}tIntermedio;

typedef struct{
    char codigoPais[3];
    char descripcionPais[50];
    int cantidadRegistros;
    float totalCreditos;
    float totalDebitos;
}tTotalOperaciones;

#endif // TIPOS_H_INCLUDED
