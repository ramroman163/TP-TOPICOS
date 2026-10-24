#ifndef CALCULOSCONTABLES_H_INCLUDED
#define CALCULOSCONTABLES_H_INCLUDED

#include "../TDAVector/Vector.h"

void totalesPorPais();
void totalesPorContinente(const char *path_intermedio);
void saldoTrimestralPorPais(const Vector* transferencias);
void totalesAnualesVariacion();
void rankingPaises();
void participacionPorContinente();
void generarArchivoIntermedio(const char *path_intermedio, const char *path_continentes, const Vector* transferencias);
void promedioTrimestralPorPais();
void promedioTrimestralPorContinente();
bool verificarExistenciaArchivoIntermedio(const char *path_intermedio);
void ordenarTransferenciasxCod(Vector* transferencias);
void ordenarTransferenciasxCredito(Vector* transferencias);
void contaryCargarOperaciones(const Vector* vTransfercias, Vector* vTransfResumen);
void mostrarVectorTransfResumen(const Vector* v, Print printResumen);
int compararPaisNombre(const void* a, const void* b);

#endif // CALCULOSCONTABLES_H_INCLUDED
