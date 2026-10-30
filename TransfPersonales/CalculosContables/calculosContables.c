#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "../Tipos/tipos.h"
#include "../Archivos/archivos.h"
#include "../Utils/utils.h"
#include "../TDAVector/Vector.h"
#include "calculosContables.h"

int acumularDatosAnuales(const Vector *transferencias, Vector* anios);
void calcularSaldosYVariaciones(Vector* anios);

bool verificarExistenciaArchivoIntermedio(const char *path_intermedio)
{
    FILE *arch = fopen(path_intermedio, "rt");

    if(arch == NULL)
        return false;

    fclose(arch);

    return true;
}

// Punto 1
int contaryCargarOperaciones(const Vector* vCopiaTransferencias, Vector* vTransfResumen)
{
    int cantidadElem = (int)vectorCE(vCopiaTransferencias);

    if(cantidadElem == 0)
    {
        return SIN_DATOS;
    }

    tPersonales* i = vectorGetElementoDePos(vCopiaTransferencias, 0);
    tPersonales* ult = vectorGetElementoDePos(vCopiaTransferencias, cantidadElem - 1);
    tTotalOperaciones opResumen;

    while(i <= ult)
    {
        int contRegistros = 0;
        float acumCreditos = 0, acumDebitos = 0;
        char codPaisActual[TAM_COD + 1];
        char descPaisActual[TAM_DESC + 1];

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

        } while(i<=ult && strcmp(codPaisActual, i->codigo_pais) == 0);

        strcpy(opResumen.codigoPais, codPaisActual);
        strcpy(opResumen.descripcionPais, descPaisActual);
        opResumen.cantidadRegistros = contRegistros;
        opResumen.totalCreditos = acumCreditos;
        opResumen.totalDebitos = acumDebitos;

        //guarda en pos 0 e incrementa ce en uno, una vez que encuentra otro país, cargo todo en la prox pos

        int codRet = vectorInsertarEnPos(vTransfResumen, &opResumen, vectorCE(vTransfResumen));

        if(codRet != TODO_OK)
        {
            return codRet;
        }
    }

    return TODO_OK;
}

void mostrarVectorTransfResumen(const Vector* v)
{
    printf("\n");
    printf("%-10s | %-50s | %10s | %8s | %8s\n", "PAIS_COD", "PAIS_DESC", "REGISTROS", "TOTAL_CREDITO", "TOTAL_DEBITO");
    printf("-------------------------------------------------------------------------------------------------------------\n");
    vectorMostrar(v, printResumen);
}

int totalesPorPais(const Vector *vTransferencias)
{
    Vector copiaTransferencias; //para no reordenar el vector principal

    int codRet = vectorCopiar(&copiaTransferencias, vTransferencias);

    if(codRet != TODO_OK)
        return codRet;

    if(vectorCE(&copiaTransferencias) == 0)
    {
        vectorDestruir(&copiaTransferencias);
        return SIN_DATOS;
    }

    vectorOrdenar(&copiaTransferencias, FUSION, compararTransferenciasChar);

    Vector vResumen; //para  guardar la nueva estructura en un vector

    if(!vectorCrear(&vResumen, sizeof(tTotalOperaciones)))
    {
        vectorDestruir(&copiaTransferencias);
        return SIN_MEM;
    }

    //hago corte de control
    codRet = contaryCargarOperaciones(&copiaTransferencias, &vResumen);

    if(codRet != TODO_OK)
    {
        vectorDestruir(&copiaTransferencias);
        vectorDestruir(&vResumen);
        return codRet;
    }

    vectorOrdenar(&vResumen, FUSION, compararCreditoPaisDesc);

    mostrarVectorTransfResumen(&vResumen);

    vectorDestruir(&copiaTransferencias);
    vectorDestruir(&vResumen);

    return TODO_OK;
}

