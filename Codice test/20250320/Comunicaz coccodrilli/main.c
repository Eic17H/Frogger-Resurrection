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
        
        inizializzaArrayFlussi(N_FLUSSI, flussi);

        creaCoccodrilliIniziali(2, fd, N_FLUSSI, flussi, coccodrilliCreatiPerPrimi);

        pid_t pidRana = fork();
        if (pidRana < 0) { endwin(); perror("chiamata fork() rana"); _exit(2); };

        if (pidRana == 0) { // processo figlio
                close(fd[0]);
                rana(2, fd);
        } 
        else { // processo padre
                int vivo;
                time_t start, ora = 0;
                Messaggio messaggio;
                time(&start);
                do {  
                        // lettura coordinate
                        //close(fd[1]);
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
        if (xAttuale - xVecchia >= AVANZAMENTO_DX && xAttuale >= DIM_COLS - 1) { // se il coccodrillo che va verso destra (fa uno spostamento positivo) è fuori dalloo schermo
                return true;
        }
        if (xAttuale - xVecchia <= AVANZAMENTO_SX && xAttuale <= -W_COCCODRILLO+1) { // se il coccodrillo che va verso sinistra (fa uno spostamento negativo) è fuori dalloo schermo
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
        if (flussoCoccodrillo.verso == AVANZAMENTO_DX && messaggioCoccodrillo.posAttuale.x - W_COCCODRILLO >= flussoCoccodrillo.distanzaCoccodrilli) {
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
        switch(posVecchia.x) {
                case -W_COCCODRILLO+1:
                        mvaddstr(posVecchia.y, 0, " ");
                        mvaddstr(posVecchia.y + 1, 0, " ");
                break;

                case -W_COCCODRILLO+2: 
                        mvaddstr(posVecchia.y, 0, "  ");
                        mvaddstr(posVecchia.y + 1, 0, "  ");
                break;

                case -W_COCCODRILLO+3:
                        mvaddstr(posVecchia.y, 0, "   ");
                        mvaddstr(posVecchia.y + 1, 0, "   ");
                break;

                case -W_COCCODRILLO+4:
                        mvaddstr(posVecchia.y, 0, "    ");
                        mvaddstr(posVecchia.y + 1, 0, "    ");
                break;

                case -W_COCCODRILLO+5:
                        mvaddstr(posVecchia.y, 0, "     ");
                        mvaddstr(posVecchia.y + 1, 0, "     ");
                break;

                case -W_COCCODRILLO+6:
                        mvaddstr(posVecchia.y, 0, "      ");
                        mvaddstr(posVecchia.y + 1, 0, "      ");
                break;

                case -W_COCCODRILLO+7:
                        mvaddstr(posVecchia.y, 0, "       ");
                        mvaddstr(posVecchia.y + 1, 0, "       ");
                break; 

                case DIM_COLS-1:
                        mvaddstr(posVecchia.y, posVecchia.x, " ");
                        mvaddstr(posVecchia.y + 1, posVecchia.x, " ");
                break;          

                case DIM_COLS-2:
                        mvaddstr(posVecchia.y, posVecchia.x, "  ");
                        mvaddstr(posVecchia.y + 1, posVecchia.x, "  ");
                break;                       

                case DIM_COLS-3:
                        mvaddstr(posVecchia.y, posVecchia.x, "   ");
                        mvaddstr(posVecchia.y + 1, posVecchia.x, "   ");
                break;                       

                case DIM_COLS-4:
                        mvaddstr(posVecchia.y, posVecchia.x, "    ");
                        mvaddstr(posVecchia.y + 1, posVecchia.x, "    ");
                break;                       

                case DIM_COLS-5:
                        mvaddstr(posVecchia.y, posVecchia.x, "     ");
                        mvaddstr(posVecchia.y + 1, posVecchia.x, "     ");
                break;                       

                case DIM_COLS-6:
                        mvaddstr(posVecchia.y, posVecchia.x, "      ");
                        mvaddstr(posVecchia.y + 1, posVecchia.x, "      ");
                break;                       

                case DIM_COLS-7:
                        mvaddstr(posVecchia.y, posVecchia.x, "       ");
                        mvaddstr(posVecchia.y + 1, posVecchia.x, "       ");
                break;                       

                default:
                        mvaddstr(posVecchia.y, posVecchia.x, COCCODRILLO_NO);
                        mvaddstr(posVecchia.y + 1, posVecchia.x, COCCODRILLO_NO);
                break;
        }
}

void disegnaCoccodrillo(Posizione posAttuale, int versoCoccodrillo) {
        if (versoCoccodrillo >= AVANZAMENTO_DX) {
                switch (posAttuale.x) {
                        case -W_COCCODRILLO+1:
                                mvaddstr(posAttuale.y, 0, "^");
                                mvaddstr(posAttuale.y + 1, 0, "<");
                        break;
                        
                        case -W_COCCODRILLO+2:
                                mvaddstr(posAttuale.y, 0, "^^");
                                mvaddstr(posAttuale.y + 1, 0, "_<");
                        break;
                        
                        case -W_COCCODRILLO+3:
                                mvaddstr(posAttuale.y, 0, "_^^");
                                mvaddstr(posAttuale.y + 1, 0, "u_<");
                        break;
                        
                        case -W_COCCODRILLO+4:
                                mvaddstr(posAttuale.y, 0, "__^^");
                                mvaddstr(posAttuale.y + 1, 0, "uu_<");
                        break;
                        
                        case -W_COCCODRILLO+5:
                                mvaddstr(posAttuale.y, 0, "___^^");
                                mvaddstr(posAttuale.y + 1, 0, "_uu_<");
                        break;
                        
                        case -W_COCCODRILLO+6:
                                mvaddstr(posAttuale.y, 0, "____^^");
                                mvaddstr(posAttuale.y + 1, 0, "__uu_<");
                        break;
                        
                        case -W_COCCODRILLO+7:
                                mvaddstr(posAttuale.y, 0, "_____^^");
                                mvaddstr(posAttuale.y + 1, 0, "u__uu_<");
                        break; 
                        
                        default:
                                mvaddstr(posAttuale.y, posAttuale.x, COCCODRILLO_SU);
                                mvaddstr(posAttuale.y + 1, posAttuale.x, COCCODRILLO_GIU);
                        break;
                }
                return;
        }

        switch(posAttuale.x) {
                case DIM_COLS-1:
                        mvaddstr(posAttuale.y, posAttuale.x, "^");
                        mvaddstr(posAttuale.y + 1, posAttuale.x, "<");
                break;          
                
                case DIM_COLS-2:
                        mvaddstr(posAttuale.y, posAttuale.x, "^^");
                        mvaddstr(posAttuale.y + 1, posAttuale.x, ">_");
                break;                       
                
                case DIM_COLS-3:
                        mvaddstr(posAttuale.y, posAttuale.x, "^^_");
                        mvaddstr(posAttuale.y + 1, posAttuale.x, ">_u");
                break;                       
                
                case DIM_COLS-4:
                        mvaddstr(posAttuale.y, posAttuale.x, "^^__");
                        mvaddstr(posAttuale.y + 1, posAttuale.x, ">_uu");
                break;                       
                
                case DIM_COLS-5:
                        mvaddstr(posAttuale.y, posAttuale.x, "^^___");
                        mvaddstr(posAttuale.y + 1, posAttuale.x, ">_uu_");
                break;                       
                
                case DIM_COLS-6:
                        mvaddstr(posAttuale.y, posAttuale.x, "^^____");
                        mvaddstr(posAttuale.y + 1, posAttuale.x, ">_uu__");
                break;                       
                
                case DIM_COLS-7:
                        mvaddstr(posAttuale.y, posAttuale.x, "^^_____");
                        mvaddstr(posAttuale.y + 1, posAttuale.x, ">_uu__u");
                break;                       

                default:
                        mvaddstr(posAttuale.y, posAttuale.x, COCCODRILLO_DX_SU);
                        mvaddstr(posAttuale.y + 1, posAttuale.x, COCCODRILLO_DX_GIU);
                break;
        }
}