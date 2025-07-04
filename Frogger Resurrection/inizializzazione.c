#include <stdlib.h>
#include <ncurses.h>

#include "inizializzazione.h"
#include "listaCoccodrillo.h"
#include "processi.h"
#include "visualizzazione.h"
#include "costanti.h"
#include "coccodrillo.h"
#include "listaCoccodrillo.h"
   
void inizializzaNcurses() {
    initscr();
    keypad(stdscr, true);
    cbreak();
    nodelay(stdscr, true);
    noecho();
    curs_set(0);
}

void messaggioBenvenuto() {
    TESTO_CENTRATO("BENVENUTO!");
    refresh();
    sleep(2);
    clear();
}

void inizializzaColori() {
    start_color();
    init_pair(MARCIAPIEDE, COLOR_BLACK, COLOR_WHITE);
    init_pair(ACQUA, COLOR_WHITE, COLOR_BLUE);
    init_pair(SPONDA, COLOR_BLACK, COLOR_GREEN);
    init_pair(COCCODRILLO, COLOR_GREEN, COLOR_BLUE);
    init_pair(RANA_SU_COCCODRILLO, COLOR_WHITE, COLOR_GREEN);
    init_pair(NERO, COLOR_WHITE, COLOR_BLACK);
}

void coloraAmbienteGioco() {
    // sfondo blu
    attron(COLOR_PAIR(ACQUA));
    bkgd(COLOR_PAIR(ACQUA));
    // barra superiore        
    attron(COLOR_PAIR(NERO));
    for(int i=0; i<DIM_COLS+1; i++){
    	mvaddch(0, i, ' ');
    }
    // sponda verde
    attron(COLOR_PAIR(SPONDA));
    for(int i=0; i< DIM_COLS+1; i++){
    	for(int j=0; j< H_SPONDA; j++){
    		mvaddch(j+1, i, ' ');
    	}
    }
    // marciapiede bianco        
    attron(COLOR_PAIR(MARCIAPIEDE));
    for(int i=0; i<DIM_COLS+1; i++){
    	for(int j=0; j< H_MARCIAPIEDE; j++){
    		mvaddch(LINES-1 -j, i, ' ');
    	}
    }
}

void adattaFinestra() {
    int dim1 = strlen("TU: LINES = xx COLS = yy")/2;  
    int dim2 = strlen("DIM: LINES = xx COLS = yy")/2;

    TESTO_CENTRATO("Ridimensiona lo schermo per un'esperienza ottimale");
    refresh();
    sleep(2);

    clear();
    refresh();
    
    while (LINES != DIM_LINES || COLS != DIM_COLS) {
        WINDOW* indicaz_win = newwin(DIM_LINES, DIM_COLS, 0, 0);
        box(indicaz_win, ACS_VLINE, ACS_HLINE);
    
        TESTO_COORD_CENTRATO(indicaz_win, "TU: LINES = %d COLS = %d", LINES, COLS);
        TESTO_COORD_CENTRATO2(indicaz_win, "DIM: LINES = %d COLS = %d", DIM_LINES, DIM_COLS);

        wrefresh(indicaz_win);
        
        delwin(indicaz_win);
    }
    usleep(10000);
    clear();
}

void inizializzaFinestra() {
    if (LINES != DIM_LINES || COLS != DIM_COLS) adattaFinestra();
    inizializzaColori();
    coloraAmbienteGioco();
    refresh();
}

void creaTane(int nTane, Tana tane[nTane]) {
    int wTotTane = W_TANA*nTane, wSpondaLibera = DIM_COLS - wTotTane, spazioFraTane = wSpondaLibera / N_SEPARATORI_TANE; 
    int indiceTane = 0;
    // si sposta il cursore nel punto giusto per disegnare la nuova tana
    for (int j = spazioFraTane; j < DIM_COLS; j += W_TANA + spazioFraTane) { 
        // inizialmente tutte le tane sono aperte
        tane[indiceTane].chiusa = false;
        tane[indiceTane].y = H_TANA;
        
        tane[indiceTane].xInizio = j;
        tane[indiceTane].xFine = j + W_TANA;
        indiceTane++;
    }
}

