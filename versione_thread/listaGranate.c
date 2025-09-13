#include "listaGranate.h"

// Inserire i dati di tipo Granata
Granata assegnaDatiGranata(pthread_t id, Posizione posAttuale) {
    Granata c;
    c.id = id; c.posAttuale = posAttuale; 
    return c;
}

// Rimuove la testa della lista e la restituisce, restituisce NULL se la lista è vuota
NodoGranata* popGranata(ListaGranata* lista) {
    if(listaVuotaGranata(*lista)) return NULL;
    NodoGranata* temp = malloc(sizeof(NodoGranata));
    lista->testa = lista->testa->successivo;
    if(lista->testa == NULL) lista->coda = NULL;
    else lista->testa->precedente = NULL;
    scollegaGranata(temp);
    return temp;
}

// Inserisce un nodo in una lista
void pushGranata(ListaGranata* lista, NodoGranata* nodo) {
    scollegaGranata(nodo);
    if(listaVuotaGranata(*lista)){
        lista->testa = lista->coda = nodo;
    } else {
        lista->coda->successivo = nodo;
        nodo->precedente = lista->coda;
        lista->coda = nodo;
    }
}

// Scollega un nodo da qualunque lista
void scollegaGranata(NodoGranata* nodo) {
    nodo->precedente = NULL;
    nodo->successivo = NULL;
}

// Restituisce true se la lista è vuota, false altrimenti
bool listaVuotaGranata(ListaGranata lista) {
    if((lista.testa==NULL)&&(lista.coda==NULL)) {
        return true;
    } else {
        if(lista.testa==NULL) lista.testa = lista.coda;
        else lista.coda = lista.testa;
        return false;
    }
}

// Crea una lista vuota di tipo Granata
ListaGranata* creaListaVuotaGranata() {
    ListaGranata* lista = malloc(sizeof(ListaGranata));
    lista->testa = NULL;
    lista->coda = NULL;
    return lista;
}

// Crea un nodo di tipo Granata, scollegato da altri nodi
NodoGranata* creaNodoGranata(Granata dato){
    NodoGranata* nodo = malloc(sizeof(NodoGranata));
    nodo->dato = dato;
    nodo->precedente = NULL;
    nodo->successivo = NULL;
    return nodo;
}

void aggiornaPosInListaGranate(Messaggio messaggio, ListaGranata* lista) {
    NodoGranata* nodo = lista->testa;

    // si trova il nodo corrispondente al Granata
    while (nodo->dato.id != messaggio.id && nodo != NULL) {
        nodo = nodo->successivo;
    }

    if (nodo == NULL) return ;
    // si aggiornano le coordinate
    nodo->dato.posAttuale = messaggio.posAttuale;
}
