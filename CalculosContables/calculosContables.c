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

//punto 3

//Filtro las transferencias del año ingresado
int filtrarTransferenciasAnio(const Vector* transferencias, Vector* vAnio, int anio)
{
    const tPersonales* i = (const tPersonales*) transferencias->vec;
    size_t j;

    for(j = 0; j < transferencias->ce; j++)
    {
        if((i + j)->anio == anio)
        {
            if(vectorInsertarAlFinal(vAnio, i + j) != TODO_OK)
                return 0;
        }
    }
    return 1;
}

int compararPaisNombre(const void* a, const void* b)
{
    const tPersonales* p1 = (const tPersonales*) a;
    const tPersonales* p2 = (const tPersonales*) b;
    int cmp = strcmp(p1->descripcion_pais, p2->descripcion_pais);

    if(cmp != 0)
        return cmp;
    return strcmp(p1->codigo_pais, p2->codigo_pais);
}

void calcularSaldosTrimestrales(const Vector* vAnio)
{
    const tPersonales *i, *ult, *pais;
    size_t cantPaises = 0, fila = 0, j, k;
    int trimestre;
    float (*saldos)[4];
    FILE* arch;

    i = (const tPersonales*) vAnio->vec;
    ult = i + vAnio->ce;

    while(i < ult)
    {
        pais = i;
        cantPaises++;

        do
        {
            i++;
        } } while(i < ult && compararPaisNombre(pais, i) == 0);
    }

    //Una fila por pais y 4 columnas
    saldos = malloc(cantPaises * sizeof(*saldos));
    if(saldos == NULL)
    {
        printf("Error al reservar memoria.\n");
        return;
    }
    for(k = 0; k < cantPaises; k++)
        for(j = 0; j < 4; j++)
            *(*(saldos + k) + j) = 0;

    //Creo el archivo de texto
    arch = fopen("saldo_trimestral.txt", "wt");
    if(arch == NULL)
    {
        printf("Error al crear el archivo.\n");
        free(saldos);
        return;
    }
    fprintf(arch, "%-30s %12s %12s %12s %12s\n", "PAIS", "T1", "T2", "T3", "T4");
    i = (const tPersonales*) vAnio->vec;

    //Calculo los saldos de cada pais
    while(i < ult)
    {
        pais = i;
        do
        {
            trimestre = i->trimestre - 1;
            if(trimestre >= 0 && trimestre < 4)
            {
                if(i->operacion == 'C')
                    *(*(saldos + fila) + trimestre) += i->monto;
                else
                {
                    if(i->operacion == 'D')
                        *(*(saldos + fila) + trimestre) -= i->monto;
                }
            }
            i++;
        } } while(i < ult && compararPaisNombre(pais, i) == 0);

        fprintf(arch, "%-30s", pais->descripcion_pais);
        for(j = 0; j < 4; j++)
            fprintf(arch, " %12.2f", *(*(saldos + fila) + j));
        fprintf(arch, "\n");

        fila++;
    }

    // Cierro el archivo y libero la memoria
    fclose(arch);
    free(saldos);
    printf("Archivo generado: saldo_trimestral.txt\n");
}

void saldoTrimestralPorPais(const Vector* transferencias)
{
    int anio;
    Vector vAnio;

    anio = ingresarEnteroRango(2015, 2026, "Ingrese el anio: ");

    if(!vectorCrear(&vAnio, sizeof(tPersonales)))
        return;

    if(!filtrarTransferenciasAnio(transferencias, &vAnio, anio))
    {
        printf("Error al guardar las transferencias.\n");
        vectorDestruir(&vAnio);
        return;
    }

    if(vAnio.ce == 0)
        printf("No hay registros para ese anio.\n");
    else
    {
        vectorOrdenar(&vAnio, FUSION, compararPaisNombre);
        calcularSaldosTrimestrales(&vAnio);
    }
    vectorDestruir(&vAnio);
}
