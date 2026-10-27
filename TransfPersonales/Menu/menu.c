#include <stdio.h>
#include <stdbool.h>
#include "menu.h"
#include "../Utils/utils.h"
#include "../CalculosContables/calculosContables.h"

void mostrarOpciones()
{
    printf("\nTRANSFERENCIAS PERSONALES\n");
    printf("1. Totales por pais\n");
    printf("2. Totales por continente.\n");
    printf("3. Saldo trimestral por pais.\n");
    printf("4. Totales anuales y variacion interanual.\n");
    printf("5. Ranking de paises.\n");
    printf("6. Participacion porcentual por continente. \n");
    printf("7. Generacion archivo intermedio de transferencias extendido.\n");
    printf("8. Promedio trimestral por pais.\n");
    printf("9. Promedio trimestral por continente, operacion y anio.\n");
    printf("10. Salir\n");
}

void menuEjecutar(const char *pathIntermedio, const char *pathContinentes, const Vector *transferencias)
{

    int opcion;
    int codRet;
    bool existe;

    do
    {
        mostrarOpciones();
        opcion = ingresarEnteroRango(1,10,"Opcion: ");

        switch (opcion)
        {
            case 1:
                codRet = totalesPorPais(transferencias); 
                break;
            case 2:
                existe = verificarExistenciaArchivoIntermedio(pathIntermedio);

                if(existe == false)
                {
                    codRet = generarArchivoIntermedio(pathIntermedio, pathContinentes, transferencias);

                }

                codRet = totalesPorContinente(pathIntermedio); 
                break;
            case 3:
                codRet = saldoTrimestralPorPais(transferencias); 
                break;
            case 4:
                codRet = totalesAnualesVariacion(transferencias); 
                break;
            case 5:
                codRet = rankingPaises(transferencias); 
                break;
            case 6:
                //participacionPorContinente(); 
                break;
            case 7:
                codRet = generarArchivoIntermedio(pathIntermedio, pathContinentes, transferencias); 

                break;
            case 8:
                existe = verificarExistenciaArchivoIntermedio(pathIntermedio);

                if(!existe)
                {
                    codRet = generarArchivoIntermedio(pathIntermedio, pathContinentes, transferencias);
                }
                codRet = promedioTrimestralPorPais(pathIntermedio); 
                break;
            case 9:
                codRet = promedioTrimestralPorContinente(pathIntermedio); 
                break;
            case 10:
                printf("Hasta luego.\n"); 
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n"); 
                break;
        }

        if(opcion >= 1 && opcion <= 9)
        {
            verificarCodRet(codRet);
        }
    } while (opcion != 10);
}

