#ifndef LISTAGRANATE_H
#define LISTAGRANATE_H

#include <stdlib.h>
#include <stdbool.h>
#include "struttureDati.h"

// Dati per ogni singolo Granata
typedef struct {
    pthread_t id;
    Posizione posAttuale;
    //Flusso flusso;
} Granata;

// Nodo Granata per una lista doppiamente concatenata
typedef struct nodoGranata {
    struct nodoGranata* precedente;
    struct nodoGranata* successivo;
    Granata dato;
}NodoGranata;

// Lista di Granata con testa e coda
typedef struct {
    NodoGranata* testa;
    NodoGranata* coda;
}ListaGranata;

Granata assegnaDatiGranata(pthread_t id, Posizione posAttuale);

NodoGranata* popGranata(ListaGranata* lista);
void pushGranata(ListaGranata* lista, NodoGranata* nodo);
void scollegaGranata(NodoGranata* nodo);
bool listaVuotaGranata(ListaGranata lista);
ListaGranata* creaListaVuotaGranata();
NodoGranata* creaNodoGranata(Granata dato);

NodoGranata* trovaNodoGranata(pthread_t id, ListaGranata* lista);
void aggiornaPosInListaGranate(Messaggio messaggio, ListaGranata* lista);
void eliminaNodoGranataFuoriLista(NodoGranata* nodoDaElim, ListaGranata* lista);

#endif /* LISTAGRANATA_H */