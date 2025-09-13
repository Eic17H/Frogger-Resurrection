#include "listaCoccodrillo.h"
#include "costanti.h"
#include "inizializzazione.h"
#include "regole.h"

// Inserire i dati di tipo Coccodrillo
Coccodrillo assegnaDatiCoccodrillo(pthread_t id, Posizione posAttuale, Flusso flusso) {
    Coccodrillo c;
    c.id = id; c.posAttuale = posAttuale; c.flusso = flusso;
    return c;
}

// Rimuove la testa della lista e la restituisce, restituisce NULL se la lista è vuota
NodoCoccodrillo* popCoccodrillo(ListaCoccodrillo* lista) {
    if(listaVuotaCoccodrillo(*lista)) return NULL;
    NodoCoccodrillo* temp = malloc(sizeof(NodoCoccodrillo));
    lista->testa = lista->testa->successivo;
    if(lista->testa == NULL) lista->coda = NULL;
    else lista->testa->precedente = NULL;
    scollegaCoccodrillo(temp);
    return temp;
}

// Inserisce un nodo in una lista
void pushCoccodrillo(ListaCoccodrillo* lista, NodoCoccodrillo* nodo) {
    scollegaCoccodrillo(nodo);
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
    return nodo;
}

void aggiornaPosInListaCoccodrilli(Messaggio messaggio, int n, Flusso flussi[n], ListaCoccodrillo* lista[n]) {
    int i = 0;
    NodoCoccodrillo* nodo = NULL;

    i = trovaIndiceFlusso(N_FLUSSI, flussi, messaggio.posAttuale.y);
    if (i == -1) return ;
    nodo = lista[i]->testa;

    // si trova il nodo corrispondente al coccodrillo
    while (nodo->dato.id != messaggio.id) {
        nodo = nodo->successivo;
        if (nodo == NULL) return ;
    }

    // si aggiornano le coordinate
    nodo->dato.posAttuale = messaggio.posAttuale;
}

void controllaSpawnCoccodrilli(int n, ListaCoccodrillo* lista[n], Flusso flussi[n], TuttoBuffer* buffer) {
    NodoCoccodrillo* temp = NULL, *penultimoNodo = NULL;

    for (int i = 0; i < n; i++) {
        // se la testa è più fuori schermo, si elimina 
        if (fuoriSchermo(lista[i]->testa->dato.posAttuale, COCCO, flussi[i].verso)) {
            penultimoNodo = lista[i]->testa->successivo;
            penultimoNodo->precedente = NULL;

            scollegaCoccodrillo(lista[i]->testa);
            temp = lista[i]->testa;
    
            lista[i]->testa = penultimoNodo;

            //TODO: kill(temp->dato.pid, SIGKILL);
            free(temp);
        }
        // se la coda è alla giusta distanza, si crea un nuovo coccodrillo
        if (flussi[i].verso == AVANZAMENTO_DX && lista[i]->coda->dato.posAttuale.x >= flussi[i].distanzaCoccodrilli ||
            flussi[i].verso == AVANZAMENTO_SX && DIM_COLS - lista[i]->coda->dato.posAttuale.x - W_COCCODRILLO >= flussi[i].distanzaCoccodrilli) {
            creaCoccodrillo(lista[i], flussi[i], buffer);
        }
    }
}