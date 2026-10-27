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
    double total_credito;
    double total_debito;
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
    char codigo_pais[3];
    char descripcion_pais[50];
    int cantTrimDeb;
    int cantTrimCred;
    float promDebito;
    float promCredito;
}tResulPromPais;

// Punto 9
typedef struct{
    int anio;
    char continente[20];
    float promTrimDeb;
    float promTrimCred;
}tResulPromCont;

typedef struct{
    char codigoPais[3];
    char descripcionPais[50];
    int cantidadRegistros;
    float totalCreditos;
    float totalDebitos;
}tTotalOperaciones;

// Estructura agregada para el Punto 4
typedef struct {
    int anio;
    float totalCredito;
    float totalDebito;
    float saldo;
    float variacion;
} tAnioResumen;

#endif // TIPOS_H_INCLUDED
