#include <stdio.h>
#include <stdlib.h>
#include "Menu/menu.h"
#include "Tipos/tipos.h"
#include "Archivos/archivos.h"
#include "Utils/utils.h"
#include "TDAVector/Vector.h"

#define PATH_TRANSFERENCIAS_CSV "transferencias_personales_clean.csv" //si el archivo esta subido en otra carpeta indicar la ruta completa
#define PATH_CONTINENTES_CSV "paises_continentes.csv"
#define PATH_INTERMEDIO_CSV "transferencias_base_larga.csv"

int main()
{
    Vector transferenciasPersonales;

    vectorCrear(&transferenciasPersonales, sizeof(tPersonales));

    cargarArchivoEnVector(PATH_TRANSFERENCIAS_CSV, &transferenciasPersonales, armaTransfPersonales);
    
    menuEjecutar(PATH_INTERMEDIO_CSV, PATH_CONTINENTES_CSV, &transferenciasPersonales);

    vectorDestruir(&transferenciasPersonales);

    return 0;
}
