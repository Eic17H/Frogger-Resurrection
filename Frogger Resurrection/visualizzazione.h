#ifndef VISUALIZZAZIONE_H
#define VISUALIZZAZIONE_H

#include <ncurses.h>
#include <string.h>
#include "struttureDati.h"
#include "sprite.h"
#include "altrecose.h"

#define TESTO_CENTRATO(x) mvprintw(LINES/2, COLS/2 - strlen("BENVENUTO!")/2, "BENVENUTO!");

void spostaSprite(Mittente mittente, Posizione posVecchia, Posizione posAttuale);
void visualizzaTimer(int secondi);
void visualizzaPunteggio(int punteggio);

#endif /* VISUALIZZAZIONE_H */
