#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

int ingresarEntero(char *mensaje);
int ingresarEnteroRango(int minimo, int maximo, char *mensaje);
int ingresarEnteroPositivo(char *mensaje);
float ingresarFloat(char *mensaje);
float ingresarFloatPositivo(char *mensaje);
void limpiarBuffer(void);
void armaTransfPersonales(const char *registro, void * elemento);
void armaIntermedio(const char *registro, void * elemento);
void armaContinentes(const char *registro, void * elemento);
int compararCodigoPais(const void *a, const void *b);
int compararContinente(const void *a, const void *b);
int compararCreditoDesc(const void *a, const void *b);
void printClasificacion(const void *e);
int compararTransferenciasChar(const void* e1, const void* e2);
int compararOperacionesFloat(const void* a, const void* b);
void printResumen(const void *e);

int compararCodigoPaisTotales(const void *a, const void *b); //Para saber si un registro de transferencias
//pertenece a un pais que ya tenemos en nFuestro vector acumulado
int compararCreditoPaisDesc(const void *a, const void *b); //Para ordenar de Mayor a Menor por credito
int compararDebitoPaisDesc(const void *a, const void *b); //Para ordenar de Mayor a Menor por debito


#endif // UTILS_H_INCLUDED
