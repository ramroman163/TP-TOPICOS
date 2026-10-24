#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stddef.h>
#include <stdbool.h>
#include "../Utils/CodigosRetorno.h"

#define CAP_INI 8

#define FACTOR_INCR 1.5 // > 1
#define FACTOR_OCUP 0.25 // < 1
#define FACTOR_DECR 0.5 // < 1 && > FACTOR_OCUP

#define INSERCION 1
#define FUSION 2


typedef struct
{
    void* vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}
Vector;


typedef int (*Cmp)(const void* e1, const void* e2);
typedef void (*Print)(const void* e);


bool vectorCrear(Vector* v, size_t tamElem); // malloc
int vectorOrdInsertar(Vector* v, const void* elem, Cmp cmp); // realloc
int vectorOrdInsertarCDup(Vector* v, const void* elem, Cmp cmp); // realloc
int vectorInsertarAlFinal(Vector* v, const void* elem); // realloc
int vectorInsertarEnPos(Vector* v, const void* elem, int pos); // realloc
int vectorOrdBuscar(const Vector* v, void* elem, Cmp cmp);
int vectorOrdBuscarRecursiva(const Vector* v, void* elem, Cmp cmp);
int vectorDesordBuscar(const Vector* v, void* elem, Cmp cmp);
size_t vectorCE(const Vector* v);
bool vectorOrdEliminar(Vector* v, void* elem); // realloc
bool vectorDesordEliminar(Vector* v, void* elem); // realloc
bool vectorEliminarDePos(Vector* v, int pos); // realloc
void vectorOrdenar(Vector* v, int metodo, Cmp cmp);
void vectorMostrar(const Vector* v, Print print);
void vectorVaciar(Vector* v); // realloc
void vectorDestruir(Vector* v); // free


#endif // VECTOR_H_INCLUDED
