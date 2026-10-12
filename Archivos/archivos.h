#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

void cargarArchivoEnVector(const char *path, Vector *v, void (*accion) (const char * registro, void * elemento));

#endif // ARCHIVOS_H_INCLUDED
