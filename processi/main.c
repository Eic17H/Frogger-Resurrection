#include "main.h"
#include <stdbool.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "altrecose.h"
#include "sprite.h"
#include "visualizzazione.h"
#include "inizializzazione.h"
#include "processi.h"
#include "rana.h"
#include "coccodrillo.h"
#include "struttureDati.h"
#include "costanti.h"
#include "regole.h"
#include "listaCoccodrillo.h"
#include "manche.h"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
int main() {
    // ======== ==== ==== ========
    // ======== VARIABILI ========
    // ======== ==== ==== ========
    int fd[2];
    int myPipe = pipe(fd);
    int x_rana = X_PARTENZA_RANA, y_rana = Y_PARTENZA_RANA, x_granata = NON_SU_SCHERMO, y_granata = NON_SU_SCHERMO;
    int indiceFlussoCoccodrilloPrimo; 
    Flusso flussi[N_FLUSSI];
    pid_t coccodrilliCreatiPerPrimi[N_FLUSSI];
    Tana tane[N_TANE];
    ListaCoccodrillo* listaCoccodrilli[N_FLUSSI];

    // ======== ==== = == = ==== ========
    // ======== INIZIALIZZAZIONE ========
    // ======== ==== = == = ==== ========
    srand(time(NULL));
    inizializzaNcurses();
    messaggioBenvenuto();
    inizializzaFinestra();

    pid_t pidRana = fork();

    // ERRORE
    if (forkFallita(pidRana)){
        endwin();
        perror("chiamata fork() rana");
        _exit(2);
    }
    // RANA
    else if(processoFiglio(pidRana)){
        close(fd[0]);
        rana(fd[1]);
    }
    // MAIN
    else if(processoPadre(pidRana)){
        // TODO: non sono ancora davvero indipendenti tra di loro
        manche(fd, flussi, listaCoccodrilli, pidRana, tane);
    inizializzaNcurses();
    messaggioBenvenuto();
    inizializzaFinestra();
        manche(fd, flussi, listaCoccodrilli, pidRana, tane);
    
        /**
         * Come gestire i coccodrilli
         * DISTMAX è la distanza tra due coccodrilli diciamo
         * Se la testa (della lista) è fuori dallo schermo, pop e dealloca
         * Se la coda (della lista)  è lontana >DISTMAX dal bordo dell'area di gioco, crea coccodrillo e push
         * Questo è consistente anche col fatto di creare un solo coccodrillo per riga all'inizio e far creare tutti gli altri in automatico
         */
         
    }

    endwin();
    return 0;
}
