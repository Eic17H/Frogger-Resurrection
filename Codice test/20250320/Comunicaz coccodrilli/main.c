#include <stdbool.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "main.h"
#include "rana.h"
#include "coccodrillo.h"

int main(){
        int fd[2], myPipe = pipe(fd), x_rana = X_PARTENZA_RANA, y_rana = Y_PARTENZA_RANA, x_granata = NON_SU_SCHERMO, y_granata = NON_SU_SCHERMO, indiceFlussoCoccodrilloPrimo; 
        Flusso flussi[N_FLUSSI];
        pid_t coccodrilliCreatiPerPrimi[N_FLUSSI];
        char* spriteCoccodrilloGiu = NULL, *spriteCoccodrillosu = NULL;
        Tana tane[N_TANE];
        srand(time(NULL));

	initscr(); // inizializzazione schermo ncurses prima della fork()
        keypad(stdscr, true);
        cbreak();
        nodelay(stdscr, true);
        noecho();
        curs_set(0);

        int dim_msg_benvenuto = strlen("BENVENUTO!")/2;
        mvprintw(LINES/2, COLS/2 - dim_msg_benvenuto, "BENVENUTO!");
        refresh();
        sleep(2);
        clear();

        if (LINES != DIM_LINES || COLS != DIM_COLS) {
                adattaFinestra();
        }       

        inizializzaColori();

        coloraAmbienteGioco();
        
        creaTane(N_TANE, tane);
        disegnaTane(N_TANE, tane);

        inizializzaArrayFlussi(N_FLUSSI, flussi);

        creaCoccodrilliIniziali(2, fd, N_FLUSSI, flussi, coccodrilliCreatiPerPrimi);

        pid_t pidRana = fork();
        if (pidRana < 0) { endwin(); perror("chiamata fork() rana"); _exit(2); };

        if (pidRana == 0) { // processo figlio
                close(fd[0]);
                rana(fd[1]);
        } 
        else { // processo padre
                int vivo;
                time_t start, ora = 0;
                Messaggio messaggio;
                time(&start);
                do {  
                        // lettura coordinate
                        read(fd[0], &messaggio, sizeof(Messaggio));
                        
                        switch (messaggio.mittente) {
                                case RANA:
                                        inizializzaColoreSprite(messaggio.posVecchia.y);
                                        
                                        mvaddstr(messaggio.posVecchia.y, messaggio.posVecchia.x, "    ");

                                        inizializzaColoreSprite(messaggio.posAttuale.y);

                                        y_rana = messaggio.posAttuale.y;
                                        x_rana = messaggio.posAttuale.x;
                                        
                                        mvaddstr(y_rana, x_rana, SPRITE_RANA);
                                break;

                                case GRANATA:
                                        inizializzaColoreSprite(messaggio.posAttuale.y);
                                
                                        mvaddstr(messaggio.posVecchia.y, messaggio.posVecchia.x, " ");
                                                                        
                                        if (messaggio.posAttuale.x < DIM_COLS && messaggio.posAttuale.x >= 0) {
                                                y_granata = messaggio.posAttuale.y;
                                                x_granata = messaggio.posAttuale.x;
                                                
                                                mvaddch(y_granata, x_granata, SPRITE_GRANATA);
                                        }
                                        else {
                                                kill(messaggio.pid, SIGKILL);
                                        }
                                break;

                                case COCCOD:
                                        attron(COLOR_PAIR(COCCODRILLO));
                                        cancellaVecchioCoccodrillo(messaggio.posVecchia);

                                        if (!coccodrilloFuoriSchermo(messaggio.posAttuale.x, messaggio.posVecchia.x)) {
                                                disegnaCoccodrillo(messaggio.posAttuale, messaggio.posAttuale.x - messaggio.posVecchia.x);
                                             
                                                indiceFlussoCoccodrilloPrimo = trovaPosCoccodrilloListaPrimi(messaggio.pid, N_FLUSSI, coccodrilliCreatiPerPrimi);
                                
                                                if (indiceFlussoCoccodrilloPrimo > NON_IN_LISTA && creareNuovoCoccodrillo(flussi[indiceFlussoCoccodrilloPrimo], messaggio)) {
                                                        creaNuovoPrimoCoccodrillo(N_FLUSSI, coccodrilliCreatiPerPrimi, flussi[indiceFlussoCoccodrilloPrimo], indiceFlussoCoccodrilloPrimo, 2, fd);
                                                }
                                        }
                                        else {
                                                kill(messaggio.pid, SIGKILL);
                                        }
                                break;
                        }

                        // stampa timer
                        attron(COLOR_PAIR(NERO));
                        time(&ora);
                        mvprintw(0, 0, "Tempo: %d ", DURATA_MANCHE_S - (int)(ora - start));
                        refresh();

                } while (ora - start < DURATA_MANCHE_S); 
                kill(pidRana, SIGTERM);
        }
        
        endwin();
        
	return 0;
}

