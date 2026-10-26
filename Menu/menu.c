#include <stdio.h>
#include <stdbool.h>
#include "menu.h"
#include "../Utils/utils.h"
#include "../CalculosContables/calculosContables.h"


/*
El menú deberá presentar, como mínimo, las siguientes opciones:
1.	Totales por país.
2.	Totales por continente.
3.	Saldo trimestral por país.
4.	Totales anuales y variación interanual.
5.	Ranking de países.
6.	Participación porcentual por continente.
7.	Generación del archivo intermedio transferencias_base_larga.csv.
8.	Promedio trimestral por país.
9.	Promedio trimestral por continente, operación y año.
10.	Salir.
*/

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

    do
    {
        mostrarOpciones();
        opcion = ingresarEnteroRango(1,10,"Opcion: ");
        bool existe;

        switch (opcion)
        {
            case 1:
                totalesPorPais(transferencias); break;
            case 2:
                existe = verificarExistenciaArchivoIntermedio(pathIntermedio);

                if(existe == false)
                    generarArchivoIntermedio(pathIntermedio, pathContinentes, transferencias);

                totalesPorContinente(pathIntermedio); break;
            case 3:
                saldoTrimestralPorPais(transferencias); break;
            case 4:
                //totalesAnualesVariacion();
                break;
            case 5:
                rankingPaises(transferencias); break;
            case 6:
                //participacionPorContinente();
                break;
            case 7:
                generarArchivoIntermedio(pathIntermedio, pathContinentes, transferencias); break;
            case 8:

                existe = verificarExistenciaArchivoIntermedio(pathIntermedio);

                if(!existe)
                {
                    generarArchivoIntermedio(pathIntermedio, pathContinentes, transferencias);
                }
                promedioTrimestralPorPais(pathIntermedio); break;
            case 9:
                promedioTrimestralPorContinente(pathIntermedio); break;
            case 10:
                printf("Hasta luego.\n"); break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n"); break;
        }
    } while (opcion != 10);
}

