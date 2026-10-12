#include <stdio.h>
#include <stdlib.h>
#include "../Menu/menu.h"
#include "../Tipos/tipos.h"
#include "../Archivos/archivos.h"
#include "../Utils/utils.h"
// aca incluir la biblio de tda vector dinamico generico

#define PATH_TRANSFERENCIAS_CSV "transferencias_personales_clean.csv" //si el archivo esta subido en otra carpeta indicar la ruta completa

int main()
{
    Vector transferenciasPersonales;

    vectorCrear(&transferenciasPersonales, sizeof(tPersonales));

    cargarArchivoEnVector(PATH_TRANSFERENCIAS_CSV, &transferenciasPersonales, armaTransfPersonales);
    
    mostrarOpciones();

    vectorDestruir(&transferenciasPersonales);

    return 0;
}
