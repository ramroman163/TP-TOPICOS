#include <stdio.h>
#include "utils.h"
#include "Tipos/tipos.h"

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

    return strcmpi(transferencia->codigo_pais, continente->codigo_pais);
}

int compararContinente(const void *a, const void *b)
{
    const tIntermedio *inter = (const tIntermedio*)a;
    const tClasificacion *clasif = (const tClasificacion*)b;

    return strcmpi(inter->continente, clasif->continente);
}

int compararCreditoDesc(const void *a, const void *b)
{
    const tClasificacion *clasif_a = (const tClasificacion*)a;
    const tClasificacion *clasif_b = (const tClasificacion*)b;

    return clasif_b->total_credito - clasif_a->total_credito;
}

void printClasificacion(const void *e)
{
    const tClasificacion* clasif = (tClasificacion*)e;
    printf("%s - %d - %.2lf - %.2lf\n", clasif->continente, clasif->registros, clasif->total_credito,
           clasif->total_debito);
}

int compararTransferenciasChar(const void* a, const void* b)
{
    const tPersonales* op1 = (const tPersonales*)a;
    const tPersonales* op2 = (const tPersonales*)b;

    return strcmpi(op1->codigo_pais, op2->codigo_pais);
}

int compararOperacionesFloat(const void* a, const void* b)
{
    const tTotalOperaciones* op1 = (const tTotalOperaciones*)a;
    const tTotalOperaciones* op2 = (const tTotalOperaciones*)b;

    if (op2->totalCreditos > op1->totalCreditos)
        return 1;
    if (op2->totalCreditos < op1->totalCreditos)
        return -1;
    
    return 0;
}
void printResumen(const void *e)
{
    const tTotalOperaciones* resumen = (tTotalOperaciones*)e;
    printf("%-10s  %-30s  %-10d  %15.2f  %15.2f\n", resumen->codigoPais, resumen->descripcionPais, 
        resumen->cantidadRegistros, resumen->totalCreditos, resumen->totalDebitos);
}