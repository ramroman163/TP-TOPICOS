#include <stdio.h>
#include <stdlib.h>
#include "../TDAVector/Vector.h"
#include "../Archivos/archivos.h"

void cargarArchivoEnVector(const char *path, Vector *v, CargaStruct accion)
{
    FILE *archivo;
    char linea[256];
    size_t tamanio = vectorGetTamElem(v);
    void *elemento = malloc(tamanio);

    if(!elemento)
    {
        puts("Hubo un error reinicie el programa");
        return;
    }

    archivo = fopen(path, "r");
    if (archivo == NULL)
    {
        printf("Error: no se pudo abrir %s\n", path);
        return;
    }

    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        accion(linea, elemento); //aca se guarda el registro en la estructura correspondiente

        vectorInsertarAlFinal(v, elemento); //lo guardo en el vector
    }

    free(elemento);

    fclose(archivo);
}

void exportar_vec_a_binario(const char *path, const Vector *v)
{
    FILE *pbin = fopen(path, "wb");

    if (!pbin)
    {
        puts("Hubo un error reinicie el programa");
        return;
    }

    size_t tamanio = vectorGetTamElem(v);
    size_t ce = vectorCE(v);
    void *vector = vectorGetElementoDePos(v, 0);

    fwrite(vector, tamanio, ce, pbin);

    fclose(pbin);
}
