#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "../Tipos/tipos.h"
#include "CodigosRetorno.h"

int ingresarEntero(char *mensaje)
{
    int numero;
    printf("%s", mensaje);
    scanf("%d", &numero);
    return numero;
}

int ingresarEnteroRango(int minimo, int maximo, char *mensaje)
{
    int numero;
    printf("%s", mensaje);
    scanf("%d", &numero);
    while(numero < minimo || numero > maximo)
    {
        printf("Error. Ingrese un valor entre %d y %d: ", minimo, maximo);
        scanf("%d", &numero);
    }
    return numero;
}

float ingresarFloat(char *mensaje)
{
    float numero;
    printf("%s", mensaje);
    scanf("%f", &numero);
    return numero;
}

int ingresarEnteroPositivo(char *mensaje)
{
    int numero;
    printf("%s", mensaje);
    scanf("%d", &numero);
    while(numero < 0)
    {
        printf("Error. Ingrese un numero positivo: ");
        scanf("%d", &numero);
    }
    return numero;
}

float ingresarFloatPositivo(char *mensaje)
{
    float numero;
    printf("%s", mensaje);
    scanf("%f", &numero);
    while(numero < 0)
    {
        printf("Error. Ingrese un numero positivo: ");
        scanf("%f", &numero);
    }
    return numero;
}

void limpiar_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void armaTransfPersonales(const char *registro, void * elemento)
{
    tPersonales *transferencia = (tPersonales *)elemento;

    sscanf(registro,"%d;%d;%2[^;];%49[^;];%c;%f", &transferencia->anio, &transferencia->trimestre,
            transferencia->codigo_pais, transferencia->descripcion_pais, &transferencia->operacion, &transferencia->monto);
}

void armaIntermedio(const char *registro, void * elemento)
{
    tIntermedio *inter = (tIntermedio *)elemento;

    sscanf(registro,"%d;%d;%2[^;];%49[^;];%19[^;];%c;%f", &inter->anio, &inter->trimestre,
            inter->codigo_pais, inter->descripcion_pais, inter->continente, &inter->operacion, &inter->monto);
}

void armaContinentes(const char *registro, void * elemento)
{
    tContinentes *continente = (tContinentes *)elemento;

    sscanf(registro,"%2[^;];%49[^;];%19[^\r\n;]", continente->codigo_pais, continente->descripcion_pais,
           continente->continente);
}

int compararCodigoPais(const void *a, const void *b)
{
    const tPersonales *transferencia = (const tPersonales*)a;
    const tContinentes *continente = (const tContinentes*)b;

    return strcmp(transferencia->codigo_pais, continente->codigo_pais);
}

int compararContinente(const void *a, const void *b)
{
    const tIntermedio *inter = (const tIntermedio*)a;
    const tClasificacion *clasif = (const tClasificacion*)b;

    return strcmp(inter->continente, clasif->continente);
}

int compararPaisIntermedio(const void *a, const void *b)
{
    const tIntermedio *interA = (const tIntermedio*)a;
    const tIntermedio *interB = (const tIntermedio*)b;

    return strcmp(interA->codigo_pais, interB->codigo_pais);
}

int compararCreditoDesc(const void *a, const void *b)
{
    const tClasificacion *clasif_a = (const tClasificacion*)a;
    const tClasificacion *clasif_b = (const tClasificacion*)b;

    if (clasif_b->total_credito > clasif_a->total_credito)
        return 1;

    if (clasif_b->total_credito < clasif_a->total_credito)
        return -1;

    return 0;
}

int compararPromCredTrimPais(const void* a, const void* b)
{
    const tResulPromPais* resulA = (const tResulPromPais*)a;
    const tResulPromPais* resulB = (const tResulPromPais*)b;

    if(resulA->promCredito > resulB->promCredito)
    {
        return -1;
    }

    if(resulA->promCredito < resulB->promCredito)
    {
        return 1;
    }

    return 0;
}

void printClasificacion(const void *e)
{
    const tClasificacion* clasif = (tClasificacion*)e;
    printf("%s - %d - %.2lf - %.2lf\n", clasif->continente, clasif->registros, clasif->total_credito,
           clasif->total_debito);
}