// Punto 2
int totalesPorContinente(const char *path_intermedio)
{
    FILE *p_intermedio = fopen(path_intermedio, "rt");

    if(p_intermedio == NULL)
    {
        printf("Error: no se pudo abrir %s\n", path_intermedio);
        return ACCESO_ARCH_FALLIDO;
    }

    Vector clasificaciones;
    char linea[256];
    tIntermedio inter;
    tClasificacion clasif;

    if(!vectorCrear(&clasificaciones, sizeof(tClasificacion)))
    {
        fclose(p_intermedio);
        return SIN_MEM;
    }

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

            int codRet = vectorInsertarAlFinal(&clasificaciones, &clasif);

            if(codRet != TODO_OK)
            {
                fclose(p_intermedio);
                vectorDestruir(&clasificaciones);
                return codRet;
            }
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

    printf("%-20s | %10s | %8s | %8s\n", "CONTINENTE", "REGISTROS", "CREDITO", "DEBITO");
    printf("---------------------------------------------------------\n");

    vectorMostrar(&clasificaciones, printClasificacion);

    vectorDestruir(&clasificaciones);

    return TODO_OK;
}

// Punto 3
// Filtro las transferencias del año ingresado
int filtrarTransferenciasAnio(const Vector* transferencias, Vector* vAnio, int anio)
{
    const tPersonales* i = vectorGetElementoDePos(transferencias, 0);
    size_t j;

    for(j = 0; j < (int)vectorCE(transferencias); j++)
    {
        if((i + j)->anio == anio)
        {
            int codRet = vectorInsertarAlFinal(vAnio, i + j);
            if(codRet != TODO_OK)
                return codRet;
        }
    }
    return TODO_OK;
}

int calcularSaldosTrimestrales(const Vector* vAnio)
{
    const tPersonales *i, *ult, *pais;
    size_t cantPaises = 0, fila = 0, j, k;
    int trimestre;
    float (*saldos)[4];
    FILE* arch;

    i = (const tPersonales*) vectorGetElementoDePos(vAnio, 0);
    ult = vectorGetElementoDePos(vAnio, (int)vectorCE(vAnio));

    while(i < ult)
    {
        pais = i;
        cantPaises++;

        do
        {
            i++;
        }
        while(i < ult && compararPaisNombre(pais, i) == 0);
    }

    //Una fila por pais y 4 columnas
    saldos = malloc(cantPaises * sizeof(*saldos));

    if(saldos == NULL)
    {
        return SIN_MEM;
    }

    for(k = 0; k < cantPaises; k++)
        for(j = 0; j < 4; j++)
            *(*(saldos + k) + j) = 0;

    //Creo el archivo de texto
    arch = fopen("saldo_trimestral.txt", "wt");

    if(arch == NULL)
    {
        free(saldos);
        printf("Error: no se pudo abrir %s\n", "saldo_trimestral.txt");
        return ACCESO_ARCH_FALLIDO;
    }

    fprintf(arch, "%-49s %12s %12s %12s %12s\n", "PAIS", "T1", "T2", "T3", "T4");
    printf("\n%-49s | %12s | %12s | %12s | %12s\n", "PAIS", "T1", "T2", "T3", "T4");
    printf("--------------------------------------------------------------------------------------------------------------\n");
    i = vectorGetElementoDePos(vAnio, 0);

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
        }
        while(i < ult && compararPaisNombre(pais, i) == 0);

        fprintf(arch, "%-50s", pais->descripcion_pais);
        printf("%-50s", pais->descripcion_pais);

        for(j = 0; j < 4; j++)
        {
            fprintf(arch, " %12.2f", *(*(saldos + fila) + j));
            printf("| %12.2f ", *(*(saldos + fila) + j));
        }

        fprintf(arch, "\n");
        printf("\n");

        fila++;
    }

    // Cierro el archivo y libero la memoria
    fclose(arch);
    free(saldos);

    printf("\nArchivo generado: saldo_trimestral.txt\n");

    return TODO_OK;
}

