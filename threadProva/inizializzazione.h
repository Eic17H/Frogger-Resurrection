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
void coloraAmbienteGioco(TuttoBuffer* buffer);
void inizializzaColori();
void messaggioBenvenuto();
void creaCoccodrillo(ListaCoccodrillo* lista, int fd[], Flusso flusso, TuttoBuffer* buffer);
pid_t creaRana(int n, int fd[n], TuttoBuffer* buffer);
void creaProcessoGranata(Mittente mittente, int fdScrittura, Posizione posPartenza, int direzione, ListaGranata* listaGranate, TuttoBuffer* buffer);
void creaProcessoProiettile(Mittente mittente, int fdScrittura, Posizione posPartenza, int direzione, TuttoBuffer* buffer);
void creaTane(int nTane, Tana tane[nTane]);
void disegnaTane(TuttoBuffer* buffer, int nTane, Tana tane[nTane]);
int generaVersoFlussoIniziale();
void inizializzaArrayFlussi(int nFlussi, Flusso flussi[nFlussi]);
void inizializzaListaCoccodrilli(int nFlussi, ListaCoccodrillo* lista[nFlussi]);
void creaCoccodrilliIniziali(int n, int fd[n], int nFlussi, Flusso flussi[nFlussi], ListaCoccodrillo* lista[nFlussi], TuttoBuffer* buffer);
void inizializzaManche(int nTane, int nFlussi, Tana tane[nTane], Flusso flussi[nFlussi], ListaCoccodrillo* listaCocco[nFlussi], ListaGranata** listaGranate, int fd[2], TuttoBuffer* buffer);

#endif /* INIZIALIZZAZIONE_H */