void disegnaTane(int nTane, Tana tane[nTane]) {
    // W_TANA+1 per includere il terminatore di stringa /0
    char spriteTanaAperta[H_TANA][W_TANA+1] = {SPR_TANA_APERTA1, SPR_TANA_APERTA2, SPR_TANA_APERTA3, SPR_TANA_APERTA4};
    char spriteTanaChiusa[H_TANA][W_TANA+1] = {SPR_TANA_CHIUSA1, SPR_TANA_CHIUSA2, SPR_TANA_CHIUSA3, SPR_TANA_CHIUSA4};
    int indiceTana;
    for (int i = 0; i < nTane; i++) {
        indiceTana = 0;
        // si disegna la tana partendo dall'alto fino ad arrivare alla riva
        for (int j = H_SPONDA - H_TANA + 1; j <= H_SPONDA; j++) {
            attron(COLOR_PAIR(SPONDA));
            if (!tane[i].chiusa) {
                mvaddstr(j, tane[i].xInizio, spriteTanaAperta[indiceTana]);
            }
            else {
                mvaddstr(j, tane[i].xInizio, spriteTanaChiusa[indiceTana]);
            }
            indiceTana++;
        }
    }
}

void inizializzaArrayFlussi(int nFlussi, Flusso flussi[nFlussi]) {
    int yPartenza = DIM_LINES - H_MARCIAPIEDE - 1 - 1;
    //il primo -1 è perché il coccodrillo ha anche una sprite inferiore, il secondo è perché non deve partire a un pixel di distanza rispetto al marciapiede 
    for (int i = 0; i < nFlussi; i++) {
        // la metà dei flussi andrà da destra verso sinistra, l'altra in verso contrario
        if (i % 2 == 0) {
            flussi[i].verso = AVANZAMENTO_DX;
            // per far si che il coccodrillo si mostri grdualmente (senza essere disegnato fuori schermo)
            flussi[i].posIniziale.x = -W_COCCODRILLO+1;  
        }
        else {
            flussi[i].verso = AVANZAMENTO_SX;
            flussi[i].posIniziale.x = DIM_COLS - 1;
        }
        flussi[i].posIniziale.y = yPartenza;
        
        // velocità e distanza fra coccodrilli del flusso sono estratti casualmente
        flussi[i].velocità = MIN_VELOCITA_COCCO + rand() % (MAX_VELOCITA_COCCO - MIN_VELOCITA_COCCO + 1);
        flussi[i].distanzaCoccodrilli = MIN_SPAZIO_FRA_COCCO + rand() % (MAX_SPAZIO_FRA_COCCO - MIN_SPAZIO_FRA_COCCO + 1);
        yPartenza -= DISTANZA_FLUSSI + 1; // +1 per la sprite inferiore del coccodrillo 
    }
}

void inizializzaListaCoccodrilli(int nFlussi, ListaCoccodrillo* lista[nFlussi]) {
    for (int i = 0; i < nFlussi; i++) {
        lista[i] = creaListaVuotaCoccodrillo();
    }
}

void creaCoccodrilliIniziali(int n, int fd[n], int nFlussi, Flusso flussi[nFlussi], ListaCoccodrillo* lista[nFlussi]) {
    for (int i = 0; i < nFlussi; i++) {
        creaCoccodrillo(lista[i], fd, flussi[i]);
    }
}

void creaCoccodrillo(ListaCoccodrillo* lista, int fd[], Flusso flusso) {
    pid_t pidCoccodrillo;
    Coccodrillo nuovoCoccodrillo;
    NodoCoccodrillo* nuovoNodo = NULL;

    pidCoccodrillo = fork();
    if (forkFallita(pidCoccodrillo)) { endwin(); perror("chiamata fork() coccodrillo"); _exit(2); };
    if (processoFiglio(pidCoccodrillo)) {
        close(fd[0]); 
        coccodrillo(fd[1], flusso); 
    }
    else {
        nuovoCoccodrillo = assegnaDatiCoccodrillo(pidCoccodrillo, flusso.posIniziale, flusso);
        nuovoNodo = creaNodoCoccodrillo(nuovoCoccodrillo);
        pushCoccodrillo(lista, nuovoNodo);
    }
}

void inizializzaPartita(int nTane, int nFlussi, Tana tane[nTane], Flusso flussi[nFlussi], ListaCoccodrillo* lista[nFlussi], int fd[2]) {
    creaTane(N_TANE, tane);
    disegnaTane(N_TANE, tane);
    inizializzaArrayFlussi(N_FLUSSI, flussi);
    inizializzaListaCoccodrilli(nFlussi, lista);
    creaCoccodrilliIniziali(2, fd, N_FLUSSI, flussi, lista);
    refresh();
}