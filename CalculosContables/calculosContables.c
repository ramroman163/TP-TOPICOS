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

    int cantidad = (int)vectorCE(transferencias);

    for(int i = 0 ; i < cantidad ; i++)
    {
        tPersonales *transferencia = vectorGetElementoDePos(transferencias, i);
        
        int pos = vectorOrdBuscar(&continentes, transferencia, compararCodigoPais);

        tContinentes *continente = vectorGetElementoDePos(&continentes, pos);

        tIntermedio elem;

        elem.anio = transferencia->anio;
        elem.trimestre = transferencia->trimestre;
        strcpy(elem.codigo_pais, transferencia->codigo_pais);
        strcpy(elem.descripcion_pais, transferencia->descripcion_pais);
        strcpy(elem.continente, continente->continente);
        elem.operacion = transferencia->operacion;
        elem.monto = transferencia->monto;

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

    Vector clasificaciones;
    char linea[256];
    tIntermedio inter;
    tClasificacion clasif;

    vectorCrear(&clasificaciones, sizeof(tClasificacion));

    fgets(linea, sizeof(linea), p_intermedio);

    while(fgets(linea, sizeof(linea), p_intermedio) != NULL)
    {
        armaIntermedio(linea, &inter);

        int pos = vectorDesordBuscar(&clasificaciones, &inter, compararContinente);

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

            vectorInsertarAlFinal(&clasificaciones, &clasif);
        }
        else
        {
            tClasificacion *clasificacion = vectorGetElementoDePos(&clasificaciones, pos);

            clasificacion->registros++;

            if(inter.operacion == 'C')
            {
                clasificacion->total_credito += inter.monto;
            }
            if(inter.operacion == 'D')
            {
               clasificacion->total_debito += inter.monto;
            }
        }
    }

    fclose(p_intermedio);

    vectorOrdenar(&clasificaciones, INSERCION, compararCreditoDesc);

    printf("CONTINENTE - REGISTROS - TOTAL_CREDITO - TOTAL_DEBITO\n");

    vectorMostrar(&clasificaciones, printClasificacion);

    vectorDestruir(&clasificaciones);
}

//punto 1

void ordenarTransferenciasxCod(Vector* transferencias)
{
    vectorOrdenar(transferencias, FUSION, compararTransferenciasChar);
}

void ordenarTransferenciasxCredito(Vector* transferencias)
{
    vectorOrdenar(transferencias, FUSION, compararOperacionesFloat);
}

void contaryCargarOperaciones(const Vector* vTransfercias, Vector* vTransfResumen)
{
    if(vTransfercias->ce == 0)
        return;

    tPersonales* i = (tPersonales*) vTransfercias->vec;
    tPersonales* ult = (tPersonales*) vTransfercias->vec + (vTransfercias->ce - 1);

    tTotalOperaciones opResumen;

    while(i <= ult)
    {
        int contRegistros = 0; 
        float acumCreditos = 0, acumDebitos = 0;
        char codPaisActual[3];
        char descPaisActual[50];

        strcpy(codPaisActual, i->codigo_pais);
        strcpy(descPaisActual, i->descripcion_pais);

        do
        {
            contRegistros++;

            if(i->operacion == 'C')
                acumCreditos += i->monto;

            if(i->operacion == 'D')
                acumDebitos += i->monto;

            i++;

        } while(i<=ult && strcmpi(codPaisActual, i->codigo_pais) == 0);

        strcpy(opResumen.codigoPais, codPaisActual);
        strcpy(opResumen.descripcionPais, descPaisActual); //(i-1)->descripcion_pais
        opResumen.cantidadRegistros = contRegistros;
        opResumen.totalCreditos = acumCreditos;
        opResumen.totalDebitos = acumDebitos;

        //guarda en pos 0 e incrementa ce en uno, una vez que encuentra otro país, cargo todo en la prox pos
  
        vectorInsertarEnPos(vTransfResumen, &opResumen, vTransfResumen->ce);
    }

}

void mostrarVectorTransfResumen(const Vector* v, Print printResumen)
{
    printf("\n");
    printf("%-10s %-30s %-10s %-15s %-15s\n", "PAIS_COD", "PAIS_DESC", "registros", "total_credito", "total_debito");
    vectorMostrar(v, printResumen);
}

void totalesPorPais(const char* path_transferencias, const Vector *vTransferencias)
{
    ordenarTransferenciasxCod(vTransferencias);

    Vector vResumen; //para  guardar la nueva estructura en un vector

    vectorCrear(&vResumen, sizeof(tTotalOperaciones));

    //hago corte de control
    contaryCargarOperaciones(vTransferencias, &vResumen);
    
    ordenarTransferenciasxCredito(&vResumen);

    mostrarVectorTransfResumen(&vResumen, printResumen);
}








