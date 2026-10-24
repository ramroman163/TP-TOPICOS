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
int compararIntermedio(const void *a, const void *b);
float compararPromTrim(const void* a, const void* b);
int compararCreditoDesc(const void *a, const void *b);
void printClasificacion(const void *e);
void printResulTrim(const void *e);
int compararPaisIntermedio(const void *a, const void *b);
int compararContinenteAnio(const void* a, const void* b);

#endif // UTILS_H_INCLUDED
