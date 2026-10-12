#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

int ingresarEntero(char *mensaje);
int ingresarEnteroRango(int minimo, int maximo, char *mensaje);
int ingresarEnteroPositivo(char *mensaje);
float ingresarFloat(char *mensaje);
float ingresarFloatPositivo(char *mensaje);
void limpiar_buffer(void);
void armaTransfPersonales(const char *registro, void * elemento);

#endif // UTILS_H_INCLUDED
