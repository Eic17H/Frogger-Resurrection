#include "visualizzazione.h"

void inizalizzaNcurses() {
    initscr();
    keypad(stdscr, true);
    cbreak();
    nodelay(stdscr, true);
    noecho();
    curs_set(0);
}

void inizalizzaFinestra() {
    if (LINES != DIM_LINES || COLS != DIM_COLS) adattaFinestra();
    inizializzaColori();
    coloraAmbienteGioco();
}

void adattaFinestra() {
    int dim_msg_dim = strlen("Ridimensiona lo schermo per un'esperienza ottimale")/2;
    int dim1 = strlen("TU: LINES = xx COLS = yy")/2;  
    int dim2 = strlen("DIM: LINES = xx COLS = yy")/2;

    mvprintw(LINES/2, COLS/2 - dim_msg_dim, "Ridimensiona lo schermo per un'esperienza ottimale");
    refresh();
    sleep(2);

    clear();
    refresh();
    
    while (LINES != DIM_LINES || COLS != DIM_COLS) {
            WINDOW* indicaz_win = newwin(DIM_LINES, DIM_COLS, 0, 0);
            box(indicaz_win, ACS_VLINE, ACS_HLINE);
            mvwprintw(indicaz_win, DIM_LINES/2, DIM_COLS/2 - dim1, "TU: LINES = %d COLS = %d", LINES, COLS);
            mvwprintw(indicaz_win, DIM_LINES/2 + 1, DIM_COLS/2 - dim2, "DIM: LINES = %d COLS = %d", DIM_LINES, DIM_COLS);
            wrefresh(indicaz_win);
            
            delwin(indicaz_win);
    }
    usleep(10000);
    clear();
}

void messaggioBenvenuto() {
    TESTO_CENTRATO("BENVENUTO!");
    refresh();
    sleep(2);
    clear();
}

void inizializzaPartita() {
    creaTane(N_TANE, tane);
    disegnaTane(N_TANE, tane);
    inizializzaArrayFlussi(N_FLUSSI, flussi);
    creaCoccodrilliIniziali(2, fd, N_FLUSSI, flussi, coccodrilliCreatiPerPrimi);
}