// Data in input una stringa contenente il nome del tipo (per esempio "Coccodrillo"),
// stampa nella console la libreria che ne implementa liste doppiamente concatenate

function creaLibreriaLista(tipo){

    console.log(`============================================lista${tipo}.c - INIZIO============================================`)

    console.log(`
    
#include "lista${tipo}.h"

// Rimuove la testa della lista e la restituisce, restituisce null se la lista è vuota
Nodo${tipo}* pop${tipo}(Lista${tipo}* lista) {
    if(listaVuota${tipo}) return null;
    Nodo${tipo}* temp = malloc(sizeof(Nodo${tipo}));
    lista->testa = lista->testa->successivo;
    if(lista->testa == null) lista->coda = null;
    else lista->testa->precedente = null;
    scollega(temp);
    return temp;
}

// Inserisce un nodo in una lista
void push${tipo}(Lista${tipo}* lista, Nodo${tipo}* nodo) {
    scollega(nodo)
    if(listaVuota${tipo}(lista)){
        lista->testa = lista->coda = nodo;
    } else {
        lista->coda->successivo = nodo;
        nodo->precedente = lista->coda;
        lista->coda = nodo;
    }
}

// Scollega un nodo da qualunque lista
void scollega${tipo}(Nodo${tipo}* nodo) {
    nodo->precedente = null;
    nodo->successivo = null;
}

// Restituisce true se la lista è vuota, false altrimenti
bool listaVuota${tipo}(Lista${tipo} lista) {
    if((lista->testa==null)&&(lista->coda==null)) {
        return true;
    } else {
        if(lista->testa==null) lista->testa = lista->coda;
        else lista->coda = lista->testa;
        return false;
    }
}

// Crea una lista vuota di tipo ${tipo}
Lista${tipo}* creaListaVuota${tipo}() {
    Lista${tipo}* lista = malloc(sizeof(Lista${tipo}));
    lista->testa = null;
    lista->coda = null;
    return lista;
}

// Crea un nodo di tipo ${tipo}, scollegato da altri nodi
Nodo${tipo}* creaNodo${tipo}(${tipo} dato){
    Nodo${tipo}* nodo = malloc(sizeof(Nodo${tipo}));
    nodo->dato = dato;
    nodo->precedente = null;
    nodo->successivo = null;
        }
    `)

    console.log(`^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ lista${tipo}.c - FINE ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^`)

    console.log(`============================================lista${tipo}.h - INIZIO============================================`)
   
        console.log(`


#ifndef LISTA${tipo.toUpperCase()}_H
#define LISTA${tipo.toUpperCase()}_H

#include <stdlib.h>
#include "tipi.h"

// Nodo ${tipo} per una lista doppiamente concatenata
typedef struct Nodo${tipo} {
    Nodo${tipo}* precedente;
    Nodo${tipo}* successivo;
    ${tipo} dato;
}

// Lista di ${tipo} con testa e coda
typedef struct Lista${tipo} {
    Nodo${tipo}* testa;
    Nodo${tipo}* coda;
}

Nodo${tipo}* pop${tipo}(Lista${tipo}* lista);
void push${tipo}(Lista${tipo}* lista, Nodo${tipo}* nodo);
void scollega${tipo}(Nodo${tipo}* nodo);
bool listaVuota${tipo}(Lista${tipo} lista);
Lista${tipo}* creaListaVuota${tipo}();
Nodo${tipo}* creaNodo${tipo}(${tipo} dato);

#endif /* LISTA${tipo.toUpperCase()}_H */
       `)
   
        console.log(`^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ lista${tipo}.h - FINE ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^`)
}