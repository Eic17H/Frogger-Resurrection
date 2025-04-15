#ifndef VISUALIZZAZIONE_H
#define VISUALIZZAZIONE_H

#include <ncurses.h>

#define TESTO_CENTRATO(x) mvprintw(LINES/2, COLS/2 - strlen("BENVENUTO!")/2, "BENVENUTO!");

#endif /* VISUALIZZAZIONE_H */
