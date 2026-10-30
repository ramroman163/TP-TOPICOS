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
    int leido;

    printf("%s", mensaje);
    leido = scanf("%d", &numero);
    limpiarBuffer();

    while(leido != 1 || numero < minimo || numero > maximo)
    {
        printf("Error. Ingrese un valor entre %d y %d: ", minimo, maximo);
        leido = scanf("%d", &numero);
        limpiarBuffer();
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

void limpiarBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int armaTransfPersonales(const char *registro, void * elemento)
{
    tPersonales *transferencia = (tPersonales *)elemento;

    char* act = strchr(registro, '\n');

    if(!act)
    {
        return ERROR_LINEA_LARGA;
    }

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    sscanf(act + 1, "%f", &transferencia->monto);

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    sscanf(act + 1, "%c", &transferencia->operacion);

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    strncpy(transferencia->descripcion_pais, act + 1, TAM_DESC);
    transferencia->descripcion_pais[TAM_DESC] = '\0';

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    strncpy(transferencia->codigo_pais, act + 1, TAM_COD);
    transferencia->codigo_pais[TAM_COD] = '\0';

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    sscanf(act + 1, "%d", &transferencia->trimestre);

    *act = '\0';

    if(strlen(registro) == 0)
        return LINEA_FALLADA;

    sscanf(registro, "%d", &transferencia->anio);

    return TODO_OK;
}

int armaIntermedio(const char *registro, void * elemento)
{
    tIntermedio *inter = (tIntermedio *)elemento;

    char* act = strchr(registro, '\n');

    if(!act)
    {
        return ERROR_LINEA_LARGA;
    }

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    sscanf(act + 1, "%f", &inter->monto);

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    sscanf(act + 1, "%c", &inter->operacion);

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    strncpy(inter->continente, act + 1, TAM_CONT);
    inter->continente[TAM_CONT] = '\0';

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    strncpy(inter->descripcion_pais, act + 1, TAM_DESC);
    inter->descripcion_pais[TAM_DESC] = '\0';

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    strncpy(inter->codigo_pais, act + 1, TAM_COD);
    inter->codigo_pais[TAM_COD] = '\0';

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    sscanf(act + 1, "%d", &inter->trimestre);

    *act = '\0';

    if(strlen(registro) == 0)
        return LINEA_FALLADA;

    sscanf(registro, "%d", &inter->anio);

    return TODO_OK;
}

int armaContinentes(const char *registro, void * elemento)
{
    tContinentes *continente = (tContinentes *)elemento;

    char* act = strchr(registro, '\n');

    if(!act)
    {
        return ERROR_LINEA_LARGA;
    }

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    strncpy(continente->continente, act + 1, TAM_CONT);
    continente->continente[TAM_CONT] = '\0';

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(act + 1) == 0)
        return LINEA_FALLADA;

    strncpy(continente->descripcion_pais, act + 1, TAM_DESC);
    continente->descripcion_pais[TAM_DESC] = '\0';

    *act = '\0';
    act = strrchr(registro, ';');

    if(strlen(registro) == 0)
        return LINEA_FALLADA;

    strncpy(continente->codigo_pais, registro, TAM_COD);
    continente->codigo_pais[TAM_COD] = '\0';

    return TODO_OK;
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

int compararContinenteParticipacion(const void *a, const void *b)
{
    const tIntermedio *inter = (const tIntermedio*)a;
    const tParticipacionContAcum *partCont = (const tParticipacionContAcum*)b;

    return strcmp(inter->continente, partCont->continente);
}

int compararContinenteAlfa(const void* a, const void* b)
{
    const tParticipacionContBin *partContA = (const tParticipacionContBin*)a;
    const tParticipacionContBin *partContB = (const tParticipacionContBin*)b;

    return strcmp(partContA->continente, partContB->continente);
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
    printf("%-20s | %10d | %8.2lf | %8.2lf\n", clasif->continente, clasif->registros, clasif->total_credito,
           clasif->total_debito);
}

void printResulTrim(const void *e)
{
    const tResulPromPais* promTrim = (tResulPromPais*)e;
    printf("%-10s | %-50s | %9c | %9d | %8.2f\n", promTrim->codigo_pais, promTrim->descripcion_pais, 'C', promTrim->cantTrimCred, promTrim->promCredito);
    printf("%-10s | %-50s | %9c | %9d | %8.2f\n", promTrim->codigo_pais, promTrim->descripcion_pais, 'D', promTrim->cantTrimDeb, promTrim->promDebito);
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
    printf("%-10s | %-50s | %10d | %13.2f | %12.2f\n", resumen->codigoPais, resumen->descripcionPais,
        resumen->cantidadRegistros, resumen->totalCreditos, resumen->totalDebitos);
}

void printResulTrimCont(const void *e)
{
    const tResulPromCont* resul = (tResulPromCont*)e;

    printf("%-6d | %-50s | %9.2f | %8.2f\n", resul->anio, resul->continente, resul->promTrimCred, resul->promTrimDeb);
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

void printPartCont(const void *e)
{
    const tParticipacionContBin* part = (tParticipacionContBin*)e;

    printf("%-20s | %8.2f | %8.2f\n", part->continente, part->porcPartCred, part->porcPartDeb);
}

void verificarCodRet(int cod)
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

        case ERROR_LINEA_LARGA:
            puts("Error al leer el archivo por estar mal cargado, reintente ejecucion.");
            break;

        case LINEA_FALLADA:
            puts("No se pudo leer una linea y se ha salteado.");
            break;

        default:
            printf("Código %d desconocido.\n", cod);
    }
}
