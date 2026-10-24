#ifndef CALCULOSCONTABLES_H_INCLUDED
#define CALCULOSCONTABLES_H_INCLUDED

#include "../TDAVector/Vector.h"

void totalesPorPais();
void totalesPorContinente(const char *path_intermedio);
void saldoTrimestralPorPais();
void totalesAnualesVariacion();
void rankingPaises();
void participacionPorContinente();
void generarArchivoIntermedio(const char *path_intermedio, const char *path_continentes, const Vector* transferencias);
void promedioTrimestralPorPais();
void promedioTrimestralPorContinente();
bool verificarExistenciaArchivoIntermedio(const char *path_intermedio);
void promedioTrimestralPorPais(const char *path_intermedio);
void mostrarResulPromTrim(const Vector* miVecResul, Print printResulPromTrim);

        
#endif // CALCULOSCONTABLES_H_INCLUDED