int saldoTrimestralPorPais(const Vector* transferencias)
{
    int anio;
    int codRet;
    Vector vAnio;

    anio = ingresarEnteroRango(2015, 2026, "Ingrese el anio: ");

    if(!vectorCrear(&vAnio, sizeof(tPersonales)))
    {
        return SIN_MEM;
    }

    codRet = filtrarTransferenciasAnio(transferencias, &vAnio, anio);

    if(codRet != TODO_OK)
    {
        printf("Error al guardar las transferencias.\n");
        vectorDestruir(&vAnio);
        return codRet;
    }

    if((int)vectorCE(&vAnio) == 0)
        return SIN_DATOS;
    else
    {
        vectorOrdenar(&vAnio, FUSION, compararPaisNombre);
        calcularSaldosTrimestrales(&vAnio);
    }
    vectorDestruir(&vAnio);

    return TODO_OK;
}

// Punto 4
int acumularDatosAnuales(const Vector *transferencias, Vector* anios)
{
    int codRet;

    tPersonales* i = vectorGetElementoDePos(transferencias, 0);
    tPersonales* ult = vectorGetElementoDePos(transferencias, (int) vectorCE(transferencias) - 1);

    tAnioResumen anioActual;

    // 2. Recorremos el resto del vector aplicando el corte de control
    while(i <= ult)
    {
        anioActual.anio = i->anio;
        anioActual.totalCredito = 0.0;
        anioActual.totalDebito = 0.0;

        while(i <= ult && anioActual.anio == i->anio)
        {
            if (i->operacion == 'C')
            {
                anioActual.totalCredito += i->monto;
            }
            if (i->operacion == 'D')
            {
                anioActual.totalDebito += i->monto;
            }

            i++;
        }

        anioActual.saldo = anioActual.totalCredito - anioActual.totalDebito;

        codRet = vectorInsertarAlFinal(anios, &anioActual);

        if(codRet != TODO_OK)
        {
            return codRet;
        }
    }

    return TODO_OK;
}
// --- Función auxiliar 2: Calcula saldos y variaciones interanuales ---
void calcularSaldosYVariaciones(Vector* anios)
{
    tAnioResumen* i = vectorGetElementoDePos(anios, 0);
    tAnioResumen* ult = vectorGetElementoDePos(anios, (int)vectorCE(anios) - 1);
    tAnioResumen* aux;

    // Primer anio sin variacion
    i->variacion = 0.0;
    i++;

    while(i <= ult)
    {
        aux = i - 1;

        if (aux->saldo != 0.0) //si el saldo previo es distinto de 0
        {
            i->variacion = ((i->saldo - aux->saldo) / aux->saldo) * 100;
        }
        else
        {
            i->variacion = 0.0;
        }

        i++;
    }
}
// --- Función principal del Punto 4 que coordina todo y muestra los resultados ---
int totalesAnualesVariacion(const Vector *transferencias)
{
    if(vectorCE(transferencias) == 0)
    {
        return SIN_DATOS;
    }

    Vector anios;

    if(!vectorCrear(&anios, sizeof(tAnioResumen)))
    {
        return SIN_MEM;
    }

    int codRet;
    // 1 y 2. Acumulamos y ordenamos los datos desde el vector principal
    codRet = acumularDatosAnuales(transferencias, &anios);

    if(codRet != TODO_OK)
    {
        return codRet;
    }

    // 3 y 4. Calculamos saldos y variaciones interanuales
    calcularSaldosYVariaciones(&anios);
    // Mostramos la tabla por pantalla con el formato correspondiente

    mostrarTotalesAnualesVariacion(&anios, printTotalesAnualesVariacion);

    return TODO_OK;
}

void mostrarTotalesAnualesVariacion(const Vector* anios, Print printTotalesAnualesVariacion)
{
    printf("\n%-6s | %12s | %12s | %12s | %15s\n", "ANIO", "C", "D", "SALDO", "VAR_PCT_SALDO");
    printf("----------------------------------------------------------------------\n");

    vectorMostrar(anios, printTotalesAnualesVariacion);
}

