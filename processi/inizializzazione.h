#ifndef INIZIALIZZAZIONE_H
#define INIZIALIZZAZIONE_H

#include <ncurses.h>
#include "visualizzazione.h"
#include "listaCoccodrillo.h"

void inizializzaNcurses();
void adattaFinestra();
void coloraAmbienteGioco();
void inizializzaColori();
void messaggioBenvenuto();
void inizializzaFinestra();
void creaCoccodrilliIniziali();
void creaCoccodrillo(ListaCoccodrillo* lista, int fd[], Flusso flusso);
void creaTane(int nTane, Tana tane[nTane]);
void disegnaTane(int nTane, Tana tane[nTane]);
void inizializzaArrayFlussi(int nFlussi, Flusso flussi[nFlussi]);
void inizializzaListaCoccodrilli(int nFlussi, ListaCoccodrillo* lista[nFlussi]);
void creaCoccodrilliIniziali(int n, int fd[n], int nFlussi, Flusso flussi[nFlussi], ListaCoccodrillo* lista[nFlussi]);
void inizializzaManche(int nTane, int nFlussi, Tana tane[nTane], Flusso flussi[nFlussi], ListaCoccodrillo* lista[nFlussi], int fd[2]);

#endif /* INIZIALIZZAZIONE_H */
