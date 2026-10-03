#include <stdio.h>
#include "utils.h"

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