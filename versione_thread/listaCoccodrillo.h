#ifndef LISTACOCCODRILLO_H
#define LISTACOCCODRILLO_H

#include <stdlib.h>
#include <stdbool.h>
#include "struttureDati.h"
#include "thread.h"

// Dati per ogni singolo coccodrillo
typedef struct {
    pthread_t id;
    Posizione posAttuale;
    Flusso flusso;
} Coccodrillo;

// Nodo Coccodrillo per una lista doppiamente concatenata
typedef struct nodoCoccodrillo {
    struct nodoCoccodrillo* precedente;
    struct nodoCoccodrillo* successivo;
    Coccodrillo dato;
}NodoCoccodrillo;

// Lista di Coccodrillo con testa e coda
typedef struct {
    NodoCoccodrillo* testa;
    NodoCoccodrillo* coda;
}ListaCoccodrillo;

Coccodrillo assegnaDatiCoccodrillo(pthread_t id, Posizione posAttuale, Flusso flusso);

NodoCoccodrillo* popCoccodrillo(ListaCoccodrillo* lista);
void pushCoccodrillo(ListaCoccodrillo* lista, NodoCoccodrillo* nodo);
void scollegaCoccodrillo(NodoCoccodrillo* nodo);
bool listaVuotaCoccodrillo(ListaCoccodrillo lista);
ListaCoccodrillo* creaListaVuotaCoccodrillo();
NodoCoccodrillo* creaNodoCoccodrillo(Coccodrillo dato);

void controllaSpawnCoccodrilli(int n, ListaCoccodrillo* lista[n], Flusso flussi[n], TuttoBuffer* buffer);
void aggiornaPosInListaCoccodrilli(Messaggio messaggio, int n, Flusso flussi[n], ListaCoccodrillo* lista[n]);

#endif /* LISTACOCCODRILLO_H */