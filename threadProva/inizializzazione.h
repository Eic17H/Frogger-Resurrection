#ifndef INIZIALIZZAZIONE_H
#define INIZIALIZZAZIONE_H

#include <ncurses.h>
#include "listaGranate.h"
#include "rana.h"
#include "thread.h"
#include "visualizzazione.h"
#include "listaCoccodrillo.h"

void inizializzaNcurses();
void adattaFinestra();
void coloraAmbienteGioco();
void inizializzaColori();
void messaggioBenvenuto();
void creaCoccodrillo(ListaCoccodrillo* lista, Flusso flusso, TuttoBuffer* buffer);
void creaProcessoGranata(Mittente mittente, Posizione posPartenza, int direzione, ListaGranata* listaGranate, TuttoBuffer* buffer);
void creaProcessoProiettile(Mittente mittente, Posizione posPartenza, int direzione, TuttoBuffer* buffer);
void creaTane(int nTane, Tana tane[nTane]);
void disegnaTane(TuttoBuffer* buffer, int nTane, Tana tane[nTane]);
int generaVersoFlussoIniziale();
void inizializzaArrayFlussi(int nFlussi, Flusso flussi[nFlussi]);
void inizializzaListaCoccodrilli(int nFlussi, ListaCoccodrillo* lista[nFlussi]);
void creaCoccodrilliIniziali(int nFlussi, Flusso flussi[nFlussi], ListaCoccodrillo* lista[nFlussi], TuttoBuffer* buffer);
void inizializzaManche(int nTane, int nFlussi, Tana tane[nTane], Flusso flussi[nFlussi], ListaCoccodrillo* listaCocco[nFlussi], ListaGranata** listaGranate, TuttoBuffer* buffer);

#endif /* INIZIALIZZAZIONE_H */
