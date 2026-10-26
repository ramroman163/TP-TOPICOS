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

// Punto 7

int generarArchivoIntermedio(const char *path_intermedio, const char *path_continentes, const Vector* transferencias)
{
    Vector continentes;

    if(!vectorCrear(&continentes, sizeof(tContinentes)))
    {
        return SIN_MEM;
    }

    cargarArchivoEnVector(path_continentes, &continentes, armaContinentes);

    FILE *p_intermedio = fopen(path_intermedio, "wt");

    if(p_intermedio == NULL)
    {
        printf("Error: no se pudo abrir %s\n", path_intermedio);
        return ACCESO_ARCH_FALLIDO;
    }

    fprintf(p_intermedio, "ANIO;TRIMESTRE;PAIS_COD;PAIS_DESC;CONTINENTE;OPERACION;MONTO\n");

    int cantidad = (int)vectorCE(transferencias);

    if(cantidad == 0)
    {
        return SIN_DATOS;
    }

    for(int i = 0 ; i < cantidad ; i++)
    {
        tPersonales *transferencia = vectorGetElementoDePos(transferencias, i);

        int pos = vectorOrdBuscar(&continentes, transferencia, compararCodigoPais);

        if(pos == -1)
            printf("No se pudo procesar la transferencia %d\n", i + 1);
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
                return codRet;
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

    return TODO_OK;
}

// Punto 1
void ordenarTransferenciasxCod(Vector* transferencias)
{
    vectorOrdenar(transferencias, FUSION, compararTransferenciasChar);
}

void ordenarTransferenciasxCredito(Vector* transferencias)
{
    vectorOrdenar(transferencias, FUSION, compararCreditoPaisDesc);
}

int contaryCargarOperaciones(const Vector* vTransfercias, Vector* vTransfResumen)
{
    int cantidadElem = (int)vectorCE(vTransfercias);

    if(cantidadElem == 0)
    {
        return SIN_DATOS;
    }

    tPersonales* i = vectorGetElementoDePos(vTransfercias, 0);
    tPersonales* ult = vectorGetElementoDePos(vTransfercias, cantidadElem - 1);
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
        strcpy(opResumen.descripcionPais, descPaisActual);
        opResumen.cantidadRegistros = contRegistros;
        opResumen.totalCreditos = acumCreditos;
        opResumen.totalDebitos = acumDebitos;

        //guarda en pos 0 e incrementa ce en uno, una vez que encuentra otro país, cargo todo en la prox pos

        int codRet = vectorInsertarEnPos(vTransfResumen, &opResumen, vTransfResumen->ce);

        if(codRet != TODO_OK)
            return codRet;
    }

    return TODO_OK;
}

void mostrarVectorTransfResumen(const Vector* v, Print printResumen)
{
    printf("\n");
    printf("%-10s  %-30s  %-10s  %-15s  %-15s\n", "PAIS_COD", "PAIS_DESC", "registros", "total_credito", "total_debito");
    vectorMostrar(v, printResumen);
}

int totalesPorPais(const Vector *vTransferencias)
{
    Vector copiaTransferencias; //para no reordenar el vector principal

    vectorCopiar(&copiaTransferencias, vTransferencias);

    if(vectorCE(&copiaTransferencias) == 0)
    {
        return SIN_DATOS;
    }

    ordenarTransferenciasxCod(&copiaTransferencias);

    Vector vResumen; //para  guardar la nueva estructura en un vector

    if(!vectorCrear(&vResumen, sizeof(tTotalOperaciones)))
        return SIN_MEM;

    //hago corte de control
    contaryCargarOperaciones(&copiaTransferencias, &vResumen);

    ordenarTransferenciasxCredito(&vResumen);

    mostrarVectorTransfResumen(&vResumen, printResumen);

    vectorDestruir(&copiaTransferencias);
    vectorDestruir(&vResumen);

    return TODO_OK;
}

// Punto 3
// Filtro las transferencias del año ingresado
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
        }
        while(i < ult && compararPaisNombre(pais, i) == 0);
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
        }
        while(i < ult && compararPaisNombre(pais, i) == 0);

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
        printResumen(vectorGetElementoDePos(&paises, i));
    }

    vectorDestruir(&paises);
    return TODO_OK;

}

// Punto 8
void mostrarResulPromTrimPais(const Vector* miVecResul, Print printResulTrim)
{
    printf("COD_PAIS - NOMBRE_PAIS - TIPO OPERACION - CANT TRIMESTRES - PROMEDIO\n");
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

    if(!vectorCrear(&miVecResul, sizeof(tResulProm)))
    {
        return SIN_MEM;
    }

    cargarArchivoEnVector(path_intermedio, &miVecInter, armaIntermedio);

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
    int codRet;

    tIntermedio actObjInt;
    tResulProm miObjResul;
    char codPais[3];

    tIntermedio* i = vectorGetElementoDePos(&miVecInter, 0);
    tIntermedio* ult = vectorGetElementoDePos(&miVecInter, (int)vectorCE(&miVecInter) - 1);

    actObjInt = *i;

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

            i++;

            if(i <= ult)
            {
                actObjInt = *i;
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

        codRet = vectorInsertarAlFinal(&miVecResul, &miObjResul);

        if(codRet != TODO_OK)
        {
            vectorDestruir(&miVecInter);
            vectorDestruir(&miVecResul);
        return codRet;
        }
    }

    // ordenamiento va aca

    mostrarResulPromTrimPais(&miVecResul, printResulTrim);

    vectorDestruir(&miVecInter);
    vectorDestruir(&miVecResul);

    return TODO_OK;
}


// Punto 9
void mostrarResulPromTrimCont(const Vector* miVecResul, Print printResulTrimCont)
{
    printf("\nANIO | CONTINENTE | PROM CRED | PROM DEB\n");
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

    cargarArchivoEnVector(path_intermedio, &miVecInter, armaIntermedio);

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
    int codRet;

    tIntermedio miObjAct;
    tResulPromCont miObjResul;
    char continenteAct[20];
    int anioAct;

    tIntermedio* i =  vectorGetElementoDePos(&miVecInter, 0);
    tIntermedio* ult = vectorGetElementoDePos(&miVecInter, (int)vectorCE(&miVecInter) - 1);

    miObjAct = *i;

    while(i <= ult)
    {
        anioAct = miObjAct.anio; // info de comparacion

        while(i <= ult && anioAct == miObjAct.anio)
        {
            strcpy(continenteAct, miObjAct.continente); // info de comparacion

            contTrimDeb = contTrimCred = 0;
            acumTrimDeb = acumTrimCred = 0.0;

            while(i <= ult && strcmp(continenteAct, miObjAct.continente) == 0 && anioAct == miObjAct.anio)
            {
                if(miObjAct.monto > 0)
                {
                    if(miObjAct.operacion == 'C')
                    {
                        contTrimCred++;
                        acumTrimCred += miObjAct.monto;
                    }
                    else
                    {
                        contTrimDeb++;
                        acumTrimDeb += miObjAct.monto;
                    }
                }

                i++;

                if(i <= ult)
                {
                    miObjAct = *i;
                }
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
    printf("\n%-6s | %-12s | %-12s | %-12s | %-15s\n", "ANIO", "C", "D", "SALDO", "VAR_PCT_SALDO");
    printf("-----------------------------------------------------------------------\n");

    vectorMostrar(anios, printTotalesAnualesVariacion);
}
