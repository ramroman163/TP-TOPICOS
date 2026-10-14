#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "../TDAVector/Vector.h"

void cargarArchivoEnVector(const char *path, Vector *v, void (*accion) (const char * registro, void * elemento));
void exportar_vec_a_txt_var(const char *path, const Vector *v, void (*accion) (const Vector *v, FILE *p));
void exportar_vec_a_binario(const char *path, const Vector *v);

#endif // ARCHIVOS_H_INCLUDED
