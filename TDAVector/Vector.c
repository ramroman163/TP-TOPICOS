#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Vector.h"


bool ampliarVector(Vector* v);
void reducirVector(Vector* v);
void ordenarInsercion(Vector* v, Cmp cmp);
void ordenarFusion(Vector* v, Cmp cmp);
const void* buscarOrdRec(const void* li, const void* ls, const void* elem, size_t tamElem, Cmp cmp);
void ordenarFusionRec(void* li, void* ls, void* vIzq, void* vDer, size_t tamElem, Cmp cmp);
void fusion(void* li, void* m, void* ls, void* vIzq, void* vDer, size_t tamElem, Cmp cmp);


bool vectorCrear(Vector* v, size_t tamElem)
{
    v->ce = 0;
    v->cap = 0;
    v->tamElem = 0;

    v->vec = malloc(CAP_INI * tamElem);

    if(v->vec == NULL)
    {
        return false;
    }

    v->cap = CAP_INI;
    v->tamElem = tamElem;

    return true;
}


int vectorOrdInsertar(Vector* v, const void* elem, Cmp cmp)
{
    if(v->ce == v->cap)
    {
        if(!ampliarVector(v))
        {
            return SIN_MEM;
        }
    }

    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    // Reemplazar por función buscar binaria.
    void* i = v->vec;
    while(i <= ult && cmp(elem, i) > 0)
    {
        i += v->tamElem;
    }

    if(i <= ult && cmp(elem, i) == 0)
    {
        return DUPLICADO;
    }

    return vectorInsertarEnPos(v, elem, (i - v->vec) / v->tamElem);
}

int vectorOrdInsertarCDup(Vector* v, const void* elem, Cmp cmp)
{
    if(v->ce == v->cap)
    {
        if(!ampliarVector(v))
        {
            return SIN_MEM;
        }
    }

    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    // Reemplazar por función buscar binaria.
    void* i = v->vec;
    while(i <= ult && cmp(elem, i) >= 0)
    {
        i++;
    }

    return vectorInsertarEnPos(v, elem, (i - v->vec) / v->tamElem);
}

int vectorInsertarAlFinal(Vector* v, const void* elem)
{
    if(v->ce == v->cap)
    {
        if(!ampliarVector(v))
        {
            return SIN_MEM;
        }
    }

    void* dirIns = v->vec + v->ce * v->tamElem;
    memcpy(dirIns, elem, v->tamElem);
    v->ce++;

    return TODO_OK;
}


int vectorInsertarEnPos(Vector* v, const void* elem, int pos)
{
    if(v->ce == v->cap)
    {
        if(!ampliarVector(v))
        {
            return SIN_MEM;
        }
    }

    if(pos < 0 || pos > v->ce)
    {
        return POS_INV;
    }

    void* dirIns = v->vec + pos * v->tamElem;
    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    for(void* i = ult; i >= dirIns; i -= v->tamElem)
    {
        memcpy(i + v->tamElem, i, v->tamElem);
    }

    memcpy(dirIns, elem, v->tamElem);
    v->ce++;

    return TODO_OK;
}

int vectorOrdBuscar(const Vector* v, const void* elem, Cmp cmp)
{
    const void* li = v->vec;
    const void* ls = v->vec + (v->ce - 1) * v->tamElem;
    const void* m;

    bool encontrado = false;
    int comp, pos = -1;

    while(!encontrado && li <= ls)
    {
        m = li + ((ls - li) / v->tamElem / 2) * v->tamElem;

        comp = cmp(elem, m);

        if(comp == 0)
        {
            encontrado = true;
            pos = (m - v->vec) / v->tamElem;
        }

        if(comp < 0)
        {
            ls = m - v->tamElem;
        }

        if(comp > 0)
        {
            li = m + v->tamElem;
        }
    }

    return pos;
}

