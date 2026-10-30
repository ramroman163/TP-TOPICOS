#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "../TDAVector/Vector.h"

typedef int (*CargaStruct)(const char * registro, void * elemento);

int cargarArchivoEnVector(const char *path, Vector *v, CargaStruct accion);
void exportar_vec_a_binario(const char *path, const Vector *v);

#endif // ARCHIVOS_H_INCLUDED
