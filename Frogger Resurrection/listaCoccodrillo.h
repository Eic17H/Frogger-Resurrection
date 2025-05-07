#ifndef LISTACOCCODRILLO_H
#define LISTACOCCODRILLO_H

#include <stdlib.h>
#include <stdbool.h>
#include "struttureDati.h"

// Nodo Coccodrillo per una lista doppiamente concatenata
typedef struct {
    NodoCoccodrillo* precedente;
    NodoCoccodrillo* successivo;
    Coccodrillo dato;
}NodoCoccodrillo;

// Lista di Coccodrillo con testa e coda
typedef struct {
    NodoCoccodrillo* testa;
    NodoCoccodrillo* coda;
}ListaCoccodrillo;

NodoCoccodrillo* popCoccodrillo(ListaCoccodrillo* lista);
void pushCoccodrillo(ListaCoccodrillo* lista, NodoCoccodrillo* nodo);
void scollegaCoccodrillo(NodoCoccodrillo* nodo);
bool listaVuotaCoccodrillo(ListaCoccodrillo lista);
ListaCoccodrillo* creaListaVuotaCoccodrillo();
NodoCoccodrillo* creaNodoCoccodrillo(Coccodrillo dato);

#endif /* LISTACOCCODRILLO_H */