int vectorOrdBuscarRecursiva(const Vector* v, void* elem, Cmp cmp)
{
    const void* li = v->vec;
    const void* ls = v->vec + (v->ce - 1) * v->tamElem;

    const void* dirElem = buscarOrdRec(li, ls, elem, v->tamElem, cmp);

    if(!dirElem)
    {
        return -1;
    }
    else
    {
        memcpy(elem, dirElem, v->tamElem);
        return (dirElem - v->vec) / v->tamElem;
    }
}


const void* buscarOrdRec(const void* li, const void* ls, const void* elem, size_t tamElem, Cmp cmp)
{
    if(li > ls)
    {
        return NULL;
    }

    const void* m = li + ((ls - li) / tamElem / 2) * tamElem;

    int comp = cmp(elem, m);

    if(comp == 0)
    {
        return m;
    }

    if(comp < 0)
    {
        return buscarOrdRec(li, m - tamElem, elem, tamElem, cmp);
    }
    else
    {
        return buscarOrdRec(m + tamElem, ls, elem, tamElem, cmp);
    }
}


int vectorDesordBuscar(const Vector* v, const void* elem, Cmp cmp)
{
    const void *ult = v->vec + v->ce * v->tamElem;

    for(const void *i = v->vec ; i < ult ; i += v->tamElem)
    {
        if(cmp(elem, i) == 0)
            return (i - v->vec) / v->tamElem;
    }

    return -1;
}


size_t vectorCE(const Vector* v)
{
    return v->ce;
}


bool vectorOrdEliminar(Vector* v, const void* elem, Cmp cmp)
{
    int pos = vectorOrdBuscar(v, elem, cmp);
    return vectorEliminarDePos(v, pos);
}


bool vectorDesordEliminar(Vector* v, const void* elem, Cmp cmp)
{
    int pos = vectorDesordBuscar(v, elem, cmp);
    return vectorEliminarDePos(v, pos);
}

bool vectorEliminarDePos(Vector* v, int pos)
{
    if(pos < 0 || pos >= v->ce)
    {
        return false;
    }

    void* dirElim = v->vec + pos * v->tamElem;
    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    for(void* i = dirElim; i < ult; i += v->tamElem)
    {
        memcpy(i, i + v->tamElem, v->tamElem);
    }

    v->ce--;

    if((float)v->ce / v->cap <= FACTOR_OCUP)
    {
        reducirVector(v);
    }

    return true;
}

void vectorOrdenar(Vector* v, int metodo, Cmp cmp)
{
    switch (metodo)
    {
    case INSERCION:
        ordenarInsercion(v, cmp);
        break;

    case FUSION:
        ordenarFusion(v, cmp);
        break;

    default:
        puts("Metodo de ordenamiento no implementado.");
        break;
    }
}


void ordenarInsercion(Vector* v, Cmp cmp)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    void* elemAIns = malloc(v->tamElem);
    void* j;

    for(void* i = v->vec + v->tamElem ; i <= ult; i += v->tamElem)
    {
        memcpy(elemAIns, i, v->tamElem);

        j = i - v->tamElem;

        while(j >= v->vec && cmp(elemAIns, j) < 0)
        {
            memcpy(j + v->tamElem, j, v->tamElem);

            j -= v->tamElem;
        }

        memcpy(j + v->tamElem, elemAIns, v->tamElem);
    }

    free(elemAIns);
}


void ordenarFusion(Vector* v, Cmp cmp)
{
    void* li = v->vec;
    void* ls = v->vec + (v->ce - 1) * v->tamElem;

    void* m = li + ((ls - li) / v->tamElem / 2) * v->tamElem;

    size_t ceVI = (m - li) / v->tamElem + 1;
    size_t ceVD = (ls - m) / v->tamElem;
    void* vIzq = malloc(ceVI * v->tamElem);
    void* vDer = malloc(ceVD * v->tamElem);

    if(!vIzq || !vDer)
    {
        free(vIzq);
        free(vDer);
        return;
    }

    ordenarFusionRec(li, ls, vIzq, vDer, v->tamElem, cmp);

    free(vIzq);
    free(vDer);
}


