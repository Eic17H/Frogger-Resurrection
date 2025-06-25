#include "main.h"
#include <stdbool.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
int main() {
    // Creazione variabili
    int fd[2], myPipe = pipe(fd), x_rana = X_PARTENZA_RANA, y_rana = Y_PARTENZA_RANA, x_granata = NON_SU_SCHERMO, y_granata = NON_SU_SCHERMO, indiceFlussoCoccodrilloPrimo; 
    Flusso flussi[N_FLUSSI];
    pid_t coccodrilliCreatiPerPrimi[N_FLUSSI];
    char* spriteCoccodrilloGiu = NULL, *spriteCoccodrillosu = NULL;
    Tana tane[N_TANE];
    ListaCoccodrillo* lista[N_FLUSSI];
    srand(time(NULL));

    // Inizializzazione
    inizializzaNcurses();
    messaggioBenvenuto();
    inizializzaFinestra();
    inizializzaPartita(N_TANE, N_FLUSSI, tane, flussi, lista, fd);

    pid_t pidRana = fork();

    if (forkFallita(pidRana)){
        endwin();
        perror("chiamata fork() rana");
        _exit(2);
    }
    if(processoFiglio(pidRana)){
        close(fd[0]);
        rana(fd[1]);
    }else if(processoPadre(pidRana)){
        // Inizializzazione variabili e timer
        bool vivo = true;
        time_t start, ora = 0;
        Messaggio messaggio;
        time(&start);
        //ora=start;
        int punteggio;

        // Loop della manche
        while(tempoScaduto(time(&ora), start) && vivo){
            read(fd[0], &messaggio, sizeof(Messaggio));
            spostaSprite(messaggio.mittente, messaggio.posVecchia, messaggio.posAttuale);
            
            if(messaggio.mittente == RANA) vivo = !cadutoInAcqua(messaggio.posAttuale);
            time(&ora);
            visualizzaTimer(ora-start);
            visualizzaPunteggio(punteggio);
            refresh();
        }
        kill(pidRana, SIGTERM);
    
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