// Punto 5
int rankingPaises(const Vector *transferencias)
{
    Vector paises;
    tTotalOperaciones paisAux;
    char operacion;
    int n;
    size_t totalPaises;
    int i;
    int cantidadElem = (int)vectorCE(transferencias);//reemplazo transferencias->ce por cantidadElem en el siguiente if y en el for

    if (transferencias == NULL)
    {
        printf("ERROR - El puntero a transferencias es nulo.\n");
        return SIN_DATOS;
    }

    if(vectorCE(transferencias)==0)
    {
        printf("ERROR: No hay datos de transferencias cargados en memoria.\n");
        return SIN_DATOS;
    }

//Valido e ingreso el tipo de operacion
    do
    {
        printf("Ingrese el tipo de operacion ('C' para Credito, 'D' para Debito): ");
        scanf(" %c", &operacion);

        operacion = toupper(operacion);
    } while (operacion != 'C' && operacion != 'D');


    if(!vectorCrear(&paises, sizeof(tTotalOperaciones)))
        return SIN_MEM;

    for (i = 0; i < cantidadElem; i++) //Acumulo registros por pais
    {
        const tPersonales *t = (tPersonales*)vectorGetElementoDePos(transferencias, i);//casteo el void* por la estructura correcta

        int pos = vectorDesordBuscar(&paises, t, compararCodigoPaisTotales);

        if (pos == -1)
        {
            strcpy(paisAux.codigoPais, t->codigo_pais);
            strcpy(paisAux.descripcionPais, t->descripcion_pais);
            paisAux.cantidadRegistros = 1;
            paisAux.totalCreditos = (t->operacion == 'C') ? t->monto : 0;
            paisAux.totalDebitos = (t->operacion == 'D') ? t->monto : 0;

            int codRet = vectorInsertarAlFinal(&paises, &paisAux);
            if(codRet != TODO_OK)
                return codRet;
        }
        else
        {
            tTotalOperaciones *paisExistente = vectorGetElementoDePos(&paises, pos);
            paisExistente->cantidadRegistros++;

            if (t->operacion == 'C')
                paisExistente->totalCreditos += t->monto;
            else if (t->operacion == 'D')
                paisExistente->totalDebitos += t->monto;
        }
    }

    totalPaises = vectorCE(&paises);

    if (totalPaises == 0)
    {
        printf("No se encontraron registros de paises para procesar.\n");
        vectorDestruir(&paises);
        return SIN_DATOS;
    }

    printf("\n=============================================================================================================\n");
    printf("----------------------------------------- RANKING DE PAISES -----------------------------------------");
    printf("\n=============================================================================================================\n\n");

    n = ingresarEnteroRango(1, (int)totalPaises, "Ingrese la cantidad N de paises a listar: ");
    puts("");

    //Comienzo a ordenar
    if (operacion == 'C')
        vectorOrdenar(&paises, INSERCION, compararCreditoPaisDesc);
    else
        vectorOrdenar(&paises, INSERCION, compararDebitoPaisDesc);


    printf("%-10s | %-50s | %10s | %8s | %8s\n", "PAIS_COD", "PAIS_DESC", "REGISTROS", "TOTAL_CREDITO", "TOTAL_DEBITO");
    printf("-------------------------------------------------------------------------------------------------------------\n");

    for(i=0; i < (size_t)n; i++)
    {
        printResumen(vectorGetElementoDePos(&paises, i));
    }

    vectorDestruir(&paises);
    return TODO_OK;

}