void ordenarFusionRec(void* li, void* ls, void* vIzq, void* vDer, size_t tamElem, Cmp cmp)
{
    if(li == ls)
    {
        return;
    }

    void* m = li + ((ls - li) / tamElem / 2) * tamElem;

    ordenarFusionRec(li, m, vIzq, vDer, tamElem, cmp);
    ordenarFusionRec(m + tamElem, ls, vIzq, vDer, tamElem, cmp);

    fusion(li, m, ls, vIzq, vDer, tamElem, cmp);
}


void fusion(void* li, void* m, void* ls, void* vIzq, void* vDer, size_t tamElem, Cmp cmp)
{
    size_t ceVI = (m - li) / tamElem + 1;
    size_t ceVD = (ls - m) / tamElem;

    memcpy(vIzq, li, ceVI * tamElem);
    memcpy(vDer, m + tamElem, ceVD * tamElem);

    void* actIzq = vIzq;
    void* actDer = vDer;
    void* dest = li;

    void* ultIzq = vIzq + (ceVI - 1) * tamElem;
    void* ultDer = vDer + (ceVD - 1) * tamElem;

    int comp;

    while(actIzq <= ultIzq && actDer <= ultDer)
    {
        comp = cmp(actIzq, actDer);

        if(comp == 0)
        {
            memcpy(dest, actIzq, tamElem);
            dest += tamElem;
            actIzq += tamElem;
            memcpy(dest, actDer, tamElem);
            dest += tamElem;
            actDer += tamElem;
        }

        if(comp < 0)
        {
            memcpy(dest, actIzq, tamElem);
            dest += tamElem;
            actIzq += tamElem;
        }

        if(comp > 0)
        {
            memcpy(dest, actDer, tamElem);
            dest += tamElem;
            actDer += tamElem;
        }
    }

    while(actIzq <= ultIzq)
    {
        memcpy(dest, actIzq, tamElem);
        dest += tamElem;
        actIzq += tamElem;
    }

    while(actDer <= ultDer)
    {
        memcpy(dest, actDer, tamElem);
        dest += tamElem;
        actDer += tamElem;
    }
}


void vectorMostrar(const Vector* v, Print print)
{
    const void* ult = v->vec + (v->ce - 1) * v->tamElem;

    for(const void* i = v->vec; i <= ult; i += v->tamElem)
    {
        print(i);
    }
}

void* vectorGetElementoDePos(const Vector *v, int pos)
{
    void *elementoPos = v->vec + pos * v->tamElem;

    return elementoPos;
}


void vectorVaciar(Vector* v)
{
    v->ce = 0;
    v->cap = CAP_INI;
    v->vec = realloc(v->vec, CAP_INI * v->tamElem);
}


void vectorDestruir(Vector* v)
{
    free(v->vec);
    v->vec = NULL;
    v->ce = v->cap = v->tamElem = 0;
}


bool ampliarVector(Vector* v)
{
    size_t nCap = v->cap * FACTOR_INCR;
    int* nVec = realloc(v->vec, nCap * v->tamElem);

    if(!nVec)
    {
        return false;
    }

    v->vec = nVec;
    v->cap = nCap;

    return true;
}


void reducirVector(Vector* v)
{
    size_t nCap = v->cap * FACTOR_DECR;

    if(nCap < CAP_INI)
    {
        return;
    }

    v->vec = realloc(v->vec, nCap * v->tamElem);
    
    v->cap = nCap;
}

void vectorCopiar(Vector* dest, const Vector* orig)
{
    if(!vectorCrear(dest, orig->tamElem));
        return SIN_MEM;

    dest->ce = orig->ce;
    dest->cap = orig->cap;

    dest->vec = realloc(dest->vec, orig->cap * orig->tamElem);

    memcpy(dest->vec, orig->vec, orig->ce * orig->tamElem);
}