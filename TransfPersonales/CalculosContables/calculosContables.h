#ifndef CALCULOSCONTABLES_H_INCLUDED
#define CALCULOSCONTABLES_H_INCLUDED

#include "../TDAVector/Vector.h"
#include "../Tipos/tipos.h"


int totalesPorPais(const Vector *vTransferencias);
void ordenarTransferenciasxCod(Vector* transferencias);
void ordenarTransferenciasxCredito(Vector* transferencias);
int contaryCargarOperaciones(const Vector* vTransfercias, Vector* vTransfResumen);
void mostrarVectorTransfResumen(const Vector* v, Print printResumen);
int totalesPorContinente(const char *path_intermedio);
int saldoTrimestralPorPais(const Vector* transferencias);
int calcularSaldosTrimestrales(const Vector* vAnio);
int filtrarTransferenciasAnio(const Vector* transferencias, Vector* vAnio, int anio);
int totalesAnualesVariacion(const Vector *transferencias);
int rankingPaises(const Vector *transferencias);
int participacionPorContinente(const char *pathIntermedio);
int generarArchivoIntermedio(const char *path_intermedio, const char *path_continentes, const Vector* transferencias);
int promedioTrimestralPorPais(const char *pathIntermedio);
int promedioTrimestralPorContinente(const char *pathIntermedio);
void mostrarResulPromTrimPais(const Vector* miVecResul, Print printResulPromTrim);
void mostrarResulPromTrimCont(const Vector* miVecResul, Print printResulTrimCont);
bool verificarExistenciaArchivoIntermedio(const char *pathIntermedio);
void mostrarTotalesAnualesVariacion(const Vector* anios, Print printTotalesAnualesVariacion);

#endif