// Punto 6
int participacionPorContinente(const char *pathIntermedio)
{
    FILE *pIntermedio = fopen(pathIntermedio, "rt");

    if(pIntermedio == NULL)
    {
        printf("Error: no se pudo abrir %s\n", pathIntermedio);
        return ACCESO_ARCH_FALLIDO;
    }

    Vector partContinentesAcum; // vector resultante
    float totalCred = 0;
    float totalDeb = 0;
    int codRet;

    if(!vectorCrear(&partContinentesAcum, sizeof(tParticipacionContAcum)))
    {
        fclose(pIntermedio);
        return SIN_MEM;
    }

    char linea[256];
    int pos;
    tIntermedio objInter;
    tParticipacionContAcum objPart;

    fgets(linea, sizeof(linea), pIntermedio);

    while(fgets(linea, sizeof(linea), pIntermedio) != NULL)
    {
        armaIntermedio(linea, &objInter);

        pos = vectorDesordBuscar(&partContinentesAcum, &objInter, compararContinenteParticipacion);

        if(pos == -1)
        {
            strcpy(objPart.continente, objInter.continente);
            objPart.acumCred = 0;
            objPart.acumDeb = 0;

            if(objInter.operacion == 'C')
                objPart.acumCred += objInter.monto;

            if(objInter.operacion == 'D')
                objPart.acumDeb += objInter.monto;

            codRet = vectorInsertarAlFinal(&partContinentesAcum, &objPart);

            if(codRet != TODO_OK)
            {
                vectorDestruir(&partContinentesAcum);
                fclose(pIntermedio);
                return codRet;
            }
        }
        else
        {
            tParticipacionContAcum *partCont = vectorGetElementoDePos(&partContinentesAcum, pos);

            if(objInter.operacion == 'C')
            {
                partCont->acumCred += objInter.monto;
            }
            if(objInter.operacion == 'D')
            {
                partCont->acumDeb += objInter.monto;
            }
        }

        if(objInter.operacion == 'C')
        {
            totalCred += objInter.monto;
        }

        if(objInter.operacion == 'D')
        {
            totalDeb += objInter.monto;
        }

    }

    fclose(pIntermedio);

    int cantElem = (int)vectorCE(&partContinentesAcum);

    Vector partContinentesBin;

    if(!vectorCrear(&partContinentesBin, sizeof(tParticipacionContBin)))
    {
        vectorDestruir(&partContinentesAcum);
        return SIN_MEM;
    }

    tParticipacionContAcum* miPartCont;
    tParticipacionContBin partContFinal;

    for(int i = 0; i < cantElem; i++)
    {
        miPartCont = vectorGetElementoDePos(&partContinentesAcum, i);
        strcpy(partContFinal.continente, miPartCont->continente);

        if(miPartCont->acumCred != 0 && totalCred != 0)
        {
            partContFinal.porcPartCred = (miPartCont->acumCred / totalCred ) * 100;
        }
        else
        {
            partContFinal.porcPartCred = 0.0;
        }

        if(miPartCont->acumDeb != 0 && totalDeb != 0)
        {
            partContFinal.porcPartDeb = (miPartCont->acumDeb / totalDeb ) * 100;
        }
        else
        {
            partContFinal.porcPartDeb = 0.0;
        }

        codRet = vectorOrdInsertar(&partContinentesBin, &partContFinal, compararContinenteAlfa);

        if(codRet != TODO_OK)
        {
            vectorDestruir(&partContinentesBin);
            return codRet;
        }
    }

    mostrarResulPartCont(&partContinentesBin, printPartCont);

    exportar_vec_a_binario("participacion_continentes.dat", &partContinentesBin);

    return TODO_OK;
}

void mostrarResulPartCont(const Vector* partContinentesBin, Print printPartCont)
{
    printf("%-20s | %8s | %8s\n", "CONTINENTE", "CREDITO", "DEBITO");
    printf("--------------------------------------------\n");
    vectorMostrar(partContinentesBin, printPartCont);
}

