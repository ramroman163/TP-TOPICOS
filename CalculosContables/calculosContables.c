#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "../Tipos/tipos.h"
#include "../Archivos/archivos.h"
#include "../Utils/utils.h"
#include "../TDAVector/Vector.h"

bool verificarExistenciaArchivoIntermedio(const char *path_intermedio)
{
    FILE *arch = fopen(path_intermedio, "rt");

    if(arch == NULL)
        return false;

    fclose(arch);

    return true;
}

// Punto 7

void generarArchivoIntermedio(const char *path_intermedio, const char *path_continentes, const Vector* transferencias)
{
    Vector continentes;

    vectorCrear(&continentes, sizeof(tContinentes));

    cargarArchivoEnVector(path_continentes, &continentes, armaContinentes);

    FILE *p_intermedio = fopen(path_intermedio, "wt");

    if(p_intermedio == NULL)
    {
        printf("Error: no se pudo abrir %s\n", path_intermedio);
        return;
    }

    fprintf(p_intermedio, "ANIO;TRIMESTRE;PAIS_COD;PAIS_DESC;CONTINENTE;OPERACION;MONTO\n");

    for(size_t i = 0 ; i < transferencias->ce ; i++)
    {
        int pos = vectorOrdBuscar(&continentes, ((tPersonales*)(transferencias->vec)) + i, compararCodigoPais);

        tIntermedio elem;

        elem.anio = (((tPersonales*)(transferencias->vec)) + i)->anio;
        elem.trimestre = (((tPersonales*)(transferencias->vec)) + i)->trimestre;
        strcpy(elem.codigo_pais, (((tPersonales*)(transferencias->vec))+ i)->codigo_pais);
        strcpy(elem.descripcion_pais, (((tPersonales*)(transferencias->vec))+ i)->descripcion_pais);
        strcpy(elem.continente, (((tContinentes*)(continentes.vec))+ pos)->continente);
        elem.operacion = (((tPersonales*)(transferencias->vec)) + i)->operacion;
        elem.monto = (((tPersonales*)(transferencias->vec)) + i)->monto;

        fprintf(p_intermedio,"%d;%d;%s;%s;%s;%c;%.2f\n", elem.anio, elem.trimestre, elem.codigo_pais,
                elem.descripcion_pais, elem.continente, elem.operacion, elem.monto);
    }

    fclose(p_intermedio);
    vectorDestruir(&continentes);
}


// Punto 2

void totalesPorContinente(const char *path_intermedio)
{
    FILE *p_intermedio = fopen(path_intermedio, "rt");

    if(p_intermedio == NULL)
    {
        printf("Error: no se pudo abrir %s\n", path_intermedio);
        return;
    }

    Vector clasificacion;
    char linea[256];
    tIntermedio inter;
    tClasificacion clasif;

    vectorCrear(&clasificacion, sizeof(tClasificacion));

    fgets(linea, sizeof(linea), p_intermedio);

    while(fgets(linea, sizeof(linea), p_intermedio) != NULL)
    {
        armaIntermedio(linea, &inter);

        int pos = vectorDesordBuscar(&clasificacion, &inter, compararContinente);

        if(pos == -1)
        {
            strcpy(clasif.continente, inter.continente);
            clasif.registros = 1;
            clasif.total_credito = 0;
            clasif.total_debito = 0;

            if(inter.operacion == 'C')
                clasif.total_credito += inter.monto;

            if(inter.operacion == 'D')
                clasif.total_debito += inter.monto;

            vectorInsertarAlFinal(&clasificacion, &clasif);
        }
        else
        {
            (((tClasificacion*)(clasificacion.vec)) + pos)->registros++;

            if(inter.operacion == 'C')
            {
                (((tClasificacion*)(clasificacion.vec)) + pos)->total_credito += inter.monto;
            }
            if(inter.operacion == 'D')
            {
                (((tClasificacion*)(clasificacion.vec)) + pos)->total_debito += inter.monto;
            }
        }
    }

    fclose(p_intermedio);

    vectorOrdenar(&clasificacion, INSERCION, compararCreditoDesc);

    printf("CONTINENTE - REGISTROS - TOTAL_CREDITO - TOTAL_DEBITO\n");

    vectorMostrar(&clasificacion, printClasificacion);

    vectorDestruir(&clasificacion);
}


//Punto 5

void rankingPaises(const Vector *transferencias)
{
    Vector paises;
    tPaisTotales paisAux;
    char operacion;
    int n;
    size_t totalPaises;
    size_t i;

    if (transferencias == NULL || transferencias->ce == 0)
    {
        printf("ERROR: No hay datos de transferencias cargados en memoria.\n");
        return;
    }

//Valido e ingreso el tipo de operacion
    do
    {
        printf("Ingrese el tipo de operacion ('C' para Credito, 'D' para Debito): ");
        scanf(" %c", &operacion);

        operacion = toupper(operacion);
    } while (operacion != 'C' && operacion != 'D');


    vectorCrear(&paises, sizeof(tPaisTotales));

    for (i=0; i< transferencias->ce; i++) //Acumulo registros por pais
    {
        const tPersonales *t = ((const tPersonales *)(transferencias->vec)) + i;


        int pos = vectorDesordBuscar(&paises, t, compararCodigoPaisTotales);

        if (pos == -1)
        {
            strcpy(paisAux.codigo_pais, t->codigo_pais);
            strcpy(paisAux.descripcion_pais, t->descripcion_pais);
            paisAux.registros = 1;
            paisAux.total_credito = (t->operacion == 'C') ? t->monto : 0;
            paisAux.total_debito = (t->operacion == 'D') ? t->monto : 0;

            vectorInsertarAlFinal(&paises, &paisAux);
        }
            else
            {
                tPaisTotales *paisExistente = ((tPaisTotales *)(paises.vec)) + pos;
                paisExistente->registros++;

                if (t->operacion == 'C')
                    paisExistente->total_credito += t->monto;
                else if (t->operacion == 'D')
                    paisExistente->total_debito += t->monto;
            }
    }

    totalPaises = vectorCE(&paises);

    if (totalPaises == 0)
    {
        printf("No se encontraron registros de paises para procesar.\n");
        vectorDestruir(&paises);
        return;
    }

    printf("\n=============================================================================\n");
    printf("-------------------- RANKING DE PAISES -------------------\n");
    printf("\n==============================================================================\n");

    n = ingresarEnteroRango(1, (int)totalPaises, "Ingrese la cantidad N de paises a listar:");

        //Comienzo a ordenar
    if (operacion == 'C')
        vectorOrdenar(&paises, INSERCION, compararCreditoPaisDesc);
    else
        vectorOrdenar(&paises, INSERCION, compararDebitoPaisDesc);


        printf("\n%-10s | %-25s | %-10s | %-15s | %-15s\n", "PAIS_COD", "PAIS_DESC", "registros", "total_credito", "total_debito");

        for(i=0; i < (size_t)n; i++)
        {
            const tPaisTotales *pais = ((const tPaisTotales *)(paises.vec)) + i;

        printf("%-10s | %-25s | %10d | %15.2f | %15.2f\n", pais->codigo_pais, pais->descripcion_pais, pais->registros, pais->total_credito, pais->total_debito);
    }

    vectorDestruir(&paises);
}