int altezzaSponda(){
	return 6;
}

int altezzaMarciapiede(){
	return 6;
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
        for(int i=0; i<DIM_COLS+1; i++){
        	for(int j=0; j<altezzaSponda(); j++){
        		mvaddch(j+1, i, ' ');
        	}
        }
        // marciapiede bianco        
        attron(COLOR_PAIR(MARCIAPIEDE));
        for(int i=0; i<DIM_COLS+1; i++){
        	for(int j=0; j<altezzaMarciapiede(); j++){
        		mvaddch(LINES-1 -j, i, ' ');
        	}
        }

        refresh();
}

void inizializzaColoreSprite(int ySprite) {
        if (ySprite <= altezzaSponda()) {
                attron(COLOR_PAIR(SPONDA));
        }
        else if (ySprite >= LINES -altezzaMarciapiede()) {
                attron(COLOR_PAIR(MARCIAPIEDE));
        }
        else {
                attron(COLOR_PAIR(ACQUA));
        }
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
        char spriteTanaAperta[H_TANA][W_TANA+1] = {SPRITE_TANA_APERTA1, SPRITE_TANA_APERTA2, SPRITE_TANA_APERTA3, SPRITE_TANA_APERTA4};
        char spriteTanaChiusa[H_TANA][W_TANA+1] = {SPRITE_TANA_CHIUSA1, SPRITE_TANA_CHIUSA2, SPRITE_TANA_CHIUSA3, SPRITE_TANA_CHIUSA4};
        int indiceTana;

        for (int i = 0; i < nTane; i++) {
                indiceTana = 0;
                // si disegna la tana partendo dall'alto fino ad arrivare alla riva
                for (int j = altezzaSponda() - H_TANA + 1; j <= altezzaSponda(); j++) {
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
        refresh();
}

void inizializzaArrayFlussi(int nFlussi, Flusso flussi[nFlussi]) {
        int yPartenza = LINES - altezzaMarciapiede() - 1 - 1;
        //il primo -1 è perché il coccodrillo ha anche una sprite inferiore, il secondo è perché non deve partire a un pixel di distanza rispetto al marciapiede 

        for (int i = 0; i < nFlussi; i++) {
                // la metà dei flussi andrà da destra verso sinistra, l'altra in verso contrario
                if (i % 2 == 0) {
                        flussi[i].verso = AVANZAMENTO_DX;
                        // per far si che il coccodrillo si mostri grdualmente (senza essere disegnato fuori shcermo)
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

void creaCoccodrilliIniziali(int n, int fd[n], int nFlussi, Flusso flussi[nFlussi], pid_t coccodrilliCreatiPerPrimi[nFlussi]) {
        pid_t pidCoccodrillo;

        for (int i = 0; i < nFlussi; i++) {
 
                pidCoccodrillo = fork();
                if (pidCoccodrillo < 0) { endwin(); perror("chiamata fork() rana"); _exit(2); };

                if (pidCoccodrillo == 0) { // processo figlio
                        close(fd[0]);
                        coccodrillo(fd[1], flussi[i]); 
                }
                else {
                        coccodrilliCreatiPerPrimi[i] = pidCoccodrillo;
                }
        }
}

bool coccodrilloFuoriSchermo(int xAttuale, int xVecchia) {
        if (xAttuale - xVecchia >= AVANZAMENTO_DX && xAttuale > DIM_COLS - 1) { // se il coccodrillo che va verso destra (fa uno spostamento positivo) è fuori dalloo schermo
                return true;
        }
        if (xAttuale - xVecchia <= AVANZAMENTO_SX && xAttuale < -W_COCCODRILLO+1) { // se il coccodrillo che va verso sinistra (fa uno spostamento negativo) è fuori dalloo schermo
                return true;
        }
        return false;
}

int trovaPosCoccodrilloListaPrimi(pid_t pidCoccodrillo, int nFlussi, pid_t coccodrilliCreatiPerPrimi[nFlussi]) {
        int i = 0, posizCoccodrillo = NON_IN_LISTA;
        bool coccodrilloPrimo = false;

        while (i < nFlussi && !coccodrilloPrimo) {
                if (pidCoccodrillo == coccodrilliCreatiPerPrimi[i]) {
                        coccodrilloPrimo = true;
                        posizCoccodrillo = i;
                }
                i++;
        }

        return posizCoccodrillo;
}

bool creareNuovoCoccodrillo(Flusso flussoCoccodrillo, Messaggio messaggioCoccodrillo) {
        // se il coccodrillo va verso destra e la distanza dal margine sinistro permette di stampare un altro coccodrillo
        if (flussoCoccodrillo.verso == AVANZAMENTO_DX && messaggioCoccodrillo.posAttuale.x >= flussoCoccodrillo.distanzaCoccodrilli) {
                return true;
        }
        // se il coccodrillo va verso sinistra e la distanza dal margine deztro permette di stampare un altro coccodrillo
        if (flussoCoccodrillo.verso == AVANZAMENTO_SX && DIM_COLS - messaggioCoccodrillo.posAttuale.x - W_COCCODRILLO -1 >= flussoCoccodrillo.distanzaCoccodrilli) {
                return true;
        }
        return false;
}

void creaNuovoPrimoCoccodrillo(int nFlussi, pid_t coccodrilliCreatiPerPrimi[nFlussi], Flusso FlussoCoccodrillo, int indiceFlussoCoccodrillo, int n, int fd[n]) {
        pid_t pidNuovoCoccodrillo = fork();
        if (pidNuovoCoccodrillo < 0) { endwin(); perror("chiamata fork() nuovo coccodrillo"); _exit(2); }; 

        if (pidNuovoCoccodrillo == 0) {
                close(fd[0]);
                coccodrillo(fd[1], FlussoCoccodrillo);
        }
        else {
                coccodrilliCreatiPerPrimi[indiceFlussoCoccodrillo] = pidNuovoCoccodrillo;
        }
}

void cancellaVecchioCoccodrillo(Posizione posVecchia) {
        char spirteCoccodrilloVuoto[W_COCCODRILLO][W_COCCODRILLO+1] = {COCCODRILLO_NO, " ", "  ", "   ", "    ", "     ", "      ", "       "};
        int xSprite = posVecchia.x, pxAlLimiteSchermo = 0; 

        // se il coccodrillo sta sbucando gradualmente dal lato sinistro dello schermo (non si è ancora mostrato interamente)
        if (posVecchia.x >= -W_COCCODRILLO+1 && posVecchia.x <= -W_COCCODRILLO+7) {
                // i pixel che sporgono sono dati dalla lunghezza del coccodrillo + x (che qui è sempre negativa). Se x = -W_COCCODRILLO +1, sta sporgendo un solo pixel 
                pxAlLimiteSchermo = W_COCCODRILLO + posVecchia.x;
                xSprite = 0;
        }
        // se il coccodrillo sta scomparendo gradualmente dal lato destro dello schermo 
        else if (posVecchia.x >= DIM_COLS-7 && posVecchia.x <= DIM_COLS-1) {
                // se per esempio x = DIM_COLS - 1, facendo DIM_COLS - x si ottiene che al limite dello schermo c'è un solo pixel
                pxAlLimiteSchermo = DIM_COLS - posVecchia.x; 
        }
        // se il coccodrillo è intero, si stamperà la sprite di default (in posizione 0)
        
        mvaddstr(posVecchia.y, xSprite, spirteCoccodrilloVuoto[pxAlLimiteSchermo]); 
        mvaddstr(posVecchia.y + 1, xSprite, spirteCoccodrilloVuoto[pxAlLimiteSchermo]);
}

void disegnaCoccodrillo(Posizione posAttuale, int versoCoccodrillo) {
        // le colonne sono W_COCCODRILLO+1 perché a W_COCCODRILLO si aggiunge il terminatore di stringa /0
        char spriteCoccodrilloDxSu[W_COCCODRILLO][W_COCCODRILLO+1] = {COCCODRILLO_DX_SU,   "^", "^^", "_^^", "__^^", "___^^", "____^^", "_____^^"};
        char spriteCoccodrilloDxGiu[W_COCCODRILLO][W_COCCODRILLO+1] = {COCCODRILLO_DX_GIU, "<", "_<", "u_<", "uu_<", "_uu_<", "__uu_<", "u__uu_<"};
        char spriteCoccodrilloSxSu[W_COCCODRILLO][W_COCCODRILLO+1] = {COCCODRILLO_SX_SU,   "^", "^^", "^^_", "^^__", "^^___", "^^____", "^^_____"}; 
        char spriteCoccodrilloSxGiu[W_COCCODRILLO][W_COCCODRILLO+1] = {COCCODRILLO_SX_GIU, ">", ">_", ">_u", ">_uu", ">_uu_", ">_uu__", ">_uu__u"};
        int xSprite = posAttuale.x, pxAlLimiteSchermo = 0; 

        // se il coccodrillo sta sbucando gradualmente dal lato sinistro dello schermo (non si è ancora mostrato interamente)
        if (posAttuale.x >= -W_COCCODRILLO+1 && posAttuale.x <= -W_COCCODRILLO+7) {
                // i pixel che sporgono sono dati dalla lunghezza del coccodrillo + x (che qui è sempre negativa). Se x = -W_COCCODRILLO +1, sta sporgendo un solo pixel 
                pxAlLimiteSchermo = W_COCCODRILLO + posAttuale.x;
                xSprite = 0;
        }
        // se il coccodrillo sta scomparendo gradualmente dal lato destro dello schermo 
        else if (posAttuale.x >= DIM_COLS-7 && posAttuale.x <= DIM_COLS-1) {
                // se per esempio x = DIM_COLS - 1, facendo DIM_COLS - x si ottiene che al limite dello schermo c'è un solo pixel
                pxAlLimiteSchermo = DIM_COLS - posAttuale.x; 
        }
        // se il coccodrillo è intero, si stamperà la sprite di default (in posizione 0)

        if (versoCoccodrillo >= AVANZAMENTO_DX) {
                mvaddstr(posAttuale.y, xSprite, spriteCoccodrilloDxSu[pxAlLimiteSchermo]); 
                mvaddstr(posAttuale.y + 1, xSprite, spriteCoccodrilloDxGiu[pxAlLimiteSchermo]);
                return ;
        }

        mvaddstr(posAttuale.y, xSprite, spriteCoccodrilloSxSu[pxAlLimiteSchermo]); 
        mvaddstr(posAttuale.y + 1, xSprite, spriteCoccodrilloSxGiu[pxAlLimiteSchermo]);
}