// Punto 7
int generarArchivoIntermedio(const char *path_intermedio, const char *path_continentes, const Vector* transferencias)
{
    Vector continentes;

    if(!vectorCrear(&continentes, sizeof(tContinentes)))
    {
        return SIN_MEM;
    }

    int codRet = cargarArchivoEnVector(path_continentes, &continentes, armaContinentes);

    if(codRet != TODO_OK)
        return codRet;

    FILE *p_intermedio = fopen(path_intermedio, "wt");

    if(p_intermedio == NULL)
    {
        vectorDestruir(&continentes);
        printf("\nError: no se pudo abrir %s\n", path_intermedio);
        return ACCESO_ARCH_FALLIDO;
    }

    fprintf(p_intermedio, "ANIO;TRIMESTRE;PAIS_COD;PAIS_DESC;CONTINENTE;OPERACION;MONTO\n");

    int cantidad = (int)vectorCE(transferencias);

    if(cantidad == 0)
    {
        vectorDestruir(&continentes);
        return SIN_DATOS;
    }

    for(int i = 0 ; i < cantidad ; i++)
    {
        tPersonales *transferencia = vectorGetElementoDePos(transferencias, i);

        int pos = vectorOrdBuscar(&continentes, transferencia, compararCodigoPais);

        if(pos == -1)
            printf("\nNo se pudo procesar la transferencia %d\n", i + 1);
        else
        {
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
    }

    fclose(p_intermedio);
    vectorDestruir(&continentes);

    printf("\nArchivo generado: %s\n\n", path_intermedio);

    return TODO_OK;
}

// Punto 8
void mostrarResulPromTrimPais(const Vector* miVecResul, Print printResulTrim)
{
    printf("%-10s | %-50s | %9s | %9s | %8s\n", "COD_PAIS", "DESC_PAIS", "OPERACION", "CANT_TRIM", "PROMEDIO");
    printf("----------------------------------------------------------------------------------------------------\n");
    vectorMostrar(miVecResul, printResulTrim);
}

int promedioTrimestralPorPais(const char *path_intermedio)
{
    Vector miVecInter;
    Vector miVecResul;

    if(!vectorCrear(&miVecInter, sizeof(tIntermedio)))
    {
        return SIN_MEM;
    }

    if(!vectorCrear(&miVecResul, sizeof(tResulPromPais)))
    {
        return SIN_MEM;
    }

    int codRet = cargarArchivoEnVector(path_intermedio, &miVecInter, armaIntermedio);

    if(codRet != TODO_OK)
        return codRet;

    if(vectorCE(&miVecInter) == 0)
    {
        vectorDestruir(&miVecInter);
        vectorDestruir(&miVecResul);
        return SIN_DATOS;
    }

    // Ordenamiento por codigo de pais
    vectorOrdenar(&miVecInter, FUSION, compararPaisIntermedio);

    int contTrimDeb;
    int contTrimCred;
    float acumTrimDeb;
    float acumTrimCred;

    tResulPromPais miObjResul;
    char codPais[TAM_COD + 1];

    tIntermedio* i = vectorGetElementoDePos(&miVecInter, 0);
    tIntermedio* ult = vectorGetElementoDePos(&miVecInter, (int)vectorCE(&miVecInter) - 1);

    while(i <= ult)
    {
        strcpy(codPais, i->codigo_pais);

        contTrimDeb = contTrimCred = 0;
        acumTrimDeb = acumTrimCred = 0.0;

        strcpy(miObjResul.codigo_pais, i->codigo_pais);
        strcpy(miObjResul.descripcion_pais, i->descripcion_pais);

        while(i <= ult && strcmp(i->codigo_pais, codPais) == 0)
        {
            if(i->monto > 0)
            {
                if(i->operacion == 'C')
                {
                    contTrimCred++;
                    acumTrimCred += i->monto;
                }
                else
                {
                    contTrimDeb++;
                    acumTrimDeb += i->monto;
                }

            }

            i++;
        }

        // Cargamos promedios para un pais, en una sola fila

        if(contTrimCred == 0)
        {
            miObjResul.promCredito = 0.0;
        }
        else
        {
            miObjResul.promCredito = (acumTrimCred) / contTrimCred;
        }

        miObjResul.cantTrimCred = contTrimCred;

        if(contTrimDeb == 0)
        {
            miObjResul.promDebito = 0.0;
        }
        else
        {
            miObjResul.promDebito = (acumTrimDeb) / contTrimDeb;
        }

        miObjResul.cantTrimDeb = contTrimDeb;

        codRet = vectorInsertarAlFinal(&miVecResul, &miObjResul);

        if(codRet != TODO_OK)
        {
            vectorDestruir(&miVecInter);
            vectorDestruir(&miVecResul);
            return codRet;
        }
    }

    vectorOrdenar(&miVecResul, FUSION, compararPromCredTrimPais);

    mostrarResulPromTrimPais(&miVecResul, printResulTrim);

    vectorDestruir(&miVecInter);
    vectorDestruir(&miVecResul);

    return TODO_OK;
}

// Punto 9
void mostrarResulPromTrimCont(const Vector* miVecResul, Print printResulTrimCont)
{
    printf("%-6s | %-50s | %9s | %8s\n", "ANIO", "CONTINENTE", "PROM CRED", "PROM DEB");
    printf("------------------------------------------------------------------------------------\n");
    vectorMostrar(miVecResul, printResulTrimCont);
}

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
        vectorDestruir(&miVecInter);
        return SIN_MEM;
    }

    int codRet = cargarArchivoEnVector(path_intermedio, &miVecInter, armaIntermedio);

    if(codRet != TODO_OK)
        return codRet;

    if(vectorCE(&miVecInter) == 0)
    {
        vectorDestruir(&miVecInter);
        vectorDestruir(&miVecResul);
        return SIN_DATOS;
    }

    // Ordenamos por año + continente
    vectorOrdenar(&miVecInter, FUSION, compararContinenteAnio);

    int contTrimDeb;
    int contTrimCred;
    float acumTrimDeb;
    float acumTrimCred;

    tResulPromCont miObjResul;
    char continenteAct[TAM_CONT + 1];
    int anioAct;

    tIntermedio* i =  vectorGetElementoDePos(&miVecInter, 0);
    tIntermedio* ult = vectorGetElementoDePos(&miVecInter, (int)vectorCE(&miVecInter) - 1);


    while(i <= ult)
    {
        anioAct = i->anio; // info de comparacion

        while(i <= ult && anioAct == i->anio)
        {
            strcpy(continenteAct, i->continente); // info de comparacion

            contTrimDeb = contTrimCred = 0;
            acumTrimDeb = acumTrimCred = 0.0;

            while(i <= ult && strcmp(continenteAct, i->continente) == 0 && anioAct == i->anio)//para el caso en donde termine un mismo anio y siga otro con el mismo continente
            {
                if(i->monto > 0)
                {
                    if(i->operacion == 'C')
                    {
                        contTrimCred++;
                        acumTrimCred += i->monto;
                    }
                    else
                    {
                        contTrimDeb++;
                        acumTrimDeb += i->monto;
                    }
                }

                i++;
            }

            strcpy(miObjResul.continente, continenteAct);
            miObjResul.anio = anioAct;

            if(contTrimCred > 0)
            {
                miObjResul.promTrimCred = acumTrimCred / contTrimCred;
            }
            else
            {
                miObjResul.promTrimCred = 0.0;
            }

            if(contTrimDeb > 0)
            {
                miObjResul.promTrimDeb = acumTrimDeb / contTrimDeb;
            }
            else
            {
                miObjResul.promTrimDeb = 0.0;
            }

            codRet = vectorInsertarAlFinal(&miVecResul, &miObjResul);

            if(codRet != TODO_OK)
            {
                vectorDestruir(&miVecInter);
                vectorDestruir(&miVecResul);
                return codRet;
            }
        }
    }

    mostrarResulPromTrimCont(&miVecResul, printResulTrimCont);

    vectorDestruir(&miVecInter);
    vectorDestruir(&miVecResul);

    return TODO_OK;
}
