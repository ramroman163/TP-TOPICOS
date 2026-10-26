#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "../Tipos/tipos.h"
#include "../TDAVector/vector.h"

void mostrarOpciones();
void menuEjecutar(const char *pathIntermedio, const char *pathContinentes, const Vector *transferencias);

#endif // MENU_H_INCLUDED
