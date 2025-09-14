#ifndef LISTA_THREAD_DA_TERMINARE_H
#define LISTA_THREAD_DA_TERMINARE_H

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    pthread_t tid;
    bool daTerminare;
} ThreadTerminabile;

typedef struct elementoTerminabile{
    ThreadTerminabile dato;
    struct elementoTerminabile* precedente;
    struct elementoTerminabile* successivo;
} ElementoTerminabile;

typedef struct {
    ElementoTerminabile* testa;
    ElementoTerminabile* coda;
} ListaThreadTerminabili;

bool cercaTidInLista (ListaThreadTerminabili lista, pthread_t tid);
void pushNuovoThreadTerminabile (ListaThreadTerminabili lista, pthread_t tid);
void terminaThread(ListaThreadTerminabili lista, pthread_t tid);
bool checkSeEDaIgnorare(ListaThreadTerminabili lista, pthread_t tid);

#endif /* LISTA_THREAD_DA_TERMINARE_H */