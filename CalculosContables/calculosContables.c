#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

// Punto 8
int promedioTrimestralPorPais(const char *path_intermedio)
{
    Vector miVecInter;
    Vector miVecResul;

    if(!vectorCrear(&miVecInter, sizeof(tIntermedio)))
    {
        return SIN_MEM;
    }

    if(!vectorCrear(&miVecResul, sizeof(tResulProm)))
    {
        return SIN_MEM;
    }

    cargarArchivoEnVector(path_intermedio, &miVecInter, armaIntermedio);

    vectorOrdenar(&miVecInter, FUSION, compararPaisIntermedio);

    int contTrimDeb;
    int contTrimCred;
    float acumTrimDeb;
    float acumTrimCred;

    tIntermedio actObjInt;
    tResulProm miObjResul;
    char codPais[3];

    size_t tamInter = sizeof(tIntermedio);

    void* i = miVecInter.vec;
    void* ult = miVecInter.vec + (vectorCE(&miVecInter) - 1) * tamInter;

    memcpy(&actObjInt, i, sizeof(tIntermedio));
    
    while(i <= ult)
    {
        strcpy(codPais, actObjInt.codigo_pais);

        contTrimDeb = contTrimCred = 0;
        acumTrimDeb = acumTrimCred = 0.0;

        strcpy(miObjResul.codigo_pais, actObjInt.codigo_pais);
        strcpy(miObjResul.descripcion_pais, actObjInt.descripcion_pais);

        while(i <= ult && strcmp(actObjInt.codigo_pais, codPais) == 0)
        {
            if(actObjInt.monto > 0)
            {   
                if(actObjInt.operacion == 'C')
                {
                    contTrimCred++;
                    acumTrimCred += actObjInt.monto;
                } 
                else
                {
                    contTrimDeb++;
                    acumTrimDeb += actObjInt.monto;
                }
                
            }

            i += tamInter;

            if(i <= ult)
            {
                memcpy(&actObjInt, i, sizeof(tIntermedio));
            }
        }

        miObjResul.operacion = 'C';
        
        if(contTrimCred == 0)
        {
            miObjResul.prom = 0.0;
        }
        else
        {
            miObjResul.prom = (acumTrimCred) / contTrimCred;
        }

        miObjResul.cantTrim = contTrimCred;

        vectorInsertarAlFinal(&miVecResul, &miObjResul);

        miObjResul.operacion = 'D';
        
        if(contTrimDeb == 0)
        {
            miObjResul.prom = 0.0;
        }
        else
        {
            miObjResul.prom = (acumTrimDeb) / contTrimDeb;
        }

        miObjResul.cantTrim = contTrimDeb;

        vectorOrdInsertar(&miVecResul, &miObjResul, compararPromTrim);
    }

    // ordenamiento va aca

    mostrarResulPromTrim(&miVecResul, printResulTrim);

    vectorDestruir(&miVecInter);
    vectorDestruir(&miVecResul);
}

void mostrarResulPromTrim(const Vector* miVecResul, Print printResulTrim)
{
    printf("COD_PAIS - NOMBRE_PAIS - TIPO OPERACION - CANT TRIMESTRES - PROMEDIO\n");
    vectorMostrar(miVecResul, printResulTrim);
}

// Punto 9

/*

Utilizando como entrada el archivo transferencias_base_larga.csv generado en el Punto 7, calcular el promedio trimestral de las operaciones para cada continente, discriminando por año y por tipo de operación (Crédito y Débito).
Para cada combinación de continente, año y tipo de operación, el promedio deberá calcularse considerando únicamente aquellos trimestres en los que el monto acumulado sea mayor que cero. Los trimestres sin operaciones no deberán incluirse en el cálculo.
El programa deberá contemplar el caso en que un continente no registre operaciones positivas para un determinado año y tipo de operación, evitando la división por cero. En ese caso, el promedio informado deberá ser 0.
Finalmente, presentar los resultados en una tabla donde cada fila represente un continente y un año, y las columnas indiquen el promedio trimestral de Créditos y el promedio trimestral de Débitos.
La salida deberá tener un formato similar al siguiente


Ordenamos base_larga por continente - año.3 

Hacemos corte de control acumulando y contando. 

Validamos que no hacemos división por 0. 

Calcular promedio trimestral de créditos y el promedio trimestral de débitos. 

Mostrar tabla. 

*/

int promedioTrimestralPorContinente(const char *path_intermedio)
{
    Vector miVecInter;
    Vector miVecResul;

    if(!vectorCrear(&miVecInter, sizeof(tIntermedio)))
    {
        return SIN_MEM;
    }

    if(!vectorCrear(&miVecResul, sizeof(tResulPromCont)))
    {
        return SIN_MEM;
    }

    cargarArchivoEnVector(path_intermedio, &miVecInter, armaIntermedio);

    vectorOrdenar(&miVecInter, FUSION, compararContinenteAnio);
}