#ifndef VISUALIZZAZIONE_H
#define VISUALIZZAZIONE_H

#include <ncurses.h>
#include <string.h>
#include "listaCoccodrillo.h"
#include "struttureDati.h"
#include "sprite.h"
#include "altrecose.h"

#define TESTO_CENTRATO(x) mvprintw(LINES/2, COLS/2 - strlen(x)/2, x);
#define TESTO_COORD_CENTRATO(w, s, x, y) mvwprintw(w, LINES/2, COLS/2 - strlen(s)/2, s, x, y);
#define TESTO_COORD_CENTRATO2(w, s, x, y) mvwprintw(w, LINES/2+1, COLS/2 - strlen(s)/2, s, x, y);

void spostaSprite(Messaggio messaggio);

void creaStringaVuota(int n, char* stringa);
void assegnaSpriteCoccodrillo(Posizione posAttuale, Posizione posVecchia, char* sprite1, char* sprite2);

void invertiStringa(char* stringa);

void inizializzaColoreSprite(int ySprite);

int calcolaDaTagliareR(Posizione pos);
int calcolaDaTagliareL(Posizione pos);

char* tagliaStringa(char* stringa, int lunghezza);

void selezionaColoreCoccodrillo(int versoCoccodrillo, bool staPerSparare);

void cancellaCoccodrillo(char stringaVuota[], Posizione posVecchia, int vecchioDaTagliareL);
void stampaCoccodrillo(char spriteSu[], char spriteGiu[], Posizione posAttuale, int daTagliareL);

void visualizzaTimer(int secondi);
void visualizzaPunteggio(int punteggio);
void visualizzaVite(int vite);
void visualizzaRoundRimasti(int roundRimasti);

void messaggioFinePartita(int nTaneOccupate, int punteggio);
#endif /* VISUALIZZAZIONE_H */
