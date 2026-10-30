#ifndef TIPOS_H_INCLUDED
#define TIPOS_H_INCLUDED

#define TAM_COD 2
#define TAM_DESC 49
#define TAM_CONT 19

typedef struct
{
    int anio;
    int trimestre;
    char codigo_pais[TAM_COD + 1];
    char descripcion_pais[TAM_DESC + 1];
    char operacion;
    float monto;

}tPersonales;

typedef struct{
    char codigo_pais[TAM_COD + 1];
    char descripcion_pais[TAM_DESC + 1];
    char continente[TAM_CONT + 1];
}tContinentes;

typedef struct{
    char continente[TAM_CONT + 1];
    int registros;
    double total_credito;
    double total_debito;
}tClasificacion;

typedef struct{
    int anio;
    int trimestre;
    char codigo_pais[TAM_COD + 1];
    char descripcion_pais[TAM_DESC + 1];
    char continente[TAM_CONT + 1];
    char operacion;
    float monto;
}tIntermedio;

typedef struct{
    char codigo_pais[TAM_COD + 1];
    char descripcion_pais[TAM_DESC + 1];
    int cantTrimDeb;
    int cantTrimCred;
    float promDebito;
    float promCredito;
}tResulPromPais;

// Punto 9
typedef struct{
    int anio;
    char continente[TAM_CONT + 1];
    float promTrimDeb;
    float promTrimCred;
}tResulPromCont;

typedef struct{
    char codigoPais[TAM_COD + 1];
    char descripcionPais[TAM_DESC + 1];
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

typedef struct {
    char continente[TAM_CONT + 1];
    float acumCred;
    float acumDeb;
} tParticipacionContAcum;

typedef struct {
    char continente[TAM_CONT + 1];
    float porcPartCred;
    float porcPartDeb;
} tParticipacionContBin;

#endif // TIPOS_H_INCLUDED
