#ifndef CALCULOSCONTABLES_H_INCLUDED
#define CALCULOSCONTABLES_H_INCLUDED

#include "../TDAVector/Vector.h"
#include "../Tipos/tipos.h"

void totalesPorPais(const Vector *vTransferencias);
void totalesPorContinente(const char *path_intermedio);
void saldoTrimestralPorPais(const Vector* transferencias);
void totalesAnualesVariacion();
void rankingPaises(const Vector *transferencias);
void participacionPorContinente();
void generarArchivoIntermedio(const char *path_intermedio, const char *path_continentes, const Vector* transferencias);
int promedioTrimestralPorPais(const char *pathIntermedio);
int promedioTrimestralPorContinente(const char *pathIntermedio);
void mostrarResulPromTrimPais(const Vector* miVecResul, Print printResulPromTrim);
void mostrarResulPromTrimCont(const Vector* miVecResul, Print printResulTrimCont);
bool verificarExistenciaArchivoIntermedio(const char *pathIntermedio);

void ordenarTransferenciasxCod(Vector* transferencias);
void ordenarTransferenciasxCredito(Vector* transferencias);
int contaryCargarOperaciones(const Vector* vTransfercias, Vector* vTransfResumen);
void mostrarVectorTransfResumen(const Vector* v, Print printResumen);
int compararPaisNombre(const void* a, const void* b);

#endif // CALCULOSCONTABLES_H_INCLUDED
