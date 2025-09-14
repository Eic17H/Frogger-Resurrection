#include "listaThreadDaTerminare.h"

bool cercaTidInLista (ListaThreadTerminabili lista, pthread_t tid) {
    if(lista.coda->dato.tid == tid) {
        return true;
    }
    for(ElementoTerminabile* elem = lista.testa; elem->successivo != NULL; elem = elem->successivo) {
        if(elem->dato.tid == tid) {
            return true;
            }
    }
    return false;
}

void pushNuovoThreadTerminabile (ListaThreadTerminabili lista, pthread_t tid) {
    if(cercaTidInLista(lista,tid)) return;
    ElementoTerminabile* thread = malloc(sizeof(ElementoTerminabile));
    thread->dato.tid = tid;
    thread->dato.daTerminare = false;
    thread->precedente = NULL;
    thread->successivo = NULL;
    
    if(lista.testa == NULL && lista.coda == NULL) {
        lista.testa = lista.coda = thread;
    } else {
        lista.coda->successivo = thread;
        lista.coda = thread;
    }
}

void terminaThread(ListaThreadTerminabili lista, pthread_t tid) {
    if(lista.coda->dato.tid == tid) {
        lista.coda->dato.daTerminare = true;
        return;
    }
    for(ElementoTerminabile* elem = lista.testa; elem->successivo != NULL; elem = elem->successivo) {
        if(elem->dato.tid == tid) {
            elem->dato.daTerminare = true;
            return;
            }
    }
}

bool checkSeEDaIgnorare(ListaThreadTerminabili lista, pthread_t tid) {
    if(lista.coda->dato.tid == tid) {
        return lista.coda->dato.daTerminare;
    }
    for(ElementoTerminabile* elem = lista.testa; elem->successivo != NULL; elem = elem->successivo) {
        if(elem->dato.tid == tid) {
            return elem->dato.daTerminare;
            }
    }
    return false;
}