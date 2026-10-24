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