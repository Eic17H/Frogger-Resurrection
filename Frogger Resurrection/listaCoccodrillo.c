#include "listaCoccodrillo.h"

// Rimuove la testa della lista e la restituisce, restituisce NULL se la lista è vuota
NodoCoccodrillo* popCoccodrillo(ListaCoccodrillo* lista) {
    if(listaVuotaCoccodrillo) return NULL;
    NodoCoccodrillo* temp = malloc(sizeof(NodoCoccodrillo));
    lista->testa = lista->testa->successivo;
    if(lista->testa == NULL) lista->coda = NULL;
    else lista->testa->precedente = NULL;
    scollega(temp);
    return temp;
}

// Inserisce un nodo in una lista
void pushCoccodrillo(ListaCoccodrillo* lista, NodoCoccodrillo* nodo) {
    scollega(nodo);
    if(listaVuotaCoccodrillo(*lista)){
        lista->testa = lista->coda = nodo;
    } else {
        lista->coda->successivo = nodo;
        nodo->precedente = lista->coda;
        lista->coda = nodo;
    }
}

// Scollega un nodo da qualunque lista
void scollegaCoccodrillo(NodoCoccodrillo* nodo) {
    nodo->precedente = NULL;
    nodo->successivo = NULL;
}

// Restituisce true se la lista è vuota, false altrimenti
bool listaVuotaCoccodrillo(ListaCoccodrillo lista) {
    if((lista.testa==NULL)&&(lista.coda==NULL)) {
        return true;
    } else {
        if(lista.testa==NULL) lista.testa = lista.coda;
        else lista.coda = lista.testa;
        return false;
    }
}

// Crea una lista vuota di tipo Coccodrillo
ListaCoccodrillo* creaListaVuotaCoccodrillo() {
    ListaCoccodrillo* lista = malloc(sizeof(ListaCoccodrillo));
    lista->testa = NULL;
    lista->coda = NULL;
    return lista;
}

// Crea un nodo di tipo Coccodrillo, scollegato da altri nodi
NodoCoccodrillo* creaNodoCoccodrillo(Coccodrillo dato){
    NodoCoccodrillo* nodo = malloc(sizeof(NodoCoccodrillo));
    nodo->dato = dato;
    nodo->precedente = NULL;
    nodo->successivo = NULL;
}