#include <stdio.h>
#include <stdlib.h>
// aca incluir la biblio de tda vector dinamico generico

void cargarArchivoEnVector(const char *path, Vector *v, void (*accion) (const char * registro, void * elemento))
{
    FILE *archivo;
    char linea[256];
    void *elemento = malloc(v->tamElem);

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

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        /* saltar linea de encabezado o lineas vacias */
        if (linea[0] == '\n' || linea[0] == '\r' || linea[0] == '#')
            continue;

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

    fwrite(v->vec, v->tamElem, v->ce, pbin);

    fclose(pbin);
}

void exportar_vec_a_txt_var(const char *path, const Vector *v, void (*accion) (const Vector *v, FILE *p))
{
    FILE *ptxt = fopen(path, "wt");

    if (!ptxt)
    {
        puts("Hubo un error reinicie el programa");
        return;
    }

    accion(v, ptxt);

    fclose(ptxt);
}