void printResulTrim(const void *e)
{
    const tResulPromPais* promTrim = (tResulPromPais*)e;
    printf("%-10s  %-25s  %3c  %8d  %15.2f\n", promTrim->codigo_pais, promTrim->descripcion_pais, 'C', promTrim->cantTrimCred, promTrim->promCredito);
    printf("%-10s  %-25s  %3c  %8d  %15.2f\n", promTrim->codigo_pais, promTrim->descripcion_pais, 'D', promTrim->cantTrimDeb, promTrim->promDebito);
}

int compararContinenteAnio(const void* a, const void* b)
{
    const tIntermedio* interA = (const tIntermedio*)a;
    const tIntermedio* interB = (const tIntermedio*)b;

    if(interA->anio > interB->anio)
    {
        return 1;
    }

    if(interA->anio < interB->anio)
    {
        return -1;
    }

    return strcmp(interA->continente, interB->continente);
}


int compararCodigoPaisTotales(const void *a, const void *b)
{
    const tPersonales *transf = (const tPersonales *)a;
    const tTotalOperaciones *pais = (const tTotalOperaciones *)b;

    return strcmp(transf->codigo_pais, pais->codigoPais);

}

int compararCreditoPaisDesc(const void *a, const void *b)
{
    const tTotalOperaciones *pais_a = (const tTotalOperaciones *)a;
    const tTotalOperaciones *pais_b = (const tTotalOperaciones *)b;

    if (pais_b->totalCreditos > pais_a->totalCreditos)
        return 1;

    if (pais_b->totalCreditos < pais_a->totalCreditos)
        return -1;

    return 0;
}

int compararDebitoPaisDesc(const void *a, const void *b)
{
    const tTotalOperaciones *pais_a = (const tTotalOperaciones *)a;
    const tTotalOperaciones *pais_b = (const tTotalOperaciones *)b;

    if (pais_b->totalDebitos > pais_a->totalDebitos)
        return 1;

   if (pais_b->totalDebitos < pais_a->totalDebitos)
        return -1;

   return 0;
}


int compararTransferenciasChar(const void* a, const void* b)
{
    const tPersonales* op1 = (const tPersonales*)a;
    const tPersonales* op2 = (const tPersonales*)b;

    return strcmp(op1->codigo_pais, op2->codigo_pais);
}

void printResumen(const void *e)
{
    const tTotalOperaciones* resumen = (tTotalOperaciones*)e;
    printf("%-10s  %-30s  %-10d  %15.2f  %15.2f\n", resumen->codigoPais, resumen->descripcionPais,
        resumen->cantidadRegistros, resumen->totalCreditos, resumen->totalDebitos);
}

void printResulTrimCont(const void *e)
{
    const tResulPromCont* resul = (tResulPromCont*)e;

    printf("%4d | %-30s | %.2f | %.2f\n", resul->anio, resul->continente, resul->promTrimCred, resul->promTrimDeb);
}

void printTotalesAnualesVariacion(const void *e)
{
    const tAnioResumen* resul = (tAnioResumen*)e;

    if(resul->variacion == 0.0)
    {
        printf("%-6d | %12.2f | %12.2f | %12.2f | NA\n", resul->anio, resul->totalCredito, resul->totalDebito, resul->saldo);
    }
    else
    {
        printf("%-6d | %12.2f | %12.2f | %12.2f | %15.2f\n", resul->anio, resul->totalCredito, resul->totalDebito, resul->saldo, resul->variacion);
    }
}

int compararPaisNombre(const void* a, const void* b)
{
    const tPersonales* p1 = (const tPersonales*) a;
    const tPersonales* p2 = (const tPersonales*) b;
   
    return strcmp(p1->descripcion_pais, p2->descripcion_pais);
}

void verificarCodRetInsertar(int cod)
{
    switch(cod)
    {
        case TODO_OK:
            break;

        case LLENO:
            puts("Vector lleno, reintente la ejecucion.");
            break;

        case DUPLICADO:
            puts("Elemento no insertado. Motivo: Duplicado.");
            break;

        case POS_INV:
            puts("Elemento/s no insertado/s. Motivo: Posición inválida.");
            break;

        case SIN_MEM:
            puts("Sin memoria, reintente la ejecucion.");
            break;

        case SIN_DATOS:
            puts("Sin datos disponibles, reintente la ejecucion.");
            break;

        case ACCESO_ARCH_FALLIDO:
            puts("Acceso fallido a archivo, reintente la ejecucion.");
            break;

        default:
            printf("Código %d desconocido.\n", cod);
    }
}